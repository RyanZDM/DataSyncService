using EBoard.Common;
using System;
using System.Data.SqlClient;
using System.Linq;
using System.Windows.Forms;

namespace EBoard.SysManager
{
	public partial class MainFrom : Form
	{
		private User currentUser;
		public User CurrentUser
		{
			get { return currentUser; }
			set
			{
				if (currentUser != value)
				{
					currentUser = value;
					UpdateFeatureMenu();
				}
			}
		}

		private void UpdateFeatureMenu()
		{
			var connection = DbFactory.GetConnection();
			var dal = new Dal(connection);
			var roles = dal.GetUserRoles(CurrentUser);

			if (roles.Any(r => string.Equals(r.RoleId, "Administrators", StringComparison.OrdinalIgnoreCase)))
			{
				this.dataMgrToolStripMenuItem.Enabled = true;
				this.userMgrToolStripMenuItem.Enabled = true;
				this.rptMgrToolStripMenuItem.Enabled = true;

				return;
			}

			if (roles.Any(r => string.Equals(r.RoleId, "DataMaintain", StringComparison.OrdinalIgnoreCase)))
			{
				this.dataMgrToolStripMenuItem.Enabled = true;
			}

			if (roles.Any(r => string.Equals(r.RoleId, "ReportManage", StringComparison.OrdinalIgnoreCase)))
			{
				this.rptMgrToolStripMenuItem.Enabled = true;
			}

		}

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
			//OpenForm<DataMaintainForm>();
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

		private void rptMgrToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenForm<ReportMgrForm>();
		}
		
		#endregion
	}
}
