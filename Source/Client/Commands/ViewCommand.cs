﻿// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using Newtonsoft.Json;
using Soup.Api;
using System.Net.Http;
using System.Threading.Tasks;

namespace Soup.Client
{
	internal class ViewCommand : ICommand
	{
		public string Name => "view";

		/// <summary>
		/// Invoke the view command
		/// </summary>
		public async Task InvokeAsync(string[] args)
		{
			if (args.Length < 2)
			{
				ShowUsage();
				return;
			}

			var packageName = args[1];
			try
			{
				var package = await Singleton<ISoupApi>.Instance.GetPackageAsync(packageName);
				var output = JsonConvert.SerializeObject(package);
				Log.Message(output);
			}
			catch (HttpRequestException ex)
			{
				Log.Error(ex.ToString());
			}
		}

		/// <summary>
		/// Show the usage details for this command
		/// </summary>
		private static void ShowUsage()
		{
			Log.Message("");
			Log.Message("Usage: soup view <package>");
			Log.Message("\tpackage: The unique package name.");
		}
	}
}
