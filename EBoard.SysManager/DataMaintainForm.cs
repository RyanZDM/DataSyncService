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

namespace EBoard.SysManager
{
	public partial class DataMaintainForm : FormBase
	{
		private bool isInitialzing = true;

		private SqlConnection connection;

		private Dictionary<string, Control> nodeMappings = new Dictionary<string, Control>();

		//private List<SqlDataAdapter> adapters = new List<SqlDataAdapter>();
		
		public DataMaintainForm()
		{
			InitializeComponent();
		}

		private void DataMaintainForm_Load(object sender, EventArgs e)
		{
			InitDataGridView();
			RefreshData(null, false);
		}

		private void InitDataGridView()
		{
			isInitialzing = true;
			try
			{
				connection = DbFactory.GetConnection();
				nodeMappings.Clear();

				// For monitor items
				var adapter = (new SqlCommandBuilder(new SqlDataAdapter("SELECT * FROM MonitorItem", connection))).DataAdapter;
				var ds = new DataSet();
				adapter.Fill(ds);
				dataGridViewMonitorItem.DataSource = ds.Tables[0];
				dataGridViewMonitorItem.Tag = adapter;
				var node = treeView1.Nodes[0].Nodes["MonitorItem"];
				node.Tag = dataGridViewMonitorItem;
				nodeMappings.Add(node.Name.ToLower(), dataGridViewMonitorItem);

				// For Generatal parameters
				adapter = (new SqlCommandBuilder(new SqlDataAdapter("SELECT * FROM GeneralParams WHERE Hide=0", connection))).DataAdapter;
				ds = new DataSet();
				adapter.Fill(ds);
				dataGridViewGeneralParams.DataSource = ds.Tables[0];
				dataGridViewGeneralParams.Tag = adapter;
				node = treeView1.Nodes[0].Nodes["SysParam"];
				node.Tag = dataGridViewGeneralParams;
				nodeMappings.Add(node.Name.ToLower(), panelGeneralParams);

				var dal = new Dal(connection);
				var categoryList = dal.GetGeneralParamCategory();
				comboBoxCategory.DataSource = categoryList;
			}
			finally
			{
				isInitialzing = false;
			}
		}

		public DataRow Add()
		{
			var node = treeView1.SelectedNode;
			var view = node.Tag as DataGridView;
			if (view == null)
				return null;

			var row = (view.DataSource as DataTable).Rows.Add();
			view.Select();

			HasDirtyData = true;

			return row;
		}
		
		public int Delete()
		{
			var node = treeView1.SelectedNode;
			var view = node.Tag as DataGridView;
			if (view == null)
				return -1;

			var rowsDeleted = 0;
			var protectNotification = @"不允许删除被保护的记录";

			if (MessageBox.Show("确定要删除当前选中的记录吗？", "确认", MessageBoxButtons.YesNo) != DialogResult.Yes)
				return 0;

			if (view.SelectedRows.Count > 0)
			{
				foreach (DataGridViewRow row in view.SelectedRows)
				{
					if (IsRowProtected(row))
					{
						MessageBox.Show(protectNotification);
						continue;
					}

					view.Rows.Remove(row);
					rowsDeleted++;
				}
			}
			else
			{
				// Delete current row
				var row = view.CurrentRow;
				if (row != null)
				{
					if (!IsRowProtected(row))
					{
						view.Rows.Remove(row);
						rowsDeleted = 1;
					}
					else
					{
						MessageBox.Show(protectNotification);
					}					
				}
			}

			if (rowsDeleted > 0)
			{
				HasDirtyData = true;
			}

			// TODO: save immediately

			return rowsDeleted;
		}

		public void RefreshData(bool checkDirtyData = true)
		{
			RefreshData(treeView1.SelectedNode, checkDirtyData);
		}

		public void RefreshData(TreeNode node, bool checkDirtyData = true)
		{
			if (node == null)
				return;

			if (checkDirtyData && !CheckDirtyData())
				return;

			if (connection == null)
				connection = DbFactory.GetConnection();

			// TODO: Get settings of current node and refresh data
			var view = node.Tag as DataGridView;
			if (view == null)
				return;

			var adapter = view.Tag as SqlDataAdapter;			
			var ds = new DataSet();
			adapter.Fill(ds);
			view.DataSource = ds.Tables[0];
			//view.BringToFront();
			
			// Refresh data may trigger the CellValueChanged event, need to reset hasDirtyData flag
			HasDirtyData = false;
		}

		public override bool Save()
		{
			return base.Save();
		}

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

			var node = treeView1.SelectedNode;
			var view = node.Tag as DataGridView;
			if (view == null)
				return;

			(view.DataSource as DataTable).RejectChanges();
			HasDirtyData = false;
		}

		private bool IsRowProtected(DataGridViewRow row)
		{
			if (row == null)
				return false;

			if (!(row.DataGridView.DataSource as DataTable).Columns.Contains("IsProtected"))
				return false;

			bool ret;

			return (bool.TryParse(row.Cells["IsProtected"].Value.ToString(), out ret)) ? ret : false;
		}

		private void addToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Add();
		}

		private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Delete();
		}

		private void refreshToolStripMenuItem_Click(object sender, EventArgs e)
		{
			RefreshData();
		}

		private void saveToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Save();
		}

		private void treeView1_BeforeSelect(object sender, TreeViewCancelEventArgs e)
		{
			if (isInitialzing)
				return;

			if (!CheckDirtyData())
				e.Cancel = true;
		}

		private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
		{
			if (isInitialzing)
				return;

			var nodeName = e.Node.Name.ToLower();
			if (!nodeMappings.ContainsKey(nodeName))
				return;

			nodeMappings[nodeName].BringToFront();
		}

		private void comboBoxCategory_SelectedIndexChanged(object sender, EventArgs e)
		{
			//Filter
		}
	}
}
