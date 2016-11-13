using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EBoard.SysManager
{
	public partial class ChangePwd : Form
	{
		public string LogId { get; set; }

		public string UserName { get; set; }

		public string NewEncyptedPassword { get; private set; }

		public ChangePwd()
		{
			InitializeComponent();
		}

		private void ChangePwd_Load(object sender, EventArgs e)
		{
			this.Text = string.Format("为 {0} 设定新的密码", LogId);
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

			// TODO: encrypt the password
			NewEncyptedPassword = textBoxPwd.Text;

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
