namespace EBoard.SysManager
{
	partial class UserMgrForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(UserMgrForm));
			this.dataGridViewUser = new System.Windows.Forms.DataGridView();
			this.UserId = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.LoginId = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.UserName = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Password = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.IDCard = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Status = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.operationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.addToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.changePwdToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
			this.refreshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
			this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.saveToolStripButton = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.addToolStripButton = new System.Windows.Forms.ToolStripButton();
			this.deleteToolStripButton = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this.changePwdToolStripButton = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
			this.refreshToolStripButton = new System.Windows.Forms.ToolStripButton();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewUser)).BeginInit();
			this.menuStrip1.SuspendLayout();
			this.toolStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// dataGridViewUser
			// 
			this.dataGridViewUser.AllowUserToAddRows = false;
			this.dataGridViewUser.AllowUserToDeleteRows = false;
			this.dataGridViewUser.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.dataGridViewUser.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridViewUser.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.UserId,
            this.LoginId,
            this.UserName,
            this.Password,
            this.IDCard,
            this.Status});
			this.dataGridViewUser.Location = new System.Drawing.Point(0, 30);
			this.dataGridViewUser.Name = "dataGridViewUser";
			this.dataGridViewUser.RowTemplate.Height = 24;
			this.dataGridViewUser.Size = new System.Drawing.Size(713, 399);
			this.dataGridViewUser.TabIndex = 0;
			this.dataGridViewUser.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridViewUser_CellValueChanged);
			// 
			// UserId
			// 
			this.UserId.DataPropertyName = "UserId";
			this.UserId.HeaderText = "UserId";
			this.UserId.Name = "UserId";
			this.UserId.Visible = false;
			// 
			// LoginId
			// 
			this.LoginId.DataPropertyName = "LoginId";
			this.LoginId.HeaderText = "登录ID";
			this.LoginId.MaxInputLength = 50;
			this.LoginId.Name = "LoginId";
			this.LoginId.Width = 150;
			// 
			// UserName
			// 
			this.UserName.DataPropertyName = "Name";
			this.UserName.HeaderText = "姓名";
			this.UserName.MaxInputLength = 50;
			this.UserName.Name = "UserName";
			this.UserName.Width = 150;
			// 
			// Password
			// 
			this.Password.DataPropertyName = "Password";
			this.Password.HeaderText = "密码";
			this.Password.MaxInputLength = 50;
			this.Password.Name = "Password";
			this.Password.Visible = false;
			// 
			// IDCard
			// 
			this.IDCard.DataPropertyName = "IDCard";
			this.IDCard.HeaderText = "IDCard";
			this.IDCard.MaxInputLength = 100;
			this.IDCard.Name = "IDCard";
			this.IDCard.Width = 300;
			// 
			// Status
			// 
			this.Status.DataPropertyName = "Status";
			this.Status.FalseValue = "X";
			this.Status.HeaderText = "启用";
			this.Status.Name = "Status";
			this.Status.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Status.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
			this.Status.TrueValue = "A";
			this.Status.Width = 50;
			// 
			// menuStrip1
			// 
			this.menuStrip1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.menuStrip1.Dock = System.Windows.Forms.DockStyle.None;
			this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.operationToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(656, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(57, 27);
			this.menuStrip1.TabIndex = 1;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// operationToolStripMenuItem
			// 
			this.operationToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addToolStripMenuItem,
            this.deleteToolStripMenuItem,
            this.toolStripMenuItem1,
            this.changePwdToolStripMenuItem,
            this.toolStripMenuItem2,
            this.refreshToolStripMenuItem,
            this.toolStripMenuItem3,
            this.saveToolStripMenuItem});
			this.operationToolStripMenuItem.MergeAction = System.Windows.Forms.MergeAction.Insert;
			this.operationToolStripMenuItem.MergeIndex = 1;
			this.operationToolStripMenuItem.Name = "operationToolStripMenuItem";
			this.operationToolStripMenuItem.Size = new System.Drawing.Size(49, 23);
			this.operationToolStripMenuItem.Text = "操作";
			// 
			// addToolStripMenuItem
			// 
			this.addToolStripMenuItem.Name = "addToolStripMenuItem";
			this.addToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Insert)));
			this.addToolStripMenuItem.Size = new System.Drawing.Size(173, 26);
			this.addToolStripMenuItem.Text = "添加";
			this.addToolStripMenuItem.Click += new System.EventHandler(this.addToolStripMenuItem_Click);
			// 
			// deleteToolStripMenuItem
			// 
			this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
			this.deleteToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Delete)));
			this.deleteToolStripMenuItem.Size = new System.Drawing.Size(173, 26);
			this.deleteToolStripMenuItem.Text = "删除";
			this.deleteToolStripMenuItem.Click += new System.EventHandler(this.deleteToolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(170, 6);
			// 
			// changePwdToolStripMenuItem
			// 
			this.changePwdToolStripMenuItem.Name = "changePwdToolStripMenuItem";
			this.changePwdToolStripMenuItem.Size = new System.Drawing.Size(173, 26);
			this.changePwdToolStripMenuItem.Text = "修改密码";
			this.changePwdToolStripMenuItem.Click += new System.EventHandler(this.changePwdToolStripMenuItem_Click);
			// 
			// toolStripMenuItem2
			// 
			this.toolStripMenuItem2.Name = "toolStripMenuItem2";
			this.toolStripMenuItem2.Size = new System.Drawing.Size(170, 6);
			// 
			// refreshToolStripMenuItem
			// 
			this.refreshToolStripMenuItem.Name = "refreshToolStripMenuItem";
			this.refreshToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F5;
			this.refreshToolStripMenuItem.Size = new System.Drawing.Size(173, 26);
			this.refreshToolStripMenuItem.Text = "刷新";
			this.refreshToolStripMenuItem.Click += new System.EventHandler(this.refreshToolStripMenuItem_Click);
			// 
			// toolStripMenuItem3
			// 
			this.toolStripMenuItem3.Name = "toolStripMenuItem3";
			this.toolStripMenuItem3.Size = new System.Drawing.Size(170, 6);
			// 
			// saveToolStripMenuItem
			// 
			this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
			this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
			this.saveToolStripMenuItem.Size = new System.Drawing.Size(173, 26);
			this.saveToolStripMenuItem.Text = "保存";
			this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
			// 
			// toolStrip1
			// 
			this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
			this.toolStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveToolStripButton,
            this.toolStripSeparator1,
            this.addToolStripButton,
            this.deleteToolStripButton,
            this.toolStripSeparator2,
            this.changePwdToolStripButton,
            this.toolStripSeparator3,
            this.refreshToolStripButton});
			this.toolStrip1.Location = new System.Drawing.Point(0, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(215, 27);
			this.toolStrip1.TabIndex = 2;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// saveToolStripButton
			// 
			this.saveToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.saveToolStripButton.Enabled = false;
			this.saveToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("saveToolStripButton.Image")));
			this.saveToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.saveToolStripButton.Margin = new System.Windows.Forms.Padding(10, 1, 5, 2);
			this.saveToolStripButton.Name = "saveToolStripButton";
			this.saveToolStripButton.Size = new System.Drawing.Size(24, 24);
			this.saveToolStripButton.Text = "保存";
			this.saveToolStripButton.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 27);
			// 
			// addToolStripButton
			// 
			this.addToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.addToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("addToolStripButton.Image")));
			this.addToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.addToolStripButton.Margin = new System.Windows.Forms.Padding(10, 1, 0, 2);
			this.addToolStripButton.Name = "addToolStripButton";
			this.addToolStripButton.Size = new System.Drawing.Size(24, 24);
			this.addToolStripButton.Text = "添加";
			this.addToolStripButton.Click += new System.EventHandler(this.addToolStripMenuItem_Click);
			// 
			// deleteToolStripButton
			// 
			this.deleteToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.deleteToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("deleteToolStripButton.Image")));
			this.deleteToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.deleteToolStripButton.Margin = new System.Windows.Forms.Padding(5, 1, 5, 2);
			this.deleteToolStripButton.Name = "deleteToolStripButton";
			this.deleteToolStripButton.Size = new System.Drawing.Size(24, 24);
			this.deleteToolStripButton.Text = "删除";
			this.deleteToolStripButton.Click += new System.EventHandler(this.deleteToolStripMenuItem_Click);
			// 
			// toolStripSeparator2
			// 
			this.toolStripSeparator2.Name = "toolStripSeparator2";
			this.toolStripSeparator2.Size = new System.Drawing.Size(6, 27);
			// 
			// changePwdToolStripButton
			// 
			this.changePwdToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.changePwdToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("changePwdToolStripButton.Image")));
			this.changePwdToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.changePwdToolStripButton.Margin = new System.Windows.Forms.Padding(10, 1, 5, 2);
			this.changePwdToolStripButton.Name = "changePwdToolStripButton";
			this.changePwdToolStripButton.Size = new System.Drawing.Size(24, 24);
			this.changePwdToolStripButton.Text = "修改密码";
			this.changePwdToolStripButton.Click += new System.EventHandler(this.changePwdToolStripMenuItem_Click);
			// 
			// toolStripSeparator3
			// 
			this.toolStripSeparator3.Name = "toolStripSeparator3";
			this.toolStripSeparator3.Size = new System.Drawing.Size(6, 27);
			// 
			// refreshToolStripButton
			// 
			this.refreshToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.refreshToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("refreshToolStripButton.Image")));
			this.refreshToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.refreshToolStripButton.Margin = new System.Windows.Forms.Padding(10, 1, 5, 2);
			this.refreshToolStripButton.Name = "refreshToolStripButton";
			this.refreshToolStripButton.Size = new System.Drawing.Size(24, 24);
			this.refreshToolStripButton.Text = "刷新";
			this.refreshToolStripButton.Click += new System.EventHandler(this.refreshToolStripMenuItem_Click);
			// 
			// UserMgrForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(714, 429);
			this.Controls.Add(this.toolStrip1);
			this.Controls.Add(this.dataGridViewUser);
			this.Controls.Add(this.menuStrip1);
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "UserMgrForm";
			this.Text = "用户管理";
			this.Load += new System.EventHandler(this.UserMgrForm_Load);
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewUser)).EndInit();
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.DataGridView dataGridViewUser;
		private System.Windows.Forms.DataGridViewTextBoxColumn UserId;
		private System.Windows.Forms.DataGridViewTextBoxColumn LoginId;
		private System.Windows.Forms.DataGridViewTextBoxColumn UserName;
		private System.Windows.Forms.DataGridViewTextBoxColumn Password;
		private System.Windows.Forms.DataGridViewTextBoxColumn IDCard;
		private System.Windows.Forms.DataGridViewCheckBoxColumn Status;
		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem operationToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem addToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem changePwdToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
		private System.Windows.Forms.ToolStripMenuItem refreshToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
		private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripButton saveToolStripButton;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripButton addToolStripButton;
		private System.Windows.Forms.ToolStripButton deleteToolStripButton;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStripButton changePwdToolStripButton;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
		private System.Windows.Forms.ToolStripButton refreshToolStripButton;
	}
}