// <copyright file="RecipeTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
    class RecipeTests
    {
    public:
        [[Fact]]
        void InitializerDefault()
        {
            auto uut = Recipe();

            Assert::IsFalse(uut.IsDirty(), "Verify is not dirty.");
            Assert::AreEqual("", uut.GetName(), "Verify name matches expected.");
            Assert::AreEqual(SemanticVersion(0, 0, 0), uut.GetVersion(), "Verify version is correct.");
            Assert::IsFalse(uut.HasType(), "Verify has no type.");
            Assert::AreEqual(RecipeType::Library, uut.GetType(), "Verify default type is correct.");
            Assert::IsFalse(uut.HasDependencies(), "Verify has no dependencies.");
            Assert::IsFalse(uut.HasPublic(), "Verify has no public.");
            Assert::IsFalse(uut.HasSource(), "Verify has no source.");
        }

        [[Fact]]
        void InitializerAll()
        {
            auto uut = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3),
                RecipeType::Executable,
                std::vector<PackageReference>({
                    PackageReference("../OtherPackage"),
                }),
                "Main.cpp",
                std::vector<std::string>({
                    "SomeFile.cpp",
                }));

            Assert::IsFalse(uut.IsDirty(), "Verify is not dirty.");
            Assert::AreEqual("MyPackage", uut.GetName(), "Verify name matches expected.");
            Assert::AreEqual(SemanticVersion(1, 2, 3), uut.GetVersion(), "Verify version is correct.");
            Assert::IsTrue(uut.HasType(), "Verify has type.");
            Assert::AreEqual(RecipeType::Executable, uut.GetType(), "Verify type is correct.");
            Assert::IsTrue(uut.HasDependencies(), "Verify has dependencies.");
            Assert::AreEqual(
                std::vector<PackageReference>({
                    PackageReference("../OtherPackage"),
                }),
                uut.GetDependencies(),
                "Verify type is correct.");
            Assert::IsTrue(uut.HasPublic(), "Verify has public.");
            Assert::AreEqual("Main.cpp", uut.GetPublic(), "Verify public is correct.");
            Assert::IsTrue(uut.HasSource(), "Verify has source.");
            Assert::AreEqual(
                std::vector<std::string>({
                    "SomeFile.cpp",
                }),
                uut.GetSource(),
                "Verify source is correct.");
        }

        [[Fact]]
        void OperatorEqualDefault()
        {
            auto uut = Recipe();

            Assert::AreEqual(Recipe(), uut, "Verify equal.");
        }

        [[Fact]]
        void OperatorEqualAll()
        {
            auto uut = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3),
                RecipeType::Executable,
                std::vector<PackageReference>({
                    PackageReference("../OtherPackage"),
                }),
                "Main.cpp",
                std::vector<std::string>({
                    "SomeFile.cpp",
                }));

            Assert::AreEqual(
                Recipe(
                    "MyPackage",
                    SemanticVersion(1, 2, 3),
                    RecipeType::Executable,
                    std::vector<PackageReference>({
                        PackageReference("../OtherPackage"),
                    }),
                    "Main.cpp",
                    std::vector<std::string>({
                        "SomeFile.cpp",
                    })),
                uut,
                "Verify equal.");
        }

        [[Fact]]
        void OperatorNotEqualName()
        {
            auto uut = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3),
                RecipeType::Executable,
                std::vector<PackageReference>({
                    PackageReference("../OtherPackage"),
                }),
                "Main.cpp",
                std::vector<std::string>({
                    "SomeFile.cpp",
                }));

            Assert::AreNotEqual(
                Recipe(
                    "MyPackage2",
                    SemanticVersion(1, 2, 3),
                    RecipeType::Executable,
                    std::vector<PackageReference>({
                        PackageReference("../OtherPackage"),
                    }),
                    "Main.cpp",
                    std::vector<std::string>({
                        "SomeFile.cpp",
                    })),
                uut,
                "Verify are not equal.");
        }

        [[Fact]]
        void OperatorNotEqualVersion()
        {
            auto uut = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3),
                RecipeType::Executable,
                std::vector<PackageReference>({
                    PackageReference("../OtherPackage"),
                }),
                "Main.cpp",
                std::vector<std::string>({
                    "SomeFile.cpp",
                }));

            Assert::AreNotEqual(
                Recipe(
                    "MyPackage",
                    SemanticVersion(11, 2, 3),
                    RecipeType::Executable,
                    std::vector<PackageReference>({
                        PackageReference("../OtherPackage"),
                    }),
                    "Main.cpp",
                    std::vector<std::string>({
                        "SomeFile.cpp",
                    })),
                uut,
                "Verify are not equal.");
        }

        [[Fact]]
        void OperatorNotEqualType()
        {
            auto uut = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3),
                RecipeType::Executable,
                std::vector<PackageReference>({
                    PackageReference("../OtherPackage"),
                }),
                "Main.cpp",
                std::vector<std::string>({
                    "SomeFile.cpp",
                }));

            Assert::AreNotEqual(
                Recipe(
                    "MyPackage",
                    SemanticVersion(1, 2, 3),
                    RecipeType::Library,
                    std::vector<PackageReference>({
                        PackageReference("../OtherPackage"),
                    }),
                    "Main.cpp",
                    std::vector<std::string>({
                        "SomeFile.cpp",
                    })),
                uut,
                "Verify are not equal.");
        }

        [[Fact]]
        void OperatorNotEqualNoType()
        {
            auto uut = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3),
                RecipeType::Executable,
                std::vector<PackageReference>({
                    PackageReference("../OtherPackage"),
                }),
                "Main.cpp",
                std::vector<std::string>({
                    "SomeFile.cpp",
                }));

            Assert::AreNotEqual(
                Recipe(
                    "MyPackage",
                    SemanticVersion(1, 2, 3),
                    std::nullopt,
                    std::vector<PackageReference>({
                        PackageReference("../OtherPackage"),
                    }),
                    "Main.cpp",
                    std::vector<std::string>({
                        "SomeFile.cpp",
                    })),
                uut,
                "Verify are not equal.");
        }

        [[Fact]]
        void OperatorNotEqualDependencies()
        {
            auto uut = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3),
                RecipeType::Executable,
                std::vector<PackageReference>({
                    PackageReference("../OtherPackage"),
                }),
                "Main.cpp",
                std::vector<std::string>({
                    "SomeFile.cpp",
                }));

            Assert::AreNotEqual(
                Recipe(
                    "MyPackage",
                    SemanticVersion(1, 2, 3),
                    RecipeType::Executable,
                    std::vector<PackageReference>({
                    }),
                    "Main.cpp",
                    std::vector<std::string>({
                        "SomeFile.cpp",
                    })),
                uut,
                "Verify are not equal.");
        }

        [[Fact]]
        void OperatorNotEqualNoDependencies()
        {
            auto uut = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3),
                RecipeType::Executable,
                std::vector<PackageReference>({
                    PackageReference("../OtherPackage"),
                }),
                "Main.cpp",
                std::vector<std::string>({
                    "SomeFile.cpp",
                }));

            Assert::AreNotEqual(
                Recipe(
                    "MyPackage",
                    SemanticVersion(1, 2, 3),
                    RecipeType::Executable,
                    std::nullopt,
                    "Main.cpp",
                    std::vector<std::string>({
                        "SomeFile.cpp",
                    })),
                uut,
                "Verify are not equal.");
        }

        [[Fact]]
        void OperatorNotEqualPublic()
        {
            auto uut = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3),
                RecipeType::Executable,
                std::vector<PackageReference>({
                    PackageReference("../OtherPackage"),
                }),
                "Main.cpp",
                std::vector<std::string>({
                    "SomeFile.cpp",
                }));

            Assert::AreNotEqual(
                Recipe(
                    "MyPackage",
                    SemanticVersion(1, 2, 3),
                    RecipeType::Executable,
                    std::vector<PackageReference>({
                        PackageReference("../OtherPackage"),
                    }),
                    "Main2.cpp",
                    std::vector<std::string>({
                        "SomeFile.cpp",
                    })),
                uut,
                "Verify are not equal.");
        }

        [[Fact]]
        void OperatorNotEqualNoPublic()
        {
            auto uut = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3),
                RecipeType::Executable,
                std::vector<PackageReference>({
                    PackageReference("../OtherPackage"),
                }),
                "Main.cpp",
                std::vector<std::string>({
                    "SomeFile.cpp",
                }));

            Assert::AreNotEqual(
                Recipe(
                    "MyPackage",
                    SemanticVersion(1, 2, 3),
                    RecipeType::Executable,
                    std::vector<PackageReference>({
                        PackageReference("../OtherPackage"),
                    }),
                    std::nullopt,
                    std::vector<std::string>({
                        "SomeFile.cpp",
                    })),
                uut,
                "Verify are not equal.");
        }

        [[Fact]]
        void OperatorNotEqualSource()
        {
            auto uut = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3),
                RecipeType::Executable,
                std::vector<PackageReference>({
                    PackageReference("../OtherPackage"),
                }),
                "Main.cpp",
                std::vector<std::string>({
                    "SomeFile.cpp",
                }));

            Assert::AreNotEqual(
                Recipe(
                    "MyPackage",
                    SemanticVersion(1, 2, 3),
                    RecipeType::Executable,
                    std::vector<PackageReference>({
                        PackageReference("../OtherPackage"),
                    }),
                    "Main.cpp",
                    std::vector<std::string>({
                    })),
                uut,
                "Verify are not equal.");
        }

        [[Fact]]
        void OperatorNotEqualNoSource()
        {
            auto uut = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3),
                RecipeType::Executable,
                std::vector<PackageReference>({
                    PackageReference("../OtherPackage"),
                }),
                "Main.cpp",
                std::vector<std::string>({
                    "SomeFile.cpp",
                }));

            Assert::AreNotEqual(
                Recipe(
                    "MyPackage",
                    SemanticVersion(1, 2, 3),
                    RecipeType::Executable,
                    std::vector<PackageReference>({
                        PackageReference("../OtherPackage"),
                    }),
                    "Main.cpp",
                    std::nullopt),
                uut,
                "Verify are not equal.");
        }
    };
}
