using System;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace EBoard.Common
{
	public partial class RolesDlg : Form
	{
		private SqlConnection connection;

		public Role ReturnedRole { get; private set; }

		public RolesDlg(SqlConnection conn)
		{
			connection = conn;
			InitializeComponent();
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
			var currCell = dataGridViewRole.CurrentCell;
			if (currCell == null)
			{
				MessageBox.Show("请先选择一条记录");
				return;
			}

			var row = dataGridViewRole.Rows[currCell.RowIndex];
			var role = new Role
			{
				RoleId = row.Cells["RoleId"].Value.ToString(),
				Name = row.Cells["RoleName"].Value.ToString(),
				Status = "A"
			};

			ReturnedRole = role;
		}
	}
}
