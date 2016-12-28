using System;
using System.Windows.Forms;

namespace EBoard.Common
{
	public partial class ChangePwd : Form
	{
		public string LoginId { get; set; }

		public string UserName { get; set; }

		public string NewEncyptedPassword { get; private set; }

		public ChangePwd()
		{
			InitializeComponent();
		}

		private void ChangePwd_Load(object sender, EventArgs e)
		{
			Text = $"为 {LoginId} 设定新的密码";
		}

		private void buttonOk_Click(object sender, EventArgs e)
		{
			if (!string.Equals(textBoxPwd.Text, textBoxConfirm.Text, StringComparison.CurrentCulture))
			{
				MessageBox.Show("您两次输入的密码不一样，请重新输入。");
				textBoxConfirm.Select();
				return;
			}

			if (string.IsNullOrEmpty(textBoxPwd.Text))
			{
				MessageBox.Show("密码不许为空，请重新输入。");
				textBoxPwd.Select();
				return;
			}
			
			var encryptor = new Encryptor();
			NewEncyptedPassword = encryptor.Encrypt(textBoxPwd.Text);
			
			DialogResult = DialogResult.OK;
			Close();
		}

		private void buttonCancel_Click(object sender, EventArgs e)
		{
			DialogResult = DialogResult.Cancel;
			Close();
		}
	}
}
