#pragma once

#include "WrenHost.h"
#include "ExtensionManager.h"
#include "GenerateState.h"

namespace Soup::Core::Generate
{
	class GenerateEngine
	{
	public:
		static void Run(const Path& soupTargetDirectory)
		{
			// Run all build operations in the correct order with incremental build checks
			Log::Diag("Build generate start: " + soupTargetDirectory.ToString());

			// Load the parameters file
			auto parametersFile = soupTargetDirectory + BuildConstants::GenerateParametersFileName();
			auto parametersState = ValueTable();
			if (!ValueTableManager::TryLoadState(parametersFile, parametersState))
			{
				Log::Error("Failed to load the parameter file: " + parametersFile.ToString());
				throw std::runtime_error("Failed to load parameter file.");
			}

			// Load the read access file
			auto readAccessFile = soupTargetDirectory + BuildConstants::GenerateReadAccessFileName();
			auto readAccessList = std::vector<Path>();
			if (!PathListManager::TryLoad(readAccessFile, readAccessList))
			{
				Log::Error("Failed to load the read access file: " + readAccessFile.ToString());
				throw std::runtime_error("Failed to load read access file.");
			}

			// Load the write access file
			auto writeAccessFile = soupTargetDirectory + BuildConstants::GenerateWriteAccessFileName();
			auto writeAccessList = std::vector<Path>();
			if (!PathListManager::TryLoad(writeAccessFile, writeAccessList))
			{
				Log::Error("Failed to load the write access file: " + writeAccessFile.ToString());
				throw std::runtime_error("Failed to load write access file.");
			}

			// Get the required input state from the parameters
			auto languageExtensionResult = parametersState.find("LanguageExtension");
			std::optional<std::vector<Path>> languageExtension = std::nullopt;
			if (languageExtensionResult != parametersState.end())
			{
				auto files = std::vector<Path>();
				for (auto& file : languageExtensionResult->second.AsList())
				{
					files.push_back(Path(file.AsString()));
				}

				languageExtension = std::move(files);
			}

			auto packageDirectory = Path(parametersState.at("PackageDirectory").AsString());

			// Load the recipe file
			auto recipeFile = packageDirectory + BuildConstants::RecipeFileName();
			Recipe recipe;
			if (!RecipeExtensions::TryLoadRecipeFromFile(recipeFile, recipe))
			{
				Log::Error("Failed to load the recipe: " + recipeFile.ToString());
				throw std::runtime_error("Failed to load recipe.");
			}

			// Combine all the dependencies shared state
			auto dependenciesSharedState = LoadDependenciesSharedState(parametersState);

			// Generate the set of build extension libraries
			auto buildExtensionLibraries = GenerateBuildExtensionSet(
				languageExtension,
				dependenciesSharedState);

			// Start a new global state that is initialized to the recipe itself
			auto globalState = ValueTable();

			// Initialize the Recipe Root Table
			auto recipeState = RecipeBuildStateConverter::ConvertToBuildState(recipe.GetTable());
			globalState.emplace("Recipe", Value(std::move(recipeState)));

			// Initialize the Parameters Root Table
			globalState.emplace("Parameters", Value(std::move(parametersState)));

			// Initialize the Dependencies Root Table
			globalState.emplace("Dependencies", Value(std::move(dependenciesSharedState)));

			// Create a new build system for the requested build
			auto extensionManager = ExtensionManager();

			// Run all build extension register callbacks
			for (auto buildExtension : buildExtensionLibraries)
			{
				Log::Info("Loading Extension Script: " + buildExtension.ToString());

				// Create a temporary Wren Host to discover all build extensions
				auto host = std::make_unique<WrenHost>(buildExtension);
				host->InterpretMain();
				auto extensions = host->DiscoverExtensions();

				for (auto& extension : extensions)
					extensionManager.RegisterExtension(std::move(extension));
			}

			// Initialize a shared File System State to cache file system access
			auto fileSystemState = FileSystemState();

			// Evaluate the build extensions
			auto buildState = GenerateState(
				globalState,
				fileSystemState,
				readAccessList,
				writeAccessList);
			extensionManager.Execute(buildState);

			// Grab the build results
			auto generateInfoTable = buildState.GetGenerateInfo();
			auto evaluateGraph = buildState.BuildOperationGraph();
			auto sharedState = buildState.GetSharedState();

			// Save the runtime information so Soup View can easily visualize runtime
			auto generateInfoStateFile = soupTargetDirectory + BuildConstants::GenerateExtensionInfoFileName();
			Log::Info("Save Generate Info State: " + generateInfoStateFile.ToString());
			ValueTableManager::SaveState(generateInfoStateFile, generateInfoTable);

			// Save the operation graph so the evaluate phase can load it
			auto evaluateGraphFile = soupTargetDirectory + BuildConstants::EvaluateGraphFileName();
			OperationGraphManager::SaveState(evaluateGraphFile, evaluateGraph, fileSystemState);

			// Save the shared state that is to be passed to the downstream builds
			auto sharedStateFile = soupTargetDirectory + BuildConstants::GenerateSharedStateFileName();
			ValueTableManager::SaveState(sharedStateFile, sharedState);

			Log::Diag("Build generate end");
		}

