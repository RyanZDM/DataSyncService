﻿using EBoard.Common;
using System;
using System.Windows.Forms;

namespace EBoard.SysManager
{
	internal static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);

			var loginForm = new LoginDlg();
			if (loginForm.ShowDialog() != DialogResult.OK)
				return;

			Application.Run(new MainFrom { CurrentUser = loginForm.CurrentUser });
		}
	}
}
