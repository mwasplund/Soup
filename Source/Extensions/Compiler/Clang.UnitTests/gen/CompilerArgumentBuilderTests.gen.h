#pragma once
#include "CompilerArgumentBuilderTests.h"

TestState RunCompilerArgumentBuilderTests() 
 {
	auto className = "CompilerArgumentBuilderTests";
	auto testClass = std::make_shared<Soup::Compiler::Clang::UnitTests::CompilerArgumentBuilderTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "EmptySourceFile_Throws", [&testClass]() { testClass->EmptySourceFile_Throws(); });
	state += Soup::Test::RunTest(className, "EmptyTargetFile_Throws", [&testClass]() { testClass->EmptyTargetFile_Throws(); });
	state += Soup::Test::RunTest(className, "DefaultParameters", [&testClass]() { testClass->DefaultParameters(); });
	state += Soup::Test::RunTest(className, "SingleArgument_LanguageStandard(Soup::LanguageStandard::CPP11, \"-std=c++11\")", [&testClass]() { testClass->SingleArgument_LanguageStandard(Soup::Compiler::LanguageStandard::CPP11, "-std=c++11"); });
	state += Soup::Test::RunTest(className, "SingleArgument_LanguageStandard(Soup::LanguageStandard::CPP14, \"-std=c++14\")", [&testClass]() { testClass->SingleArgument_LanguageStandard(Soup::Compiler::LanguageStandard::CPP14, "-std=c++14"); });
	state += Soup::Test::RunTest(className, "SingleArgument_LanguageStandard(Soup::LanguageStandard::CPP17, \"-std=c++17\")", [&testClass]() { testClass->SingleArgument_LanguageStandard(Soup::Compiler::LanguageStandard::CPP17, "-std=c++17"); });
	state += Soup::Test::RunTest(className, "SingleArgument_LanguageStandard_CPP20", [&testClass]() { testClass->SingleArgument_LanguageStandard_CPP20(); });
	state += Soup::Test::RunTest(className, "SingleArgument_OptimizationLevel_Disabled", [&testClass]() { testClass->SingleArgument_OptimizationLevel_Disabled(); });
	state += Soup::Test::RunTest(className, "SingleArgument_OptimizationLevel(Soup::OptimizationLevel::Size, \"-Oz\")", [&testClass]() { testClass->SingleArgument_OptimizationLevel(Soup::Compiler::OptimizationLevel::Size, "-Oz"); });
	state += Soup::Test::RunTest(className, "SingleArgument_OptimizationLevel(Soup::OptimizationLevel::Speed, \"-O3\")", [&testClass]() { testClass->SingleArgument_OptimizationLevel(Soup::Compiler::OptimizationLevel::Speed, "-O3"); });
	state += Soup::Test::RunTest(className, "SingleArgument_GenerateDebugInformation", [&testClass]() { testClass->SingleArgument_GenerateDebugInformation(); });
	state += Soup::Test::RunTest(className, "SingleArgument_IncludePaths", [&testClass]() { testClass->SingleArgument_IncludePaths(); });
	state += Soup::Test::RunTest(className, "SingleArgument_PreprocessorDefinitions", [&testClass]() { testClass->SingleArgument_PreprocessorDefinitions(); });
	state += Soup::Test::RunTest(className, "SingleArgument_Modules", [&testClass]() { testClass->SingleArgument_Modules(); });
	state += Soup::Test::RunTest(className, "SingleArgument_ExportModule_SingleSource", [&testClass]() { testClass->SingleArgument_ExportModule_SingleSource(); });

	return state;
}