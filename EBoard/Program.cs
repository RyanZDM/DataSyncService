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
			
			Application.Run(new MainForm { CurrentUser = loginForm.CurrentUser });
		}
	}
}
