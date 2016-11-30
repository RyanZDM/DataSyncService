namespace EBoard.Common
{
	partial class Login
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem("");
			this.buttonLogin = new System.Windows.Forms.Button();
			this.buttonExit = new System.Windows.Forms.Button();
			this.tabControlLogin = new System.Windows.Forms.TabControl();
			this.tabPageTypical = new System.Windows.Forms.TabPage();
			this.labelPassword = new System.Windows.Forms.Label();
			this.labelUserId = new System.Windows.Forms.Label();
			this.textBoxPwd = new System.Windows.Forms.TextBox();
			this.textBoxUserId = new System.Windows.Forms.TextBox();
			this.tabPageIdCard = new System.Windows.Forms.TabPage();
			this.listViewUserInfo = new System.Windows.Forms.ListView();
			this.columnHeaderId = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeaderValue = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.labelFlashCard = new System.Windows.Forms.Label();
			this.tabControlLogin.SuspendLayout();
			this.tabPageTypical.SuspendLayout();
			this.tabPageIdCard.SuspendLayout();
			this.SuspendLayout();
			// 
			// buttonLogin
			// 
			this.buttonLogin.Location = new System.Drawing.Point(303, 196);
			this.buttonLogin.Margin = new System.Windows.Forms.Padding(2);
			this.buttonLogin.Name = "buttonLogin";
			this.buttonLogin.Size = new System.Drawing.Size(71, 24);
			this.buttonLogin.TabIndex = 35;
			this.buttonLogin.Text = "确定";
			this.buttonLogin.UseVisualStyleBackColor = true;
			this.buttonLogin.Click += new System.EventHandler(this.buttonLogin_Click);
			this.buttonLogin.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
			// 
			// buttonExit
			// 
			this.buttonExit.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.buttonExit.Location = new System.Drawing.Point(92, 196);
			this.buttonExit.Margin = new System.Windows.Forms.Padding(2);
			this.buttonExit.Name = "buttonExit";
			this.buttonExit.Size = new System.Drawing.Size(71, 24);
			this.buttonExit.TabIndex = 40;
			this.buttonExit.Text = "退出";
			this.buttonExit.UseVisualStyleBackColor = true;
			this.buttonExit.Click += new System.EventHandler(this.buttonExit_Click);
			this.buttonExit.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
			// 
			// tabControlLogin
			// 
			this.tabControlLogin.Controls.Add(this.tabPageTypical);
			this.tabControlLogin.Controls.Add(this.tabPageIdCard);
			this.tabControlLogin.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.tabControlLogin.Location = new System.Drawing.Point(14, 9);
			this.tabControlLogin.Margin = new System.Windows.Forms.Padding(2);
			this.tabControlLogin.Multiline = true;
			this.tabControlLogin.Name = "tabControlLogin";
			this.tabControlLogin.SelectedIndex = 0;
			this.tabControlLogin.Size = new System.Drawing.Size(405, 172);
			this.tabControlLogin.TabIndex = 5;
			this.tabControlLogin.SelectedIndexChanged += new System.EventHandler(this.tabControlLogin_SelectedIndexChanged);
			this.tabControlLogin.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
			// 
			// tabPageTypical
			// 
			this.tabPageTypical.Controls.Add(this.labelPassword);
			this.tabPageTypical.Controls.Add(this.labelUserId);
			this.tabPageTypical.Controls.Add(this.textBoxPwd);
			this.tabPageTypical.Controls.Add(this.textBoxUserId);
			this.tabPageTypical.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.tabPageTypical.Location = new System.Drawing.Point(4, 22);
			this.tabPageTypical.Margin = new System.Windows.Forms.Padding(2);
			this.tabPageTypical.Name = "tabPageTypical";
			this.tabPageTypical.Padding = new System.Windows.Forms.Padding(2);
			this.tabPageTypical.Size = new System.Drawing.Size(397, 146);
			this.tabPageTypical.TabIndex = 0;
			this.tabPageTypical.Text = "使用密码登录";
			this.tabPageTypical.ToolTipText = "使用用户名/密码登录";
			this.tabPageTypical.UseVisualStyleBackColor = true;
			// 
			// labelPassword
			// 
			this.labelPassword.AutoSize = true;
			this.labelPassword.Location = new System.Drawing.Point(119, 80);
			this.labelPassword.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelPassword.Name = "labelPassword";
			this.labelPassword.Size = new System.Drawing.Size(31, 13);
			this.labelPassword.TabIndex = 0;
			this.labelPassword.Text = "密码";
			this.labelPassword.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// labelUserId
			// 
			this.labelUserId.AutoSize = true;
			this.labelUserId.Location = new System.Drawing.Point(109, 58);
			this.labelUserId.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelUserId.Name = "labelUserId";
			this.labelUserId.Size = new System.Drawing.Size(43, 13);
			this.labelUserId.TabIndex = 0;
			this.labelUserId.Text = "用户名";
			this.labelUserId.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// textBoxPwd
			// 
			this.textBoxPwd.ImeMode = System.Windows.Forms.ImeMode.Off;
			this.textBoxPwd.Location = new System.Drawing.Point(158, 77);
			this.textBoxPwd.Margin = new System.Windows.Forms.Padding(2);
			this.textBoxPwd.Name = "textBoxPwd";
			this.textBoxPwd.PasswordChar = '*';
			this.textBoxPwd.Size = new System.Drawing.Size(134, 19);
			this.textBoxPwd.TabIndex = 2;
			this.textBoxPwd.UseSystemPasswordChar = true;
			this.textBoxPwd.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxPwd_KeyDown);
			// 
			// textBoxUserId
			// 
			this.textBoxUserId.Location = new System.Drawing.Point(158, 56);
			this.textBoxUserId.Margin = new System.Windows.Forms.Padding(2);
			this.textBoxUserId.Name = "textBoxUserId";
			this.textBoxUserId.Size = new System.Drawing.Size(134, 19);
			this.textBoxUserId.TabIndex = 1;
			this.textBoxUserId.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxUserId_KeyDown);
			// 
			// tabPageIdCard
			// 
			this.tabPageIdCard.Controls.Add(this.listViewUserInfo);
			this.tabPageIdCard.Controls.Add(this.labelFlashCard);
			this.tabPageIdCard.Location = new System.Drawing.Point(4, 22);
			this.tabPageIdCard.Margin = new System.Windows.Forms.Padding(2);
			this.tabPageIdCard.Name = "tabPageIdCard";
			this.tabPageIdCard.Padding = new System.Windows.Forms.Padding(2);
			this.tabPageIdCard.Size = new System.Drawing.Size(397, 146);
			this.tabPageIdCard.TabIndex = 1;
			this.tabPageIdCard.Text = "使用工卡登录";
			this.tabPageIdCard.ToolTipText = "使用工卡登录";
			this.tabPageIdCard.UseVisualStyleBackColor = true;
			// 
			// listViewUserInfo
			// 
			this.listViewUserInfo.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeaderId,
            this.columnHeaderValue});
			this.listViewUserInfo.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.listViewUserInfo.GridLines = true;
			this.listViewUserInfo.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.listViewUserInfo.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem1});
			this.listViewUserInfo.Location = new System.Drawing.Point(53, 32);
			this.listViewUserInfo.Margin = new System.Windows.Forms.Padding(2);
			this.listViewUserInfo.Name = "listViewUserInfo";
			this.listViewUserInfo.Scrollable = false;
			this.listViewUserInfo.Size = new System.Drawing.Size(283, 98);
			this.listViewUserInfo.TabIndex = 1;
			this.listViewUserInfo.UseCompatibleStateImageBehavior = false;
			this.listViewUserInfo.View = System.Windows.Forms.View.Details;
			this.listViewUserInfo.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
			// 
			// columnHeaderId
			// 
			this.columnHeaderId.Text = "";
			this.columnHeaderId.Width = 100;
			// 
			// columnHeaderValue
			// 
			this.columnHeaderValue.Text = "";
			this.columnHeaderValue.Width = 300;
			// 
			// labelFlashCard
			// 
			this.labelFlashCard.Dock = System.Windows.Forms.DockStyle.Fill;
			this.labelFlashCard.Font = new System.Drawing.Font("Microsoft Sans Serif", 19.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.labelFlashCard.Location = new System.Drawing.Point(2, 2);
			this.labelFlashCard.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelFlashCard.Name = "labelFlashCard";
			this.labelFlashCard.Size = new System.Drawing.Size(393, 142);
			this.labelFlashCard.TabIndex = 0;
			this.labelFlashCard.Text = "请刷卡...";
			this.labelFlashCard.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// Login
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(428, 228);
			this.ControlBox = false;
			this.Controls.Add(this.tabControlLogin);
			this.Controls.Add(this.buttonExit);
			this.Controls.Add(this.buttonLogin);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(2);
			this.Name = "Login";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Login";
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
			this.tabControlLogin.ResumeLayout(false);
			this.tabPageTypical.ResumeLayout(false);
			this.tabPageTypical.PerformLayout();
			this.tabPageIdCard.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion
		private System.Windows.Forms.Button buttonLogin;
		private System.Windows.Forms.Button buttonExit;
		private System.Windows.Forms.TabControl tabControlLogin;
		private System.Windows.Forms.TabPage tabPageTypical;
		private System.Windows.Forms.Label labelPassword;
		private System.Windows.Forms.Label labelUserId;
		private System.Windows.Forms.TextBox textBoxPwd;
		private System.Windows.Forms.TextBox textBoxUserId;
		private System.Windows.Forms.TabPage tabPageIdCard;
		private System.Windows.Forms.Label labelFlashCard;
		private System.Windows.Forms.ListView listViewUserInfo;
		private System.Windows.Forms.ColumnHeader columnHeaderId;
		private System.Windows.Forms.ColumnHeader columnHeaderValue;
	}
}