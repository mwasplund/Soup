﻿// <copyright file="BuildRunner.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "FileSystemState.h"
#include "OperationGraphManager.h"
#include "SystemAccessTracker.h"

namespace Soup::Build::Execute
{
	/// <summary>
	/// The build runner
	/// </summary>
	export class BuildRunner
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildRunner"/> class.
		/// </summary>
		BuildRunner(
			Path workingDirectory,
			FileSystemState& fileSystemState) :
			_workingDirectory(std::move(workingDirectory)),
			_fileSystemState(fileSystemState),
			_dependencyCounts(),
			_previousOperationGraph(0),
			_activeOperationGraph(_fileSystemState.GetId()),
			_stateChecker(fileSystemState)
		{
		}

		// TODO: Convert vector to const when we have a const version of the operation wrapper.
		void Execute(
			Utilities::BuildOperationListWrapper& operations,
			const Path& outputDirectory,
			bool forceBuild)
		{
			// Load the previous build state if performing an incremental build
			auto targetDirectory = _workingDirectory + outputDirectory;
			if (!forceBuild)
			{
				Log::Diag("Loading previous build state");
				if (!OperationGraphManager::TryLoadState(
					targetDirectory,
					_previousOperationGraph,
					_fileSystemState.GetId()))
				{
					Log::Info("No previous operation state found, full rebuild required");
					_previousOperationGraph = OperationGraph(_fileSystemState.GetId());
					forceBuild = true;
				}
			}

			// Build the initial operation dependency set to
			// ensure operations are built in the correct order 
			// and that there are no cycles
			auto emptyParentSet = std::set<uint64_t>();
			BuildDependencies(operations, emptyParentSet);

			// Run all build operations in the correct order with incremental build checks
			CheckExecuteOperations(operations, forceBuild);

			Log::Info("Saving updated build state");
			OperationGraphManager::SaveState(targetDirectory, _activeOperationGraph);

			Log::HighPriority("Done");
		}

	private:
		/// <summary>
		/// Build dependencies
		/// </summary>
		void BuildDependencies(
			Utilities::BuildOperationListWrapper& operations,
			const std::set<uint64_t>& parentSet)
		{
			for (auto i = 0; i < operations.GetSize(); i++)
			{
				// Make sure there are no cycles using the address as a unique id
				auto operation = operations.GetValueAt(i);
				auto operationId = reinterpret_cast<uint64_t>(operation.GetRaw());
				if (parentSet.contains(operationId))
					throw std::runtime_error("A build operation graph must be acyclic.");
				
				// Check if the operation was already a child from a different path
				auto currentOperationSearch = _dependencyCounts.find(operationId);
				if (currentOperationSearch != _dependencyCounts.end())
				{
					// Increment the dependency count
					currentOperationSearch->second++;
				}
				else
				{
					// Insert a new entry with a single count
					_dependencyCounts.emplace(operationId, 1);
					
					// Recurse to children only the first time we see an operation
					auto updatedParentSet = parentSet;
					updatedParentSet.insert(operationId);
					BuildDependencies(operation.GetChildList(), updatedParentSet);
				}
			}
		}

		/// <summary>
		/// Execute the collection of build operations
		/// </summary>
		void CheckExecuteOperations(
			Utilities::BuildOperationListWrapper& operations,
			bool forceBuild)
		{
			for (auto i = 0; i < operations.GetSize(); i++)
			{
				// Check if the operation was already a child from a different path
				// Make sure there are no cycles using the address as a unique id
				auto operation = operations.GetValueAt(i);
				auto operationId = reinterpret_cast<uint64_t>(operation.GetRaw());
				auto currentOperationSearch = _dependencyCounts.find(operationId);
				if (currentOperationSearch != _dependencyCounts.end())
				{
					auto remainingCount = --currentOperationSearch->second;
					if (remainingCount == 0)
					{
						ExecuteOperation(operation, forceBuild);
					}
					else
					{
						// This operation will be executed from a different path
					}
				}
				else
				{
					throw std::runtime_error("A operation id was missing from the dependency collection.");
				}
			}
		}

