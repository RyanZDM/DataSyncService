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

		protected bool HasDirtyData
		{
			get { return hasDirtyData; }
			set
			{
				//if (hasDirtyData == value)
				//	return;

				hasDirtyData = value;
				CheckMenuState();
			}
		}

		public UserMgrForm()
		{
			InitializeComponent();
		}

		private void UserMgrForm_Load(object sender, EventArgs e)
		{
			// This flag is changed after InitializeComponent(), no need to check menu state
			hasDirtyData = false;
			RefreshData();
		}

		public void RefreshData()
		{
			if (!CheckDirtyData())
				return;

			if (connection == null)
				connection = DbFactory.GetConnection();

			if (adapter == null)
			{
				adapter = new SqlDataAdapter("SELECT * FROM [User]", connection);
				new SqlCommandBuilder(adapter);
			}

			var ds = new DataSet();			
			adapter.Fill(ds);

			dataGridViewUser.DataSource = ds.Tables[0];

			// Refresh data may trigger the CellValueChanged event, need to reset hasDirtyData flag
			HasDirtyData = false;
		}

		private void CheckMenuState()
		{
			var hasRow = (dataGridViewUser.RowCount > 0);

			deleteToolStripMenuItem.Enabled = hasRow;
			deleteToolStripButton.Enabled = hasRow;

			changePwdToolStripMenuItem.Enabled = hasRow;
			changePwdToolStripButton.Enabled = hasRow;

			saveToolStripMenuItem.Enabled = hasDirtyData;
			saveToolStripButton.Enabled = hasDirtyData;
		}

		protected override void Cleanup()
		{
			base.Cleanup();

			try
			{
				connection.Close();
				connection.Dispose();
			}
			catch (Exception) {}
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
				adapter.Update(dataGridViewUser.DataSource as DataTable);
				HasDirtyData = false;
				return true;
			}
			catch (Exception ex)
			{
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
			var rowDeleted = 0;

			if (dataGridViewUser.SelectedRows.Count > 0)
			{
				foreach (DataGridViewRow row in dataGridViewUser.SelectedRows)
				{
					dataGridViewUser.Rows.Remove(row);
					rowDeleted++;
				}
			}
			else
			{
				// Delete current row
				var row = dataGridViewUser.CurrentRow;
				if (row != null)
				{
					dataGridViewUser.Rows.Remove(row);
					rowDeleted = 1;					
				}
			}

			if (rowDeleted > 0)
			{
				HasDirtyData = true;
			}

			// TODO: save immediately

			return rowDeleted;
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
