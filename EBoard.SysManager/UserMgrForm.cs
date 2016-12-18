using System;
using EBoard.Common;
using System.Data.SqlClient;
using System.Data;
using System.Linq;
using System.Windows.Forms;
using System.Collections.Generic;
using NLog;

namespace EBoard.SysManager
{
	public partial class UserMgrForm : FormBase
	{
		private readonly Logger logger = NLog.LogManager.GetCurrentClassLogger();

		private SqlConnection connection;

		private SqlDataAdapter adapter;

		private int CurrentRow { get; set; }

		public UserMgrForm()
		{
			InitializeComponent();
		}

		private void UserMgrForm_Load(object sender, EventArgs e)
		{
			CurrentRow = -1;

			RefreshData(false);

			RefreshData(false);
		}

		public void RefreshData(bool checkDirtyData = true)
		{
			if (checkDirtyData && !CheckDirtyData())
				return;

			if (connection == null)
				connection = DbFactory.GetConnection();

			if (adapter == null)
			{
				adapter = new SqlCommandBuilder(new SqlDataAdapter("SELECT * FROM [User]", connection)).DataAdapter;
			}

			var ds = new DataSet();
			adapter.Fill(ds);

			dataGridViewUser.DataSource = ds.Tables[0];

			// Refresh data may trigger the CellValueChanged event, need to reset hasDirtyData flag
			HasDirtyData = false;
		}

		protected override void UpdateMenuState()
		{
			base.UpdateMenuState();

			var hasRow = (dataGridViewUser.RowCount > 0);

			deleteToolStripMenuItem.Enabled = hasRow;
			deleteToolStripButton.Enabled = hasRow;

			changePwdToolStripMenuItem.Enabled = hasRow;
			changePwdToolStripButton.Enabled = hasRow;

			if (!hasRow)
			{
				propertyToolStripMenuItem.Enabled = false;
				propertyToolStripButton.Enabled = false;
			}

			saveToolStripMenuItem.Enabled = HasDirtyData;
			saveToolStripButton.Enabled = HasDirtyData;
		}

		protected override void Cleanup()
		{
			base.Cleanup();

			try
			{
				connection.Close();
				connection.Dispose();
			}
			catch (Exception ex) { logger.Error(ex, "Error occurred in Cleanup method."); }
		}

		public override void RollbackChanges()
		{
			base.RollbackChanges();

			(dataGridViewUser.DataSource as DataTable).RejectChanges();


			HasDirtyData = false;
		}

		public override bool Save()
		{
			dataGridViewUser.EndEdit();
			dataGridViewUser.CurrentCell = null;

			if (!base.Save())
				return false;

			try
			{
				var table = dataGridViewUser.DataSource as DataTable;
				adapter.Update(table);
				table.AcceptChanges();
				HasDirtyData = false;
				RefreshData();
				return true;
			}
			catch (Exception ex)
			{
				logger.Error(ex, "Failed to save data.");
				MessageBox.Show(string.Format("无法保存数据。{0}", ex.ToString()));
				return false;
			}
		}

		public DataRow AddRecord()
		{
			var row = (dataGridViewUser.DataSource as DataTable).Rows.Add();
			row["UserId"] = Guid.NewGuid().ToString();
			dataGridViewUser.Select();

			HasDirtyData = true;

			return row;
		}

		public int DeleteRecord()
		{
			var rowsDeleted = 0;

			if (MessageBox.Show("用户被删除后，无法恢复，确认删除吗？", "确认", MessageBoxButtons.YesNo) != DialogResult.Yes)
				return 0;

			var selectedRows = (dataGridViewUser.SelectedRows.Count > 0) ?
																	  dataGridViewUser.SelectedRows.OfType<DataGridViewRow>().ToList()
																	: ((dataGridViewUser.CurrentRow != null) ?
																											new List<DataGridViewRow>() { dataGridViewUser.CurrentRow }
																											: null);
			if (selectedRows != null)
			{
				foreach (var row in selectedRows)
				{
					if (DeleteUserRow(row))
						rowsDeleted++;
				}
			}

			if (rowsDeleted > 0)
			{
				HasDirtyData = true;
				if (!Save())
					return 0;
			}

			return rowsDeleted;
		}

