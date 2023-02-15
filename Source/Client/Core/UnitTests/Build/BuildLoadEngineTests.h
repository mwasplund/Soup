// <copyright file="BuildLoadEngineTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class BuildLoadEngineTests
	{
	public:
		// [[Fact]]
		void Initialize()
		{
			auto builtInLanguages = std::map<std::string, BuiltInLanguagePackage>();
			auto arguments = RecipeBuildArguments();
			auto hostBuildGlobalParameters = ValueTable();
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInLanguages,
				arguments,
				hostBuildGlobalParameters,
				recipeCache);
		}

		// [[Fact]]
		void Load_LanguageExtension_BuiltInVersion()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1.1.1"
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto builtInLanguages = std::map<std::string, BuiltInLanguagePackage>(
			{
				{
					"C++",
					 BuiltInLanguagePackage(
						"Cpp",
						"Soup.Cpp",
						SemanticVersion(1, 1, 1))
				},
				{
					"C#",
					BuiltInLanguagePackage(
						"CSharp",
						"Soup.CSharp",
						SemanticVersion(2, 2, 2))
				},
			});
			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInLanguages,
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{ "ArgumentValue", Value(true) },
								}))
						},
						{
							2,
							PackageGraph(
								2,
								2,
								ValueTable())
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								"MyPackage",
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								"Soup.Cpp",
								true,
								Path("C:/testlocation/BuiltIn/Soup.Cpp/1.1.1/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_LanguageExtension_ExternalLanguage_ExactMatch()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|4.5.6"
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto builtInLanguages = std::map<std::string, BuiltInLanguagePackage>(
			{
				{
					"C++",
					 BuiltInLanguagePackage(
						"Cpp",
						"Soup.Cpp",
						SemanticVersion(1, 1, 1))
				},
				{
					"C#",
					BuiltInLanguagePackage(
						"CSharp",
						"Soup.CSharp",
						SemanticVersion(2, 2, 2))
				},
			});
			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInLanguages,
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{ "ArgumentValue", Value(true) },
								}))
						},
						{
							2,
							PackageGraph(
								2,
								2,
								ValueTable())
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								"MyPackage",
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								"Soup.Cpp",
								true,
								Path("C:/testlocation/BuiltIn/Soup.Cpp/1.1.1/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_TriangleDependency_NoRebuild()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Dependencies: {
						Runtime: [
							"PackageA@3.3.3"
							"PackageB@4.4.4"
						]
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "PackageA"
					Language: "C++|1"
					Dependencies: {
						Runtime: [
							"PackageB@4.4.4"
						]
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "PackageB"
					Language: "C++|1"
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto builtInLanguages = std::map<std::string, BuiltInLanguagePackage>(
			{
				{
					"C++",
					 BuiltInLanguagePackage(
						"Cpp",
						"Soup.Cpp",
						SemanticVersion(1, 1, 1))
				},
				{
					"C#",
					BuiltInLanguagePackage(
						"CSharp",
						"Soup.CSharp",
						SemanticVersion(2, 2, 2))
				},
			});
			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInLanguages,
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml",
					"DIAG: Graph already loaded: C:/testlocation/BuiltIn/Soup.Cpp/1.1.1/",
					"DIAG: Recipe already loaded: C++|PackageB",
					"DIAG: Graph already loaded: C:/testlocation/BuiltIn/Soup.Cpp/1.1.1/",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml",
					"GetCurrentDirectory",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{ "ArgumentValue", Value(true) },
								}))
						},
						{
							2,
							PackageGraph(
								2,
								4,
								ValueTable())
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								"MyPackage",
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
									{
										"Runtime",
										{
											PackageChildInfo(PackageReference(std::nullopt, "PackageA", SemanticVersion(3, 3, 3)), false, 2, -1),
											PackageChildInfo(PackageReference(std::nullopt, "PackageB", SemanticVersion(4, 4, 4)), false, 3, -1),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								"PackageA",
								false,
								Path("C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/"),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
									{
										"Runtime",
										{
											PackageChildInfo(PackageReference(std::nullopt, "PackageB", SemanticVersion(4, 4, 4)), false, 3, -1),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								"PackageB",
								false,
								Path("C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/"),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
								}))
						},
						{
							4,
							PackageInfo(
								4,
								"Soup.Cpp",
								true,
								Path("C:/testlocation/BuiltIn/Soup.Cpp/1.1.1/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_BuildDependency_NoPackageLock_ExternalReference()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Dependencies: {
						Build: [
							"TestBuild@3.3.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "Wren|1"
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto builtInLanguages = std::map<std::string, BuiltInLanguagePackage>(
			{
				{
					"C++",
					 BuiltInLanguagePackage(
						"Cpp",
						"Soup.Cpp",
						SemanticVersion(1, 1, 1))
				},
				{
					"Wren",
					BuiltInLanguagePackage(
						"Wren",
						"Soup.Wren",
						SemanticVersion(2, 2, 2))
				},
			});
			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInLanguages,
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.3/PackageLock.sml",
					"INFO: PackageLock file does not exist",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.3/PackageLock.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{ "ArgumentValue", Value(true) },
								}))
						},
						{
							2,
							PackageGraph(
								2,
								3,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								2,
								ValueTable(
								{
									{ "HostValue", Value(true) },
								}))
						},
						{
							4,
							PackageGraph(
								4,
								4,
								ValueTable())
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								"MyPackage",
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "TestBuild", SemanticVersion(3, 3, 3)), true, -1, 3),
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 4),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								"TestBuild",
								false,
								Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/"),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Wren", SemanticVersion(2, 2, 2)), true, -1, 2),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								"Soup.Wren",
								true,
								Path("C:/testlocation/BuiltIn/Soup.Wren/2.2.2/"),
								nullptr,
								PackageChildrenMap())
						},
						{
							4,
							PackageInfo(
								4,
								"Soup.Cpp",
								true,
								Path("C:/testlocation/BuiltIn/Soup.Cpp/1.1.1/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_BuildDependency_NoPackageLock_Local()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Dependencies: {
						Build: [
							"../TestBuild/"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/TestBuild/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "Wren|1"
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto builtInLanguages = std::map<std::string, BuiltInLanguagePackage>(
			{
				{
					"C++",
					 BuiltInLanguagePackage(
						"Cpp",
						"Soup.Cpp",
						SemanticVersion(1, 1, 1))
				},
				{
					"Wren",
					BuiltInLanguagePackage(
						"Wren",
						"Soup.Wren",
						SemanticVersion(2, 2, 2))
				},
			});
			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInLanguages,
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/WorkingDirectory/TestBuild/Recipe.sml",
					"DIAG: Load PackageLock: C:/WorkingDirectory/TestBuild/PackageLock.sml",
					"INFO: PackageLock file does not exist",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/WorkingDirectory/TestBuild/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/TestBuild/Recipe.sml",
					"Exists: C:/WorkingDirectory/TestBuild/PackageLock.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{ "ArgumentValue", Value(true) },
								}))
						},
						{
							2,
							PackageGraph(
								2,
								3,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								2,
								ValueTable(
								{
									{ "HostValue", Value(true) },
								}))
						},
						{
							4,
							PackageGraph(
								4,
								4,
								ValueTable())
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								"MyPackage",
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(Path("../TestBuild/")), true, -1, 3),
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 4),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								"TestBuild",
								false,
								Path("C:/WorkingDirectory/TestBuild/"),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/TestBuild/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Wren", SemanticVersion(2, 2, 2)), true, -1, 2),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								"Soup.Wren",
								true,
								Path("C:/testlocation/BuiltIn/Soup.Wren/2.2.2/"),
								nullptr,
								PackageChildrenMap())
						},
						{
							4,
							PackageInfo(
								4,
								"Soup.Cpp",
								true,
								Path("C:/testlocation/BuiltIn/Soup.Cpp/1.1.1/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_BuildDependency_PackageLock()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Dependencies: {
						Build: [
							"TestBuild@3.3.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "Wren|1"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 4
					Closures: {
						Root: {
							"C++": [
								{ Name: "MyPackage", Version: "../MyPackage/", Build: "Build0" }
							]
						}
						Build0: {
							Wren: [
								{ Name: "Soup.Cpp", Version: "1.1.1" }
								{ Name: "TestBuild", Version: "3.3.3" }
							]
						}
					}
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto builtInLanguages = std::map<std::string, BuiltInLanguagePackage>(
			{
				{
					"C++",
					 BuiltInLanguagePackage(
						"Cpp",
						"Soup.Cpp",
						SemanticVersion(1, 1, 1))
				},
				{
					"Wren",
					BuiltInLanguagePackage(
						"Wren",
						"Soup.Wren",
						SemanticVersion(2, 2, 2))
				},
			});
			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInLanguages,
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.3/PackageLock.sml",
					"INFO: PackageLock file does not exist",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.3/PackageLock.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{
										"ArgumentValue",
										Value(true),
									},
								}))
						},
						{
							2,
							PackageGraph(
								2,
								3,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								2,
								ValueTable(
								{
									{
										"HostValue",
										Value(true),
									},
								}))
						},
						{
							4,
							PackageGraph(
								4,
								4,
								ValueTable())
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								"MyPackage",
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "TestBuild", SemanticVersion(3, 3, 3)), true, -1, 3),
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 4),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								"TestBuild",
								false,
								Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/"),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Wren", SemanticVersion(2, 2, 2)), true, -1, 2),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								"Soup.Wren",
								true,
								Path("C:/testlocation/BuiltIn/Soup.Wren/2.2.2/"),
								nullptr,
								PackageChildrenMap())
						},
						{
							4,
							PackageInfo(
								4,
								"Soup.Cpp",
								true,
								Path("C:/testlocation/BuiltIn/Soup.Cpp/1.1.1/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_BuildDependency_PackageLock_Override()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Dependencies: {
						Build: [
							"TestBuild@3.3.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.4/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "Wren|1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/Soup.Wren/2.2.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Wren"
					Language: "Wren|1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/Soup.Cpp/1.1.2/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "Wren|1"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 4
					Closures: {
						Root: {
							"C++": [
								{ Name: "MyPackage", Version: "../MyPackage/", Build: "Build0" }
							]
						}
						Build0: {
							Wren: [
								{ Name: "Soup.Cpp", Version: "1.1.2" }
								{ Name: "TestBuild", Version: "3.3.4" }
							]
						}
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.4/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 4
					Closures: {
						Root: {
							Wren: [
								{ Name: "TestBuild", Version: "../TestBuild/", Build: "Build0" }
							]
						}
						Build0: {
							Wren: [
								{ Name: "Soup.Wren", Version: "2.2.3" }
							]
						}
					}
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto builtInLanguages = std::map<std::string, BuiltInLanguagePackage>(
			{
				{
					"C++",
					 BuiltInLanguagePackage(
						"Cpp",
						"Soup.Cpp",
						SemanticVersion(1, 1, 1))
				},
				{
					"Wren",
					BuiltInLanguagePackage(
						"Wren",
						"Soup.Wren",
						SemanticVersion(2, 2, 2))
				},
			});
			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
			{
				{
					"ArgumentValue",
					Value(true),
				},
			});
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
			{
				{
					"HostValue",
					Value(true),
				},
			});
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInLanguages,
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.4/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.4/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/Soup.Wren/2.2.3/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/Soup.Wren/2.2.3/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/Soup.Cpp/1.1.2/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/Soup.Cpp/1.1.2/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Graph already loaded: C:/testlocation/BuiltIn/Soup.Wren/2.2.2/",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.4/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.4/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.4/PackageLock.sml",
					"OpenReadBinary: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.4/PackageLock.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/Wren/Soup.Wren/2.2.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Wren/Soup.Wren/2.2.3/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/locks/Wren/Soup.Wren/2.2.3/PackageLock.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/Wren/Soup.Cpp/1.1.2/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Wren/Soup.Cpp/1.1.2/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/locks/Wren/Soup.Cpp/1.1.2/PackageLock.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{
										"ArgumentValue",
										Value(true),
									},
								}))
						},
						{
							2,
							PackageGraph(
								2,
								4,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								3,
								ValueTable(
								{
									{
										"HostValue",
										Value(true),
									},
								}))
						},
						{
							4,
							PackageGraph(
								4,
								2,
								ValueTable(
								{
									{
										"HostValue",
										Value(true),
									},
								}))
						},
						{
							5,
							PackageGraph(
								5,
								5,
								ValueTable(
								{
									{
										"HostValue",
										Value(true),
									},
								}))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								"MyPackage",
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "TestBuild", SemanticVersion(3, 3, 3)), true, -1, 4),
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Cpp", SemanticVersion(1, 1, 2)), true, -1, 5),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								"TestBuild",
								false,
								Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.4/"),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.4/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Wren", SemanticVersion(2, 2, 3)), true, -1, 3),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								"Soup.Wren",
								false,
								Path("C:/Users/Me/.soup/packages/Wren/Soup.Wren/2.2.3/"),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Wren/Soup.Wren/2.2.3/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Wren", SemanticVersion(2, 2, 2)), true, -1, 2),
										}
									},
								}))
						},
						{
							4,
							PackageInfo(
								4,
								"Soup.Wren",
								true,
								Path("C:/testlocation/BuiltIn/Soup.Wren/2.2.2/"),
								nullptr,
								PackageChildrenMap())
						},
						{
							5,
							PackageInfo(
								5,
								"Soup.Cpp",
								false,
								Path("C:/Users/Me/.soup/packages/Wren/Soup.Cpp/1.1.2/"),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Wren/Soup.Cpp/1.1.2/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Wren", SemanticVersion(2, 2, 2)), true, -1, 2),
										}
									},
								}))
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_OtherDependency_PackageLock_ExplicitLanguage()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Dependencies: {
						Other: [
							"C#|Package1@4.4.4"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/CSharp/Package1/4.4.4/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Package1"
					Language: "C#|1"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 4
					Closures: {
						Root: {
							"C++": [
								{ Name: "MyPackage", Version: "../MyPackage/", Build: "Build0" }
							]
							"C#": [
								{ Name: "Package1", Version: "4.4.4", Build: "Build1" }
							]
						}
						Build0: {
							Wren: [
								{ Name: "Soup.Cpp", Version: "1.1.1" }
							]
						}
						Build1: {
							Wren: [
								{ Name: "Soup.CSharp", Version: "3.3.3" }
							]
						}
					}
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto builtInLanguages = std::map<std::string, BuiltInLanguagePackage>(
			{
				{
					"C++",
					 BuiltInLanguagePackage(
						"Cpp",
						"Soup.Cpp",
						SemanticVersion(1, 1, 1))
				},
				{
					"Wren",
					BuiltInLanguagePackage(
						"Wren",
						"Soup.Wren",
						SemanticVersion(2, 2, 2))
				},
				{
					"C#",
					BuiltInLanguagePackage(
						"CSharp",
						"Soup.CSharp",
						SemanticVersion(3, 3, 3))
				},
			});
			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
			{
				{
					"ArgumentValue",
					Value(true),
				},
			});
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
			{
				{
					"HostValue",
					Value(true),
				},
			});
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInLanguages,
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/CSharp/Package1/4.4.4/Recipe.sml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/CSharp/Package1/4.4.4/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/CSharp/Package1/4.4.4/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{
										"ArgumentValue",
										Value(true),
									},
								}))
						},
						{
							2,
							PackageGraph(
								2,
								3,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								4,
								ValueTable())
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								"MyPackage",
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 3),
										}
									},
									{
										"Other",
										{
											PackageChildInfo(PackageReference("C#", "Package1", SemanticVersion(4, 4, 4)), false, 2, -1),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								"Package1",
								false,
								Path("C:/Users/Me/.soup/packages/CSharp/Package1/4.4.4/"),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/CSharp/Package1/4.4.4/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.CSharp", SemanticVersion(3, 3, 3)), true, -1, 2),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								"Soup.CSharp",
								true,
								Path("C:/testlocation/BuiltIn/Soup.CSharp/3.3.3/"),
								nullptr,
								PackageChildrenMap())
						},
						{
							4,
							PackageInfo(
								4,
								"Soup.Cpp",
								true,
								Path("C:/testlocation/BuiltIn/Soup.Cpp/1.1.1/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_RuntimeDependency_PackageLock_ExplicitLanguageOverride_MultipleReferences_ReuseBuildGraph()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C#|1"
					Dependencies: {
						Runtime: [
							"../Package1/"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/Package1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Package1"
					Language: "C#|1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/Soup.CSharp/2.2.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.CSharp"
					Language: "Wren|1"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 4
					Closures: {
						Root: {
							"C#": [
								{ Name: "MyPackage", Version: "../MyPackage/", Build: "Build0" }
								{ Name: "Package1", Version: "../Package1/", Build: "Build0" }
							]
						}
						Build0: {
							Wren: [
								{ Name: "Soup.CSharp", Version: "2.2.3" }
							]
						}
					}
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto builtInLanguages = std::map<std::string, BuiltInLanguagePackage>(
			{
				{
					"Wren",
					BuiltInLanguagePackage(
						"Wren",
						"Soup.Wren",
						SemanticVersion(2, 2, 2))
				},
				{
					"C#",
					 BuiltInLanguagePackage(
						"CSharp",
						"Soup.CSharp",
						SemanticVersion(3, 3, 3))
				},
			});
			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
			{
				{
					"ArgumentValue",
					Value(true),
				},
			});
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
			{
				{
					"HostValue",
					Value(true),
				},
			});
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInLanguages,
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/WorkingDirectory/Package1/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/Soup.CSharp/2.2.3/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/Soup.CSharp/2.2.3/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Graph already loaded: C:/Users/Me/.soup/packages/Wren/Soup.CSharp/2.2.3/",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/WorkingDirectory/Package1/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/Package1/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/Wren/Soup.CSharp/2.2.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Wren/Soup.CSharp/2.2.3/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/locks/Wren/Soup.CSharp/2.2.3/PackageLock.sml",
					"GetCurrentDirectory",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{ "ArgumentValue", Value(true) },
								}))
						},
						{
							2,
							PackageGraph(
								2,
								4,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								3,
								ValueTable(
								{
									{ "HostValue", Value(true) },
								}))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								"MyPackage",
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.CSharp", SemanticVersion(2, 2, 3)), true, -1, 3),
										}
									},
									{
										"Runtime",
										{
											PackageChildInfo(PackageReference(Path("../Package1/")), false, 2, -1),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								"Package1",
								false,
								Path("C:/WorkingDirectory/Package1/"),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/Package1/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.CSharp", SemanticVersion(2, 2, 3)), true, -1, 3),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								"Soup.CSharp",
								false,
								Path("C:/Users/Me/.soup/packages/Wren/Soup.CSharp/2.2.3/"),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Wren/Soup.CSharp/2.2.3/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Wren", SemanticVersion(2, 2, 2)), true, -1, 2),
										}
									},
								}))
						},
						{
							4,
							PackageInfo(
								4,
								"Soup.Wren",
								true,
								Path("C:/testlocation/BuiltIn/Soup.Wren/2.2.2/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}
	};
}
