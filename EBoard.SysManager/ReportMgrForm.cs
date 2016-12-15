using EBoard.Common;
using System;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace EBoard.SysManager
{
	public partial class ReportMgrForm : FormBase
	{
		private SqlConnection connection;

		private Reporter reporter;

		private bool isInitialzing = true;

		private bool isRefreshing = false;

		public ReportMgrForm()
		{
			InitializeComponent();
		}

		private void ReportMgrForm_Load(object sender, EventArgs e)
		{
			connection = DbFactory.GetConnection();
			reporter = new Reporter(connection);

			var nodeToSelect = InitTree();
			if (nodeToSelect != null)
			{
				treeView1.SelectedNode = nodeToSelect;
			}

			//reporter.CreateReportFile(2016, 11);
		}
		
		private TreeNode InitTree()
		{
			TreeNode nodeToSelect = null;
			isInitialzing = true;
			
			var rootNode = treeView1.Nodes.Add("月报表");
			try
			{
				var reportList = reporter.GetMonthltReportList();
				foreach (var year in reportList.Keys)
				{
					var yearNode = rootNode.Nodes.Add(year.ToString(), year.ToString());
					foreach (var month in reportList[year].Keys)
					{
						var monthNode = yearNode.Nodes.Add(reportList[year][month], month.ToString());
						monthNode.Tag = monthNode.Name;		// Report Id
						if (nodeToSelect == null)
							nodeToSelect = monthNode;		// Select the first node
					}
				}
			}
			finally
			{
				isInitialzing = false;
			}
			
			return nodeToSelect;

		}

		private void RetrieveMonthReport()
		{
			var currentNode = treeView1.SelectedNode;
			if (currentNode == null)
				return;

			var reportId = currentNode.Tag as string;
			if (string.IsNullOrWhiteSpace(reportId))
				return;

			// Show monthly report mstr info


			// Show monthly report detail info


			// Show monthly report worker stat info
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

		private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
		{
			RetrieveMonthReport();
		}

		private void treeView1_BeforeSelect(object sender, TreeViewCancelEventArgs e)
		{
			if (e.Node == null)
				return;

			var reportId = e.Node.Tag as string;
			if (string.IsNullOrWhiteSpace(reportId))
			{
				e.Cancel = true;
			}
		}
	}
}
