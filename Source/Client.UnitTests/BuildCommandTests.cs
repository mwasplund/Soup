// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using Soup.Api;
using Soup.TestUtils;
using System;
using System.IO;
using System.Threading.Tasks;
using Xunit;

namespace Soup.Client.UnitTests
{
	public class BuildCommandTests : IDisposable
	{
		public BuildCommandTests()
		{
			var cwd = Directory.GetCurrentDirectory();
			Singleton<ILogger>.Instance = new MockLogger();
			Singleton<LocalUserConfig>.Instance = new LocalUserConfig();
			Singleton<ISoupApi>.Instance = new MockSoupApi();
		}

		public void Dispose()
		{
			Singleton<LocalUserConfig>.Instance = null;
			Singleton<ISoupApi>.Instance = null;
		}

		[Fact]
		public void NameIsCorrect()
		{
			var uut = new BuildCommand();
			Assert.Equal("build", uut.Name);
		}
	}
}
