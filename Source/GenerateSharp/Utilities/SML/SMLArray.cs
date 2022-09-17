﻿// <copyright file="SMLArray.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities
{
	public class SMLArray : IEquatable<SMLArray>
	{
		public List<SMLValue> Values { get; set; }

		public SMLArray()
		{
			Values = new List<SMLValue>();
		}

		public SMLArray(List<SMLValue> values)
		{
			Values = values;
		}

		public override bool Equals(object? obj) => this.Equals(obj as SMLArray);

		public bool Equals(SMLArray? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return Enumerable.SequenceEqual(this.Values, rhs.Values);
		}

		public override int GetHashCode() => (Values).GetHashCode();

		public static bool operator ==(SMLArray? lhs, SMLArray? rhs)
		{
			if (lhs is null)
			{
				if (rhs is null)
					return true;
				else
					return false;
			}

			return lhs.Equals(rhs);
		}

		public static bool operator !=(SMLArray? lhs, SMLArray? rhs) => !(lhs == rhs);
	}
}
