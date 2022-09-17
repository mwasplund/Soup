﻿// <copyright file="Recipe.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using System.Net.Mail;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The recipe container
	/// </summary>
	public class Recipe
	{
		private static string Property_Dependencies => "Dependencies";
		private static string Property_Runtime => "Runtime";
		private static string Property_Build => "Build";
		private static string Property_Test => "Test";
		private static string Property_Language => "Language";
		private static string Property_Name => "Name";
		private static string Property_Version => "Version";
		private static string Property_Reference => "Reference";

		private SMLDocument _document;

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		public Recipe()
		{
			_document = new SMLDocument();
			Name = string.Empty;
			Language = new LanguageReference();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		public Recipe(
			string name,
			LanguageReference language)
		{
			_document = new SMLDocument();
			Name = name;
			Language = language;
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		public Recipe(SMLDocument document)
		{
			_document = document;

			if (!HasValue(_document, Property_Name))
				throw new ArgumentException("Missing required property Name");
			if (!HasValue(_document, Property_Language))
				throw new ArgumentException("Missing required property Language");
		}

		/// <summary>
		/// Gets or sets the package name
		/// </summary>
		public SMLValue NameValue => GetValue(_document, Property_Name);

		public string Name
		{
			get { return NameValue.AsString(); }
			set { EnsureValue(_document, Property_Name, new SMLValue(value)); }
		}

		/// <summary>
		/// Gets or sets the package language
		/// </summary>
		public SMLValue LanguageValue => GetValue(_document, Property_Language);

		public LanguageReference Language
		{
			get { return LanguageReference.Parse(LanguageValue.AsString()); }
			set { EnsureValue(_document, Property_Language, new SMLValue(value.ToString())); }
		}

		/// <summary>
		/// Gets or sets the package version
		/// </summary>
		public bool HasVersion => HasValue(_document, Property_Version);

		public SemanticVersion Version
		{
			get
			{
				if (!HasVersion)
					throw new InvalidOperationException("No version.");

				return SemanticVersion.Parse(
					GetValue(_document, Property_Version).AsString());
			}
			set
			{
				EnsureValue(_document, Property_Version, new SMLValue(value.ToString()));
			}
		}

		/// <summary>
		/// Gets or sets the list of named dependency packages
		/// </summary>
		public IList<string> GetDependencyTypes()
		{
			var result = new List<string>();
			if (HasDependencies())
			{
				foreach (var value in GetDependencies().Values)
				{
					result.Add(value.Key);
				}
			}

			return result;
		}

		public bool HasNamedDependencies(string name)
		{
			return HasDependencies() && HasValue(GetDependencies(), name);
		}

		public IList<PackageReference> GetNamedDependencies(string name)
		{
			if (!HasNamedDependencies(name))
				throw new InvalidOperationException("No named dependencies.");

			var values = GetValue(GetDependencies(), name).AsArray();
			var result = new List<PackageReference>();
			foreach (var value in values.Values)
			{
				// A dependency can either be a string or a table with reference key
				if (value.Type == SMLValueType.String)
				{
					result.Add(PackageReference.Parse(value.AsString()));
				}
				else if (value.Type == SMLValueType.Table)
				{
					var valueTable = value.AsTable();
					if (!HasValue(valueTable, Property_Reference))
						throw new InvalidOperationException("Recipe dependency table missing required Reference value.");
					var referenceValue = GetValue(valueTable, Property_Reference);
					if (referenceValue.Type == SMLValueType.String)
					{
						result.Add(PackageReference.Parse(referenceValue.AsString()));
					}
					else
					{
						throw new InvalidOperationException("Recipe dependency Reference must be type String.");
					}
				}
				else
				{
					throw new InvalidOperationException("Unknown Recipe dependency type.");
				}
			}

			return result;
		}

		public void AddRuntimeDependency(string value)
		{
			var dependencies = EnsureHasTable(_document, Property_Dependencies);
			var runtimeDependencies = EnsureHasList(dependencies, Property_Runtime);

			runtimeDependencies.AddItemWithSyntax(value);
		}

		/// <summary>
		/// Gets or sets the list of runtime dependency packages
		/// </summary>
		public bool HasRuntimeDependencies => HasNamedDependencies(Property_Runtime);

		public IList<PackageReference> RuntimeDependencies
		{
			get { return GetNamedDependencies(Property_Runtime); }
		}

		/// <summary>
		/// Gets or sets the list of build dependency packages
		/// </summary>
		public bool HasBuildDependencies => HasNamedDependencies(Property_Build);

		public IList<PackageReference> BuildDependencies
		{
			get { return GetNamedDependencies(Property_Build); }
		}

		/// <summary>
		/// Gets or sets the list of test dependency packages
		/// </summary>
		public bool HasTestDependencies => HasNamedDependencies(Property_Test);

		public IList<PackageReference> TestDependencies
		{
			get { return GetNamedDependencies(Property_Test); }
		}

		/// <summary>
		/// Raw access
		/// </summary>
		public SMLDocument Document => _document;

		/// <summary>
		/// Gets or sets the table of dependency packages
		/// </summary>
		private bool HasDependencies()
		{
			return HasValue(_document, Property_Dependencies);
		}

		private SMLTable GetDependencies()
		{
			if (!HasDependencies())
				throw new InvalidOperationException("No dependencies.");

			var values = GetValue(_document, Property_Dependencies).AsTable();
			return values;
		}

		private static SMLTable EnsureHasTable(SMLDocument document, string name)
		{
			if (document.Values.ContainsKey(name))
			{
				var value = document.Values[name];
				if (value.Value.Type != SMLValueType.Table)
					throw new InvalidOperationException("The recipe already has a non-table dependencies property");

				// Find the Syntax for the table
				return value.Value.AsTable();
			}
			else
			{
				// Create a new table
				return document.AddTableWithSyntax(name);
			}
		}

		private static SMLArray EnsureHasList(SMLTable table, string name)
		{
			if (table.Values.ContainsKey(name))
			{
				var value = table.Values[name];
				if (value.Value.Type != SMLValueType.Array)
					throw new InvalidOperationException("The recipe already has a non-list dependencies property");

				// Find the Syntax for the table
				return value.Value.AsArray();
			}
			else
			{
				// Create a new list
				return table.AddArrayWithSyntax(name);
			}
		}

		private static SMLArray EnsureHasList(SMLDocument document, string name)
		{
			if (document.Values.ContainsKey(name))
			{
				var value = document.Values[name];
				if (value.Value.Type != SMLValueType.Array)
					throw new InvalidOperationException("The recipe already has a non-list dependencies property");

				// Find the Syntax for the table
				return value.Value.AsArray();
			}
			else
			{
				// Create a new list
				return document.AddArrayWithSyntax(name);
			}
		}

		private static bool HasValue(SMLTable table, string key)
		{
			return table.Values.ContainsKey(key);
		}

		private static bool HasValue(SMLDocument document, string key)
		{
			return document.Values.ContainsKey(key);
		}

		private static SMLValue GetValue(SMLTable table, string key)
		{
			if (table.Values.TryGetValue(key, out var value))
			{
				return value.Value;
			}
			else
			{
				throw new InvalidOperationException("Requested recipe value does not exist in the table.");
			}
		}

		private static SMLValue GetValue(SMLDocument document, string key)
		{
			if (document.Values.TryGetValue(key, out var value))
			{
				return value.Value;
			}
			else
			{
				throw new InvalidOperationException("Requested recipe value does not exist in the table.");
			}
		}

		private static SMLValue EnsureValue(SMLDocument document, string key, SMLValue value)
		{
			if (document.Values.ContainsKey(key))
			{
				document.Values.Add(key, new SMLTableValue(value));
			}
			else
			{
				document.Values.Add(key, new SMLTableValue(value));
			}

			return value;
		}

		private static SMLValue EnsureValue(SMLTable table, string key, SMLValue value)
		{
			if (table.Values.ContainsKey(key))
			{
				table.Values.Add(key, new SMLTableValue(value));
			}
			else
			{
				table.Values.Add(key, new SMLTableValue(value));
			}

			return value;
		}
	}
}
