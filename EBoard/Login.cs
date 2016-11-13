using EBoard.Common;
using NLog;
using System;
using System.Text;
using System.Windows.Forms;

namespace EBoard
{
	public enum LoginMode
	{
		Typical,
		IdCard
	}

	public partial class Login : Form
	{
		private static readonly Logger Logger = LogManager.GetCurrentClassLogger();

		/// <summary>
		/// The form to open when login succeed
		/// </summary>
		private string nextForm;

		private bool alwaysOpenNew;

		private LoginMode loginMode = LoginMode.Typical;

		private const int MaxSecondsForIdCardScan = 2;

		private string idCardString = "";

		private DateTime idReadBegin;

		public User CurrentUser { get; private set; }

		public Login(string next = "", bool alwaysOpenNew = true)
		{
			InitializeComponent();
			tabControlLogin.SelectedIndex = 0;
			textBoxUserId.Select();

			nextForm = (next ?? "").Trim();
			this.alwaysOpenNew = alwaysOpenNew;
		}

		private void buttonExit_Click(object sender, EventArgs e)
		{
			DialogResult = DialogResult.Cancel;
			Close();
		}

		private void buttonLogin_Click(object sender, EventArgs e)
		{
			DoLogin();
		}

		private void DoLogin()
		{
			if (loginMode == LoginMode.IdCard)
			{
				DialogResult = DialogResult.OK;
				return;
			}

			var loginId = textBoxUserId.Text.Trim();
			var pwd = textBoxPwd.Text;

			// TODO: encrypt the inputted pwd and compare with the one in db
			try
			{
				using (var db = DbFactory.GetConnection())
				{
					var dal = new Dal(db);
					var user = dal.GetUser(loginId);
					if (user == null)
					{
						MessageBox.Show(string.Format("用户[{0}]不存在。", loginId));
						return;
					}
					
					if (user.Status != "A")
					{
						MessageBox.Show("用户[{0}]状态异常，不允许登录。", loginId);
						return;
					}

					// todo encrypt inputted pwd
					var encryptedPwd = pwd;
					if (encryptedPwd != user.Password)
					{
						MessageBox.Show("用户名或密码错误，请重新输入。");
						textBoxPwd.Select();
						return;
					}

					// Validate passed
					CurrentUser = user;
					DialogResult = DialogResult.OK;

					OpenNextForm();

					Close();
				}
			}
			catch (Exception ex)
			{
				Logger.Error(ex, "Failed to get user info.");
				MessageBox.Show("登录时遇到错误，请重试.");
			}
		}

		#region Form operation		
		private void OpenNextForm()
		{
			if (string.IsNullOrEmpty(nextForm))
			{
				Close();
				return;
			}

			var openNew = alwaysOpenNew;
			Form next = null;

			if (openNew)
			{
				next = CreateForm(nextForm);
			}
			else
			{
				next = Application.OpenForms[nextForm];

				// Have to open new since no existing form found
				if (next == null)
				{
					next = CreateForm(nextForm);
				}
			}

			if (next != null)
			{
				next.Show();
				next.Activate();
			}
		}

		private Form CreateForm(string name)
		{
			var type = Type.GetType(name);
			var form = Activator.CreateInstance(type) as Form;
			return form;
		}
		#endregion

		private void textBoxUserId_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Enter)
			{
				textBoxPwd.Focus();
			}
		}

		private void textBoxPwd_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Enter)
			{
				DoLogin();
			}
		}

		private void tabControlLogin_SelectedIndexChanged(object sender, EventArgs e)
		{
			var mode = (tabControlLogin.SelectedIndex == 0) ? LoginMode.Typical : LoginMode.IdCard;
			ChangeLoginMode(mode);
		}

		#region For Login with ID Card
		private void ChangeLoginMode(LoginMode mode)
		{
			loginMode = mode;

			switch (loginMode)
			{
				case LoginMode.Typical:
					buttonLogin.Enabled = true;
					textBoxUserId.Select();
					break;

				case LoginMode.IdCard:
					buttonLogin.Enabled = false;
					labelFlashCard.Show();
					listViewUserInfo.Hide();
					break;

				default:
					break;
			}
		}

		private void OnKeyDown(object sender, KeyEventArgs e)
		{
			e.Handled = false;	// Do not handle if not in IdCard mode or the keystroke is control char

			if (loginMode != LoginMode.IdCard)				
				return;

			if (e.Modifiers != Keys.None)
				return;

			e.Handled = true;

			// Id card input begin
			if (idCardString == "")
			{
				idReadBegin = DateTime.Now;
			}

			// Id card input end
			if (e.KeyCode == Keys.Enter)
			{
				if (idCardString == "")
					return;

				var timesUsed = DateTime.Now - idReadBegin;
				if (timesUsed.TotalSeconds > MaxSecondsForIdCardScan)
				{
					// This is invalid since the input time duration is too long, maybe input from keyboard manually
					ResetIdCardString();
					return;
				}
				
				ShowIdCardContent(idCardString);
				ResetIdCardString();

				return;
			}

			idCardString += Encoding.ASCII.GetString(new byte[] { (byte)e.KeyValue });
		}

		private void ResetIdCardString()
		{
			idCardString = "";
			idReadBegin = DateTime.MinValue;
		}

		private void ShowIdCardContent(string idCard)
		{			
			if (string.IsNullOrWhiteSpace(idCard))
				return;

			try
			{
				using (var db = DbFactory.GetConnection())
				{
					var dal = new Dal(db);
					var user = dal.GetUserByIdCard(idCard);

					// Data validated
					var statusValid = string.Equals(user.Status, "A", StringComparison.OrdinalIgnoreCase);
					labelFlashCard.Hide();
					while (listViewUserInfo.Items.Count > 0) { listViewUserInfo.Items.RemoveAt(0); }
					listViewUserInfo.Show();
					listViewUserInfo.Items.AddRange(new ListViewItem[] {
						new ListViewItem(new string[] { "登录ID", user.LoginId }),
						new ListViewItem(new string[] { "姓名", user.Name }),
						new ListViewItem(new string[] { "状态", statusValid ? "正常" : "异常" })
					});

					buttonLogin.Enabled = statusValid;

					if (statusValid)
					{
						CurrentUser = user;
					}
				}
			}
			catch (Exception ex)
			{
				Logger.Error(ex, "Failed to get user by ID card.");
				MessageBox.Show("无法获取ID Card信息.");
				return;
			}

		}
		#endregion
	}
}
