using System;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace EBoard.Common
{
	public partial class UserPropertyDlg : Form
	{
		private User user;

		private SqlConnection connection;

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

			// TODO: update role info

			DataChanged = false;
			// todo exit
		}

		private void textBox_TextChanged(object sender, System.EventArgs e)
		{
			DataChanged = true;
		}

		private void checkBoxDisable_CheckedChanged(object sender, System.EventArgs e)
		{
			DataChanged = true;
		}

		private void buttonAdd_Click(object sender, EventArgs e)
		{
			//
		}

		private void buttonDelete_Click(object sender, EventArgs e)
		{
			//
		}
	}
}
