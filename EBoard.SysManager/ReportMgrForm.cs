using EBoard.Common;
using System;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace EBoard.SysManager
{
	public partial class ReportMgrForm : FormBase
	{
		private SqlConnection connection;

		private bool isInitialzing = true;

		private bool isRefreshing = false;

		public ReportMgrForm()
		{
			InitializeComponent();
		}

		private void ReportMgrForm_Load(object sender, EventArgs e)
		{
			connection = DbFactory.GetConnection();
			//var reporter = new Reporter(connection);
			//reporter.CreateReportFile(2016, 11);
		}

		private TreeNode InitTree()
		{
			TreeNode nodeToSelect = null;
			isInitialzing = true;

			try
			{
				// TODO:
				// 1. Get all monthly report from MonthReportMstr table

				// 2. create node for each record, node.Tag = ReportId, name = yyyy-mm
			}
			finally
			{
				isInitialzing = false;
			}
			
			return nodeToSelect;

		}

		#region Override methods

		protected override void Cleanup()
		{
			base.Cleanup();

			try
			{
				connection.Close();
				connection.Dispose();
			}
			catch (Exception) { }
		}

		public override void RollbackChanges()
		{
			base.RollbackChanges();

			// todo
			//(dataGridViewUser.DataSource as DataTable).RejectChanges();

			HasDirtyData = false;
		}

		public override bool Save()
		{
			if (!base.Save())
				return false;

			try
			{
				// todo
				HasDirtyData = false;
				return true;
			}
			catch (Exception ex)
			{
				MessageBox.Show(string.Format("无法保存数据。{0}", ex.ToString()));
				return false;
			}
		}

		protected override void UpdateMenuState()
		{
			base.UpdateMenuState();

			//saveToolStripMenuItem.Enabled = HasDirtyData;
			//saveToolStripButton.Enabled = HasDirtyData;

			//var hasData = dataGridViewUser.Rows.Count > 0;
			//deleteToolStripMenuItem.Enabled = hasData;
			//deleteToolStripButton.Enabled = hasData;
			//changePwdToolStripMenuItem.Enabled = hasData;
			//changePwdToolStripButton.Enabled = hasData;
		}
		#endregion
	}
}
