using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Windows.Forms;

namespace EBoard.Common
{
	public partial class RolesDlg : Form
	{
		private SqlConnection connection;

		private bool allowMultipleSelect;
		public bool AllowMultipleSelect
		{
			get { return allowMultipleSelect; }
			set { dataGridViewRole.MultiSelect = value; }
		}

		public IList<Role> ReturnedRoles { get; private set; }

		public RolesDlg(SqlConnection conn)
		{
			connection = conn;
			InitializeComponent();
			allowMultipleSelect = dataGridViewRole.MultiSelect;
		}

		private void RolesDlg_Load(object sender, EventArgs e)
		{
			var sql = "Select * from Role Where Status='A'";
			var adapter = new SqlDataAdapter(sql, connection);
			var ds = new DataSet();
			adapter.Fill(ds);
			dataGridViewRole.DataSource = ds.Tables[0];
		}

		private void buttonOK_Click(object sender, EventArgs e)
		{
			var selectedRows = (dataGridViewRole.SelectedRows.Count > 0) ?
																			  dataGridViewRole.SelectedRows.OfType<DataGridViewRow>().ToList()
																			: ((dataGridViewRole.CurrentRow != null) ?
																													new List<DataGridViewRow>() { dataGridViewRole.CurrentRow }
																													: null);
			if (selectedRows == null || selectedRows.Count < 1)
			{
				MessageBox.Show("请先选择一条记录");
				return;
			}

			ReturnedRoles = new List<Role>();
			foreach (var row in selectedRows)
			{
				ReturnedRoles.Add(
					new Role
					{
						RoleId = row.Cells["RoleId"].Value.ToString(),
						Name = row.Cells["RoleName"].Value.ToString(),
						Status = "A"
					}
				);
			}
		}
	}
}
