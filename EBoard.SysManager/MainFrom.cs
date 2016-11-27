using EBoard.Common;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Excel = Microsoft.Office.Interop.Excel;

namespace EBoard.SysManager
{
	public partial class MainFrom : Form
	{
		public MainFrom()
		{
			InitializeComponent();
			this.FormClosing += MainFrom_FormClosing;	
		}

		private void MainFrom_FormClosing(object sender, FormClosingEventArgs e)
		{
			foreach (var form in MdiChildren.OfType<FormBase>())
			{
				if (!form.CheckDirtyData())
				{
					e.Cancel = true;
					return;
				}
			}
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			Test();
			// TODO:
			OpenForm<DataMaintainForm>();
		}

		private void Test()
		{
			using (var app = new ExcelApp())
			{
				var workbook = app.CreateOrOpenExcel(@"e:\test.xls");
				var worksheet = app.CreateOrOpenWorksheet(workbook, "data");

				var sql = @"select * from generalParams";
				var connection = DbFactory.GetConnection();
				var adapter = new SqlDataAdapter(sql, connection);
				var ds = new DataSet();
				adapter.Fill(ds);
				app.WriteData(worksheet, ds.Tables[0], true, 2);
				workbook.Save();
			}
		}

		private void OpenForm<T>() where T : Form, new()
		{
			var form = MdiChildren.OfType<T>().FirstOrDefault() ?? new T();

			form.MdiParent = this;
			form.Show();
			form.Select();
		}

		#region Window position adjustment
		private void tileHorToolStripMenuItem_Click(object sender, EventArgs e)
		{
			LayoutMdi(MdiLayout.TileHorizontal);
		}

		private void tileVerToolStripMenuItem_Click(object sender, EventArgs e)
		{
			LayoutMdi(MdiLayout.TileVertical);
		}

		private void cascadeToolStripMenuItem_Click(object sender, EventArgs e)
		{
			LayoutMdi(MdiLayout.Cascade);
		}
		#endregion

		private void exitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Close();
			return;
		}
		
		#region Features
		private void userMgrToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenForm<UserMgrForm>();
		}
		
		private void dataMgrToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenForm<DataMaintainForm>();
		}

		#endregion

	}
}
