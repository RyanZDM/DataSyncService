using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

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
			// TODO:
			var frm = new UserMgrForm();
			frm.MdiParent = this;
			frm.Show();
			//frm.Dock = DockStyle.Fill;
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
