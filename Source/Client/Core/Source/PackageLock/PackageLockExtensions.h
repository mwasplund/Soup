﻿// <copyright file="PackageLockExtensions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "PackageLock.h"
#include "Recipe/RecipeSML.h"

namespace Soup::Core
{
	/// <summary>
	/// The package lock extensions
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class PackageLockExtensions
	{
	public:
		/// <summary>
		/// Attempt to load from file
		/// </summary>
		static bool TryLoadFromFile(
			const Path& packageLockFile,
			PackageLock& result)
		{
			// Verify the requested file exists
			Log::Diag("Load PackageLock: " + packageLockFile.ToString());
			if (!System::IFileSystem::Current().Exists(packageLockFile))
			{
				Log::Info("PackageLock file does not exist");
				return false;
			}

			// Open the file to read from
			auto file = System::IFileSystem::Current().OpenRead(packageLockFile, true);

			// Read the contents of the recipe file
			try
			{
				result = PackageLock(
					RecipeSML::Deserialize(
						packageLockFile,
						file->GetInStream()));
				return true;
			}
			catch (std::exception& ex)
			{
				Log::Error(std::string("Deserialize Threw: ") + ex.what());
				Log::Info("Failed to parse PackageLock.");
				return false;
			}
		}
	};
}
