using System;
using EBoard.Common;
using System.Data.SqlClient;
using System.ComponentModel;
using System.Data;
using System.Windows.Forms;

namespace EBoard.SysManager
{
	public partial class UserMgrForm : FormBase
	{
		private SqlConnection connection;

		private SqlDataAdapter adapter;

		public UserMgrForm()
		{
			InitializeComponent();
		}

		private void UserMgrForm_Load(object sender, EventArgs e)
		{
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

		protected override void UpdateDefaultButton()
		{
			base.UpdateDefaultButton();

			var hasRow = (dataGridViewUser.RowCount > 0);

			deleteToolStripMenuItem.Enabled = hasRow;
			deleteToolStripButton.Enabled = hasRow;

			changePwdToolStripMenuItem.Enabled = hasRow;
			changePwdToolStripButton.Enabled = hasRow;

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
			catch (Exception) { }
		}

		public override void RollbackChanges()
		{
			base.RollbackChanges();

			(dataGridViewUser.DataSource as DataTable).RejectChanges();


			HasDirtyData = false;
		}

		public override bool Save()
		{
			// TODO: For adding a new row but not move focus to other row yet, cannot save since the new data has not been validated
			dataGridViewUser.EndEdit();

			if (!base.Save())
				return false;

			try
			{
				//dataGridViewUser.RefreshEdit();
				var table = dataGridViewUser.DataSource as DataTable;
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
			saveToolStripButton.Enabled = HasDirtyData;

			var hasData = dataGridViewUser.Rows.Count > 0;
			deleteToolStripMenuItem.Enabled = hasData;
			deleteToolStripButton.Enabled = hasData;
			changePwdToolStripMenuItem.Enabled = hasData;
			changePwdToolStripButton.Enabled = hasData;
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

			if (dataGridViewUser.SelectedRows.Count > 0)
			{
				foreach (DataGridViewRow row in dataGridViewUser.SelectedRows)
				{
					dataGridViewUser.Rows.Remove(row);
					rowsDeleted++;
				}
			}
			else
			{
				// Delete current row
				var row = dataGridViewUser.CurrentRow;
				if (row != null)
				{
					dataGridViewUser.Rows.Remove(row);
					rowsDeleted = 1;
				}
			}

			if (rowsDeleted > 0)
			{
				HasDirtyData = true;
			}

			// TODO: save immediately
			//Table.AcceptChanges()

			return rowsDeleted;
		}

		public void ChangePassword()
		{
			var row = dataGridViewUser.CurrentRow;
			if (row == null)
				return;

			var dlg = new ChangePwd
			{
				LogId = row.Cells["LoginId"].Value.ToString(),
				UserName = row.Cells["UserName"].Value.ToString()
			};
			if (dlg.ShowDialog() == DialogResult.OK)
			{
				row.Cells["Password"].Value = dlg.NewEncyptedPassword;
			}

			// TODO: accept data and save immediately
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
	}
}