		private bool DeleteUserRow(DataGridViewRow row)
		{
			if (row == null)
				return false;

			var table = row.DataGridView.DataSource as DataTable;
			if (table == null)
				return false;

			var isNewRow = (table.Rows[row.Index].RowState == DataRowState.Added);
			if (!isNewRow)
			{
				var isProtected = (row.Cells["IsProtected"].Value.GetType() != typeof(DBNull) && (bool)row.Cells["IsProtected"].Value);
				if (isProtected)
				{
					MessageBox.Show("该记录不允许被删除");
					return false;
				}

				// Need to delete roles assigned that user
				var userId = row.Cells["UserId"].Value.ToString();
				var dal = new Dal(connection);
				dal.DeleteRole("*", userId);
			}

			row.DataGridView.Rows.Remove(row);

			return true;
		}

		public void ChangePassword()
		{
			var row = dataGridViewUser.CurrentRow;
			if (row == null)
				return;

			var dlg = new ChangePwd
			{
				LoginId = row.Cells["LoginId"].Value.ToString(),
				UserName = row.Cells["UserName"].Value.ToString()
			};

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				(dataGridViewUser.DataSource as DataTable).Rows[row.Index]["Password"] = dlg.NewEncyptedPassword;
			}

			Save();
		}

		private void ChangeUserProperty()
		{
			var row = dataGridViewUser.CurrentRow;
			if (row == null)
				return;

			var loginId = row.Cells["LoginId"].Value.ToString();
			if (HasDirtyData)
			{
				if (MessageBox.Show("查看或修改用户属性之前，会自动保存已做过的修改，继续吗？", "查看或修改属性", MessageBoxButtons.YesNo) != DialogResult.Yes)
					return;

				if (!Save())
					return;
			}

			var val = row.Cells["IsProtected"].Value;
			var isProtected = (val.GetType() != typeof(DBNull)) ? (bool)val : false;

			var dlg = new UserPropertyDlg(connection) { LoginId = loginId, IsUserProtected = isProtected };
			dlg.ShowDialog();
			if (dlg.DataChanged)
			{
				RefreshData();
			}
		}

		private void dataGridViewUser_CellValueChanged(object sender, DataGridViewCellEventArgs e)
		{
			HasDirtyData = true;
		}

		private void addToolStripMenuItem_Click(object sender, EventArgs e)
		{
			AddRecord();
		}

		private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
		{
			DeleteRecord();
		}

		private void changePwdToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ChangePassword();
		}

		private void refreshToolStripMenuItem_Click(object sender, EventArgs e)
		{
			RefreshData();
		}

		private void saveToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Save();
		}

		private void propertyToolStripMenuItem_Click(object sender, EventArgs e)
		{
			ChangeUserProperty();
		}

		private void dataGridViewUser_CurrentCellChanged(object sender, EventArgs e)
		{
			var rowCount = dataGridViewUser.Rows.Count;
			if (rowCount < 1 && CurrentRow == -1)
				return;

			var currRow = dataGridViewUser.CurrentRow;
			if (currRow == null)
			{
				CurrentRow = -1;
				return;
			}

			if (CurrentRow == currRow.Index)
				return;

			CurrentRow = currRow.Index;
			propertyToolStripMenuItem.Enabled = ((dataGridViewUser.DataSource as DataTable).Rows[currRow.Index].RowState != DataRowState.Added);
			propertyToolStripButton.Enabled = propertyToolStripMenuItem.Enabled;
		}

		private void dataGridViewUser_DoubleClick(object sender, EventArgs e)
		{
			ChangeUserProperty();
		}

		private void dataGridViewUser_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
		{
			var colName = dataGridViewUser.Columns[e.ColumnIndex].Name;
			if (colName.Equals("LoginId", StringComparison.OrdinalIgnoreCase) || colName.Equals("Status", StringComparison.OrdinalIgnoreCase))
			{
				var val = dataGridViewUser.Rows[e.RowIndex].Cells["IsProtected"].Value;
				if (val.GetType() != typeof(DBNull) && (bool)val)
				{
					e.Cancel = true; ;
				}
			}
		}
	}
}
