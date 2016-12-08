using System;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Windows.Forms;

namespace EBoard.Common
{
	public partial class UserPropertyDlg : Form
	{
		private User user;

		private SqlConnection connection;

		private bool isInitializing;

		private bool dataChanged;
		public bool DataChanged
		{
			get { return dataChanged; }
			private set
			{
				dataChanged = value;
				buttonSave.Enabled = dataChanged;
			}
		}

		public UserPropertyDlg(SqlConnection conn)
		{
			InitializeComponent();

			connection = conn;
		}

		private string loginId;
		public string LoginId
		{
			get { return loginId; }
			set
			{
				if (loginId == value)
					return;
				try
				{
					isInitializing = true;
					loginId = value;

					var dal = new Dal(connection);
					user = dal.GetUser(LoginId);
					dal.GetUserRoles(user);

					Text = string.Format("{0} 属性", loginId);
					labelLoginId.Text = LoginId;

					textBoxUserName.DataBindings.Add("Text", user, "Name");
					textBoxIDCard.DataBindings.Add("Text", user, "IDCard");
					checkBoxDisable.Checked = user.Status != "A";

					// Roles
					dal.GetUserRoles(user);
					dataGridViewRoles.Rows.Clear();
					foreach (var role in user.Roles)
					{
						var rowNum = dataGridViewRoles.Rows.Add();
						dataGridViewRoles.Rows[rowNum].Cells["RoleId"].Value = role.RoleId;
						dataGridViewRoles.Rows[rowNum].Cells["RoleName"].Value = role.Name;
					}

					DataChanged = false;
				}
				finally
				{
					isInitializing = false;
				}
			}
		}

		private void buttonSave_Click(object sender, System.EventArgs e)
		{
			user.Status = checkBoxDisable.Checked ? "X" : "A";

			if (!string.IsNullOrEmpty(textBoxPassword.Text))
			{
				// User changed the password
				if (!string.Equals(textBoxPassword.Text, textBoxPassowrdConfirm.Text, StringComparison.CurrentCulture))
				{
					MessageBox.Show("两次输入的密码不一致，请重新输入");
					return;
				}

				// TODO encryt password
				user.Password = textBoxPassword.Text;
			}

			var dal = new Dal(connection);
			try
			{
				dal.UpdateUser(user);
			}
			catch (Exception ex)
			{
				MessageBox.Show("无法保存用户信息。{0}", ex.ToString());
				return;
			}

			buttonSave.Enabled = false;
		}

		private void textBox_TextChanged(object sender, System.EventArgs e)
		{
			if (isInitializing)
				return;

			DataChanged = true;
		}

		private void checkBoxDisable_CheckedChanged(object sender, System.EventArgs e)
		{
			if (isInitializing)
				return;

			DataChanged = true;
		}

		private void buttonAdd_Click(object sender, EventArgs e)
		{
			var dlg = new RolesDlg(connection);
			dlg.ShowDialog();
			if (dlg.ReturnedRole == null)
				return;

			try
			{
				var dal = new Dal(connection);
				if (dal.AddRole(dlg.ReturnedRole.RoleId, user.UserId))
				{
					var rowNum = dataGridViewRoles.Rows.Add();
					var row = dataGridViewRoles.Rows[rowNum];
					row.Cells["RoleId"].Value = dlg.ReturnedRole.RoleId;
					row.Cells["RoleName"].Value = dlg.ReturnedRole.Name;
					user.Roles.Add(dlg.ReturnedRole);
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show("无法添加角色。" + ex.ToString());
			}
		}

		private void buttonDelete_Click(object sender, EventArgs e)
		{
			if (dataGridViewRoles.Rows.Count < 1)
				return;

			if (dataGridViewRoles.SelectedRows == null)
				return;

			try
			{
				var dal = new Dal(connection);
				foreach (DataGridViewRow row in dataGridViewRoles.SelectedRows)
				{
					var roleId = row.Cells["RoleId"].Value.ToString();
					dal.DeleteRole(roleId, user.UserId.ToString());
					dataGridViewRoles.Rows.Remove(row);
					var found = user.Roles.FirstOrDefault(r => r.RoleId.ToString() == roleId);
					if (found != null)
						user.Roles.Remove(found);
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show("无法删除角色。" + ex.ToString());
			}
		}

		private void tabControlProperty_SelectedIndexChanged(object sender, EventArgs e)
		{
			var flag = (tabControlProperty.SelectedIndex == 1);
			buttonAdd.Visible = flag;
			buttonDelete.Visible = flag;
			buttonSave.Visible = !flag;
		}

		private void UserPropertyDlg_Load(object sender, EventArgs e)
		{
			textBoxPassword.TextChanged += new System.EventHandler(textBox_TextChanged);
			textBoxIDCard.TextChanged += new System.EventHandler(textBox_TextChanged);
			textBoxUserName.TextChanged += new System.EventHandler(textBox_TextChanged);
			textBoxPassowrdConfirm.TextChanged += new System.EventHandler(textBox_TextChanged);
			checkBoxDisable.CheckedChanged += new System.EventHandler(checkBoxDisable_CheckedChanged);
		}
	}
}
