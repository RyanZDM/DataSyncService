using EBoard.Common;
using System;
using System.Windows.Forms;

namespace EBoard
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);

			var loginForm = new Login();
			if (loginForm.ShowDialog() != DialogResult.OK)
			{
				return;
			}

			// There is only one user mapped to current shift.
			// Automatically map current logged on user with current shift if no mapping yet
			// Ask user if continue log on if there is already user mapped to current shift.


			Application.Run(new MainForm { CurrentUser = loginForm.CurrentUser });
		}
	}
}
