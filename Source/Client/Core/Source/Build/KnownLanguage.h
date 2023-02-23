﻿// <copyright file="KnownLanguage.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	#ifdef SOUP_BUILD
	export
	#endif
	struct KnownLanguage
	{
		KnownLanguage(
			std::string languageSafeName,
			std::string extensionName) :
			LanguageSafeName(std::move(languageSafeName)),
			ExtensionName(std::move(extensionName))
		{
		}

		std::string LanguageSafeName;
		std::string ExtensionName;
	};
}