	private:
		/// <summary>
		/// Using the parameters to resolve the dependency output folders, load up the shared state table and
		/// combine them into a single value table to be used as input the this generate phase.
		/// </summary>
		static ValueTable LoadDependenciesSharedState(const ValueTable& parametersTable)
		{
			auto sharedDependenciesTable = ValueTable();
			auto dependencyTableValue = parametersTable.find("Dependencies");
			if (dependencyTableValue != parametersTable.end())
			{
				auto& dependenciesTable = dependencyTableValue->second.AsTable();
				for (auto dependencyTypeValue : dependenciesTable)
				{
					auto dependencyType = dependencyTypeValue.first;
					auto& dependencies = dependencyTypeValue.second.AsTable();
					for (auto dependencyValue : dependencies)
					{
						auto dependencyName = dependencyValue.first;
						auto& dependency = dependencyValue.second.AsTable();
						auto soupTargetDirectory = Path(dependency.at("SoupTargetDirectory").AsString());
						auto sharedStateFile = soupTargetDirectory + BuildConstants::GenerateSharedStateFileName();

						// Load the shared state file
						auto sharedStateTable = ValueTable();
						if (!ValueTableManager::TryLoadState(sharedStateFile, sharedStateTable))
						{
							Log::Error("Failed to load the shared state file: " + sharedStateFile.ToString());
							throw std::runtime_error("Failed to load shared state file.");
						}

						// Add the shared build state from this child build into the correct
						// table depending on the build type
						auto& typedDependenciesTable = EnsureValueTable(sharedDependenciesTable, dependencyType);
						typedDependenciesTable.emplace(
							dependencyName,
							Value(sharedStateTable));
					}
				}
			}

			return sharedDependenciesTable;
		}

		static ValueTable& EnsureValueTable(ValueTable& table, const std::string& key)
		{
			auto findResult = table.find(key);
			if (findResult != table.end())
			{
				return findResult->second.AsTable();
			}
			else
			{
				auto insertResult = table.emplace(key, Value(ValueTable()));
				return insertResult.first->second.AsTable();
			}
		}

		/// <summary>
		/// Generate the collection of build extensions
		/// </summary>
		static std::vector<Path> GenerateBuildExtensionSet(
			const std::optional<std::vector<Path>>& languageExtension,
			const ValueTable& dependenciesSharedState)
		{
			auto buildExtensionLibraries = std::vector<Path>();

			// Run the RecipeBuild extension to inject core build tasks
			if (languageExtension.has_value())
			{
				std::copy(
					languageExtension.value().begin(),
					languageExtension.value().end(),
					std::back_inserter(buildExtensionLibraries));
			}

			// Check for any dynamic libraries in the shared state
			auto buildDependenciesValue = dependenciesSharedState.find("Build");
			if (buildDependenciesValue != dependenciesSharedState.end())
			{
				for (auto dependencyValue : buildDependenciesValue->second.AsTable())
				{
					auto& dependency = dependencyValue.second.AsTable();
					auto buildTableValue = dependency.find("Build");
					if (buildTableValue != dependency.end())
					{
						auto& buildTable = buildTableValue->second.AsTable();
						auto targetFileValue = buildTable.find("TargetFile");
						if (targetFileValue != buildTable.end())
						{
							auto targetFile = Path(targetFileValue->second.AsString());
							buildExtensionLibraries.push_back(std::move(targetFile));
						}
						else
						{
							Log::Warning("Found build dependency with no target file.");
						}
					}
					else
					{
						Log::Warning("Found build dependency with no build table.");
					}
				}
			}

			return buildExtensionLibraries;
		}
	};
}
