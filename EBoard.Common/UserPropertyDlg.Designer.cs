namespace EBoard.Common
{
	partial class UserPropertyDlg
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
			this.buttonSave = new System.Windows.Forms.Button();
			this.buttonCancel = new System.Windows.Forms.Button();
			this.tabControlProperty = new System.Windows.Forms.TabControl();
			this.tabPageGeneral = new System.Windows.Forms.TabPage();
			this.textBoxPassowrdConfirm = new System.Windows.Forms.TextBox();
			this.textBoxPassword = new System.Windows.Forms.TextBox();
			this.textBoxIDCard = new System.Windows.Forms.TextBox();
			this.textBoxUserName = new System.Windows.Forms.TextBox();
			this.labelLoginId = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.checkBoxDisable = new System.Windows.Forms.CheckBox();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.tabPageRole = new System.Windows.Forms.TabPage();
			this.dataGridViewRoles = new System.Windows.Forms.DataGridView();
			this.UserId = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RoleId = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RoleName = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.buttonAdd = new System.Windows.Forms.Button();
			this.buttonDelete = new System.Windows.Forms.Button();
			this.tabControlProperty.SuspendLayout();
			this.tabPageGeneral.SuspendLayout();
			this.tabPageRole.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewRoles)).BeginInit();
			this.SuspendLayout();
			// 
			// buttonSave
			// 
			this.buttonSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonSave.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.buttonSave.Enabled = false;
			this.buttonSave.Location = new System.Drawing.Point(337, 479);
			this.buttonSave.Margin = new System.Windows.Forms.Padding(4);
			this.buttonSave.Name = "buttonSave";
			this.buttonSave.Size = new System.Drawing.Size(90, 31);
			this.buttonSave.TabIndex = 0;
			this.buttonSave.Text = "保存";
			this.buttonSave.UseVisualStyleBackColor = true;
			this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
			// 
			// buttonCancel
			// 
			this.buttonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.buttonCancel.Location = new System.Drawing.Point(12, 479);
			this.buttonCancel.Margin = new System.Windows.Forms.Padding(4);
			this.buttonCancel.Name = "buttonCancel";
			this.buttonCancel.Size = new System.Drawing.Size(100, 31);
			this.buttonCancel.TabIndex = 1;
			this.buttonCancel.Text = "退出";
			this.buttonCancel.UseVisualStyleBackColor = true;
			// 
			// tabControlProperty
			// 
			this.tabControlProperty.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tabControlProperty.Controls.Add(this.tabPageGeneral);
			this.tabControlProperty.Controls.Add(this.tabPageRole);
			this.tabControlProperty.Location = new System.Drawing.Point(12, 12);
			this.tabControlProperty.Name = "tabControlProperty";
			this.tabControlProperty.SelectedIndex = 0;
			this.tabControlProperty.Size = new System.Drawing.Size(412, 449);
			this.tabControlProperty.TabIndex = 2;
			this.tabControlProperty.SelectedIndexChanged += new System.EventHandler(this.tabControlProperty_SelectedIndexChanged);
			// 
			// tabPageGeneral
			// 
			this.tabPageGeneral.Controls.Add(this.textBoxPassowrdConfirm);
			this.tabPageGeneral.Controls.Add(this.textBoxPassword);
			this.tabPageGeneral.Controls.Add(this.textBoxIDCard);
			this.tabPageGeneral.Controls.Add(this.textBoxUserName);
			this.tabPageGeneral.Controls.Add(this.labelLoginId);
			this.tabPageGeneral.Controls.Add(this.label4);
			this.tabPageGeneral.Controls.Add(this.checkBoxDisable);
			this.tabPageGeneral.Controls.Add(this.label3);
			this.tabPageGeneral.Controls.Add(this.label2);
			this.tabPageGeneral.Controls.Add(this.label1);
			this.tabPageGeneral.Location = new System.Drawing.Point(4, 25);
			this.tabPageGeneral.Name = "tabPageGeneral";
			this.tabPageGeneral.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageGeneral.Size = new System.Drawing.Size(404, 420);
			this.tabPageGeneral.TabIndex = 0;
			this.tabPageGeneral.Text = "通用";
			this.tabPageGeneral.UseVisualStyleBackColor = true;
			// 
			// textBoxPassowrdConfirm
			// 
			this.textBoxPassowrdConfirm.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxPassowrdConfirm.Location = new System.Drawing.Point(121, 187);
			this.textBoxPassowrdConfirm.Name = "textBoxPassowrdConfirm";
			this.textBoxPassowrdConfirm.Size = new System.Drawing.Size(265, 22);
			this.textBoxPassowrdConfirm.TabIndex = 6;
			this.textBoxPassowrdConfirm.UseSystemPasswordChar = true;			
			// 
			// textBoxPassword
			// 
			this.textBoxPassword.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxPassword.Location = new System.Drawing.Point(121, 155);
			this.textBoxPassword.Name = "textBoxPassword";
			this.textBoxPassword.Size = new System.Drawing.Size(265, 22);
			this.textBoxPassword.TabIndex = 6;
			this.textBoxPassword.UseSystemPasswordChar = true;
			// 
			// textBoxIDCard
			// 
			this.textBoxIDCard.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxIDCard.Location = new System.Drawing.Point(121, 124);
			this.textBoxIDCard.Name = "textBoxIDCard";
			this.textBoxIDCard.Size = new System.Drawing.Size(265, 22);
			this.textBoxIDCard.TabIndex = 6;
			// 
			// textBoxUserName
			// 
			this.textBoxUserName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxUserName.Location = new System.Drawing.Point(121, 93);
			this.textBoxUserName.Name = "textBoxUserName";
			this.textBoxUserName.Size = new System.Drawing.Size(265, 22);
			this.textBoxUserName.TabIndex = 6;
			// 
			// labelLoginId
			// 
			this.labelLoginId.AutoSize = true;
			this.labelLoginId.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.labelLoginId.Location = new System.Drawing.Point(59, 29);
			this.labelLoginId.Name = "labelLoginId";
			this.labelLoginId.Size = new System.Drawing.Size(19, 25);
			this.labelLoginId.TabIndex = 5;
			this.labelLoginId.Text = "-";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(20, 127);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(65, 17);
			this.label4.TabIndex = 4;
			this.label4.Text = "IDCard：";
			// 
			// checkBoxDisable
			// 
			this.checkBoxDisable.AutoSize = true;
			this.checkBoxDisable.Location = new System.Drawing.Point(20, 248);
			this.checkBoxDisable.Name = "checkBoxDisable";
			this.checkBoxDisable.Size = new System.Drawing.Size(100, 21);
			this.checkBoxDisable.TabIndex = 3;
			this.checkBoxDisable.Text = "禁用该账号";
			this.checkBoxDisable.UseVisualStyleBackColor = true;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(20, 189);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(36, 17);
			this.label3.TabIndex = 2;
			this.label3.Text = "确认";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(20, 157);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(50, 17);
			this.label2.TabIndex = 1;
			this.label2.Text = "密码：";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(20, 96);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(64, 17);
			this.label1.TabIndex = 0;
			this.label1.Text = "用户名：";
			// 
			// tabPageRole
			// 
			this.tabPageRole.Controls.Add(this.dataGridViewRoles);
			this.tabPageRole.Location = new System.Drawing.Point(4, 25);
			this.tabPageRole.Name = "tabPageRole";
			this.tabPageRole.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageRole.Size = new System.Drawing.Size(404, 420);
			this.tabPageRole.TabIndex = 1;
			this.tabPageRole.Text = "角色";
			this.tabPageRole.UseVisualStyleBackColor = true;
			// 
			// dataGridViewRoles
			// 
			this.dataGridViewRoles.AllowUserToAddRows = false;
			this.dataGridViewRoles.AllowUserToDeleteRows = false;
			this.dataGridViewRoles.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridViewRoles.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.UserId,
            this.RoleId,
            this.RoleName});
			this.dataGridViewRoles.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dataGridViewRoles.Location = new System.Drawing.Point(3, 3);
			this.dataGridViewRoles.Name = "dataGridViewRoles";
			this.dataGridViewRoles.ReadOnly = true;
			this.dataGridViewRoles.RowTemplate.Height = 24;
			this.dataGridViewRoles.Size = new System.Drawing.Size(398, 414);
			this.dataGridViewRoles.TabIndex = 0;
			// 
			// UserId
			// 
			this.UserId.DataPropertyName = "UserId";
			this.UserId.HeaderText = "UserId";
			this.UserId.Name = "UserId";
			this.UserId.ReadOnly = true;
			this.UserId.Visible = false;
			// 
			// RoleId
			// 
			this.RoleId.DataPropertyName = "RoleId";
			this.RoleId.HeaderText = "角色编号";
			this.RoleId.Name = "RoleId";
			this.RoleId.ReadOnly = true;
			this.RoleId.Width = 120;
			// 
			// RoleName
			// 
			this.RoleName.DataPropertyName = "Name";
			this.RoleName.HeaderText = "名称";
			this.RoleName.Name = "RoleName";
			this.RoleName.ReadOnly = true;
			this.RoleName.Width = 210;
			// 
			// buttonAdd
			// 
			this.buttonAdd.Location = new System.Drawing.Point(157, 479);
			this.buttonAdd.Name = "buttonAdd";
			this.buttonAdd.Size = new System.Drawing.Size(90, 31);
			this.buttonAdd.TabIndex = 3;
			this.buttonAdd.Text = "添加";
			this.buttonAdd.UseVisualStyleBackColor = true;
			this.buttonAdd.Visible = false;
			this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
			// 
			// buttonDelete
			// 
			this.buttonDelete.Location = new System.Drawing.Point(247, 479);
			this.buttonDelete.Name = "buttonDelete";
			this.buttonDelete.Size = new System.Drawing.Size(90, 31);
			this.buttonDelete.TabIndex = 4;
			this.buttonDelete.Text = "删除";
			this.buttonDelete.UseVisualStyleBackColor = true;
			this.buttonDelete.Visible = false;
			this.buttonDelete.Click += new System.EventHandler(this.buttonDelete_Click);
			// 
			// UserPropertyDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.buttonCancel;
			this.ClientSize = new System.Drawing.Size(436, 525);
			this.Controls.Add(this.buttonDelete);
			this.Controls.Add(this.buttonAdd);
			this.Controls.Add(this.tabControlProperty);
			this.Controls.Add(this.buttonCancel);
			this.Controls.Add(this.buttonSave);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(4);
			this.Name = "UserPropertyDlg";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "UserProperty";
			this.Load += new System.EventHandler(this.UserPropertyDlg_Load);
			this.tabControlProperty.ResumeLayout(false);
			this.tabPageGeneral.ResumeLayout(false);
			this.tabPageGeneral.PerformLayout();
			this.tabPageRole.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewRoles)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button buttonSave;
		private System.Windows.Forms.Button buttonCancel;
		private System.Windows.Forms.TabControl tabControlProperty;
		private System.Windows.Forms.TabPage tabPageGeneral;
		private System.Windows.Forms.TabPage tabPageRole;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.CheckBox checkBoxDisable;
		private System.Windows.Forms.Label labelLoginId;
		private System.Windows.Forms.TextBox textBoxPassowrdConfirm;
		private System.Windows.Forms.TextBox textBoxPassword;
		private System.Windows.Forms.TextBox textBoxIDCard;
		private System.Windows.Forms.TextBox textBoxUserName;
		private System.Windows.Forms.DataGridView dataGridViewRoles;
		private System.Windows.Forms.DataGridViewTextBoxColumn UserId;
		private System.Windows.Forms.DataGridViewTextBoxColumn RoleId;
		private System.Windows.Forms.DataGridViewTextBoxColumn RoleName;
		private System.Windows.Forms.Button buttonAdd;
		private System.Windows.Forms.Button buttonDelete;
	}
}