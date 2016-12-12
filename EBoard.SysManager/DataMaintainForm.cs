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

		private bool isRefreshing = false;

		private SqlConnection connection;

		private Dictionary<string, Control> nodeMappings = new Dictionary<string, Control>();

		public DataMaintainForm()
		{
			InitializeComponent();
		}

		private void DataMaintainForm_Load(object sender, EventArgs e)
		{
			var node = InitDataGridView();			
			RefreshData(node, false);
		}

		private TreeNode InitDataGridView()
		{
			TreeNode nodeToSelect = null;
			isInitialzing = true;
			try
			{
				connection = DbFactory.GetConnection();
				nodeMappings.Clear();

				// For monitor items
				var dataTypeTable = new DataTable();
				dataTypeTable.Columns.AddRange(new DataColumn[]
												{
													new DataColumn("Data", typeof(int)),
													new DataColumn("Display", typeof(string))
												});
				dataTypeTable.Rows.Add(2, "short");
				dataTypeTable.Rows.Add(3, "DWORD");
				dataTypeTable.Rows.Add(4, "float");
				dataTypeTable.Rows.Add(5, "Double");
				dataTypeTable.Rows.Add(11, "bool");

				DataType.DataSource = dataTypeTable;
				DataType.ValueMember = "Data";
				DataType.DisplayMember = "Display";

				var adapter = (new SqlCommandBuilder(new SqlDataAdapter("SELECT * FROM MonitorItem", connection))).DataAdapter;
				var ds = new DataSet();
				adapter.Fill(ds);
				dataGridViewMonitorItem.DataSource = ds.Tables[0].DefaultView;
				dataGridViewMonitorItem.CellValueChanged += dataGridView_CellValueChanged;
				dataGridViewMonitorItem.Tag = adapter;
				var node = treeView1.Nodes[0].Nodes["MonitorItem"];
				node.Tag = dataGridViewMonitorItem;
				nodeMappings.Add(node.Name.ToLower(), dataGridViewMonitorItem);

				// By default select this node
				nodeToSelect = node;

				// For Generatal parameters
				adapter = (new SqlCommandBuilder(new SqlDataAdapter("SELECT * FROM GeneralParams WHERE Hide=0 ORDER BY DispOrder", connection))).DataAdapter;
				ds = new DataSet();
				adapter.Fill(ds);
				dataGridViewGeneralParams.DataSource = ds.Tables[0].DefaultView;
				dataGridViewGeneralParams.CellValueChanged += dataGridView_CellValueChanged;
				dataGridViewGeneralParams.Tag = adapter;
				
				node = treeView1.Nodes[0].Nodes["SysParam"];
				node.Tag = dataGridViewGeneralParams;
				nodeMappings.Add(node.Name.ToLower(), panelGeneralParams);

				comboBoxCategory.DropDownStyle = ComboBoxStyle.DropDownList;
				InitCategoryList();

				HasDirtyData = false;
			}
			finally
			{
				isInitialzing = false;
			}

			return nodeToSelect;
		}

		public DataRowView Add()
		{
			var view = GetCurrentDataGridView();
			if (view == null)
				return null;

			var row = (view.DataSource as DataView).AddNew();
			view.Select();

			HasDirtyData = true;

			return row;
		}

		public int Delete()
		{
			var view = GetCurrentDataGridView();
			if (view == null)
				return -1;

			var rowsDeleted = 0;
			var protectNotification = @"不允许删除被保护的记录";

			if (MessageBox.Show("记录删除后不可恢复，确定要删除当前选中的记录吗？", "确认", MessageBoxButtons.YesNo) != DialogResult.Yes)
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

			// Save immediately
			Save();

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

			var view = node.Tag as DataGridView;
			if (view == null)
				return;

			isRefreshing = true;

			treeView1.SelectedNode = node;

			var adapter = view.Tag as SqlDataAdapter;
			var ds = new DataSet();
			adapter.Fill(ds);
			view.DataSource = ds.Tables[0].DefaultView;

			// Refresh data may trigger the CellValueChanged event, need to reset hasDirtyData flag
			HasDirtyData = false;

			isRefreshing = false;
		}

		public override bool Save()
		{
			if (!base.Save())
				return false;

			var view = GetCurrentDataGridView();
			if (view == null)
				return false;

			try
			{
				view.EndEdit();

				var adapter = view.Tag as SqlDataAdapter;
				var table = (view.DataSource as DataView).Table;
				
				adapter.Update(table);
				table.AcceptChanges();
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

			saveToolStripMenuItem.Enabled = HasDirtyData;
			toolStripButtonSave.Enabled = HasDirtyData;

			var view = GetCurrentDataGridView();
			if (view == null)
				return;

			var hasData = (view != null) ? (view.Rows.Count > 0) : false;
			deleteToolStripMenuItem.Enabled = hasData;
			toolStripButtonDelete.Enabled = hasData;
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

			var view = GetCurrentDataGridView();
			if (view == null)
				return;

			(view.DataSource as DataView).Table.RejectChanges();
			HasDirtyData = false;
		}

		private DataGridView GetCurrentDataGridView()
		{
			var node = treeView1.SelectedNode;
			if (node == null)
				return null;

			return node.Tag as DataGridView;
		}

		private bool IsRowProtected(DataGridViewRow row)
		{
			if (row == null)
				return false;

			if (!(row.DataGridView.DataSource as DataView).Table.Columns.Contains("IsProtected"))
				return false;

			bool ret;

			return (bool.TryParse(row.Cells["IsProtected"].Value.ToString(), out ret)) ? ret : false;
		}

		private void InitCategoryList()
		{

			var categoryList = new SortedSet<string> { "" };
			var rows = (dataGridViewGeneralParams.DataSource as DataView).Table.Rows;
			foreach (DataRow row in rows)
			{
				categoryList.Add(row["Category"].ToString());
			}

			comboBoxCategory.DataSource = categoryList.ToList();
		}

		#region Event subscription
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

			// Accept the curret changee
			var node = treeView1.SelectedNode;
			if (node != null)
			{
				var view = node.Tag as DataGridView;
				if (view != null)
				{
					view.EndEdit();
					view.CurrentCell = null;
				}
			}

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
			var category = comboBoxCategory.Text;
			var filter = comboBoxCategory.Text == "" ? "" : string.Format("Category = '{0}'", category);

			(dataGridViewGeneralParams.DataSource as DataView).RowFilter = filter;
		}

		private void dataGridViewGeneralParams_DataSourceChanged(object sender, EventArgs e)
		{
			InitCategoryList();
		}
		#endregion

		private void dataGridView_CellValueChanged(object sender, DataGridViewCellEventArgs e)
		{
			if (isRefreshing)
				return;

			HasDirtyData = true;
		}

		private void dataGridViewGeneralParams_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
		{
			var colName = dataGridViewGeneralParams.Columns[e.ColumnIndex].Name;
			if (colName.Equals("ItemName", StringComparison.OrdinalIgnoreCase))
			{
				if (string.Equals("System", dataGridViewGeneralParams.Rows[e.RowIndex].Cells["Category"].Value.ToString(), StringComparison.OrdinalIgnoreCase))
				{
					e.Cancel = true;
				}
			}
		}
	}
}