		/// <summary>
		/// Execute a single build operation and all of its children
		/// </summary>
		void ExecuteOperation(
			Utilities::BuildOperationWrapper& operation,
			bool forceBuild)
		{
			// Build up the operation unique command
			auto command = CommandInfo(
				Path(operation.GetWorkingDirectory()),
				Path(operation.GetExecutable()),
				std::string(operation.GetArguments()));

			bool buildRequired = forceBuild;
			if (!forceBuild)
			{
				// Check if each source file is out of date and requires a rebuild
				Log::Diag("Check for updated source");

				// Check if this operation is in the build operation graph
				const OperationInfo* operationInfo;
				if (_previousOperationGraph.TryFindOperationInfo(command, operationInfo))
				{
					// Check if any of the input files have changed since last build
					if (_stateChecker.IsOutdated(
						operationInfo->Output,
						operationInfo->Input))
					{
						buildRequired = true;
					}
					else
					{
						Log::Info("Up to date");

						// Move over the completed operation information to the new graph since nothing has changed
						_activeOperationGraph.AddOperationInfo(*operationInfo);
					}
				}
				else
				{
					// The build command has not been run before
					Log::Info("Unknown operation");
					buildRequired = true;
				}
			}

			if (buildRequired)
			{
				Log::HighPriority(operation.GetTitle());
				auto message = "Execute: " + command.Executable.ToString() + " " + command.Arguments;
				Log::Diag(message);

				auto callback = std::make_shared<SystemAccessTracker>();
				auto process = Monitor::IDetourProcessManager::Current().CreateDetourProcess(
					command.Executable,
					command.Arguments,
					command.WorkingDirectory,
					callback);

				process->Start();
				process->WaitForExit();

				auto stdOut = process->GetStandardOutput();
				auto stdErr = process->GetStandardError();
				auto exitCode = process->GetExitCode();

				// Check the result of the monitor
				callback->VerifyResult();

				// Retrieve the input/output files
				// TODO: Verify operation output matches input
				auto runtimeInput = callback->GetInput();
				for (auto& value : operation.GetInputFileList().CopyAsStringVector())
				{
					runtimeInput.insert(value);
				}

				auto input = std::vector<Path>();
				for (auto& value : runtimeInput)
				{
					input.push_back(Path(value));
				}

				auto runtimeOutput = callback->GetOutput();
				for (auto& value : operation.GetOutputFileList().CopyAsStringVector())
				{
					runtimeOutput.insert(value);
				}

				auto output = std::vector<Path>();
				for (auto& value : runtimeOutput)
				{
					output.push_back(Path(value));
				}

				// Save off the build graph for future builds
				auto operationInfo = OperationInfo(
					command,
					_fileSystemState.ToFileIds(input, command.WorkingDirectory),
					_fileSystemState.ToFileIds(output, command.WorkingDirectory));

				// Ensure the File System State is notified of any output files that have changed
				_fileSystemState.CheckFileWriteTimes(operationInfo.Output);

				_activeOperationGraph.AddOperationInfo(std::move(operationInfo));

				if (!stdOut.empty())
				{
					// Upgrade output to a warning if the command fails
					if (exitCode != 0)
						Log::Warning(stdOut);
					else
						Log::Info(stdOut);
				}

				// If there was any error output then the build failed
				// TODO: Find warnings + errors
				if (!stdErr.empty())
				{
					Log::Error(stdErr);
				}

				if (exitCode != 0)
				{
					throw std::runtime_error("Compiler Object Error: " + std::to_string(exitCode));
				}
			}
			else
			{
				Log::Info(operation.GetTitle());
			}

			// Recursively build all of the operation children
			// Note: Force build if this operation was built
			CheckExecuteOperations(operation.GetChildList(), buildRequired);
		}

	private:
		Path _workingDirectory;
		FileSystemState& _fileSystemState;
		std::map<int64_t, int64_t> _dependencyCounts;
		OperationGraph _previousOperationGraph;
		OperationGraph _activeOperationGraph;
		BuildHistoryChecker _stateChecker;
	};
}
