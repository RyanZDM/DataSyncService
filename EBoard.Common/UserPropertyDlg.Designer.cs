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
			this.buttonSave.Location = new System.Drawing.Point(240, 359);
			this.buttonSave.Name = "buttonSave";
			this.buttonSave.Size = new System.Drawing.Size(75, 23);
			this.buttonSave.TabIndex = 0;
			this.buttonSave.Text = "保存";
			this.buttonSave.UseVisualStyleBackColor = true;
			this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
			// 
			// buttonCancel
			// 
			this.buttonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.buttonCancel.Location = new System.Drawing.Point(159, 359);
			this.buttonCancel.Name = "buttonCancel";
			this.buttonCancel.Size = new System.Drawing.Size(75, 23);
			this.buttonCancel.TabIndex = 1;
			this.buttonCancel.Text = "取消";
			this.buttonCancel.UseVisualStyleBackColor = true;
			// 
			// tabControlProperty
			// 
			this.tabControlProperty.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tabControlProperty.Controls.Add(this.tabPageGeneral);
			this.tabControlProperty.Controls.Add(this.tabPageRole);
			this.tabControlProperty.Location = new System.Drawing.Point(9, 9);
			this.tabControlProperty.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.tabControlProperty.Name = "tabControlProperty";
			this.tabControlProperty.SelectedIndex = 0;
			this.tabControlProperty.Size = new System.Drawing.Size(309, 337);
			this.tabControlProperty.TabIndex = 2;
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
			this.tabPageGeneral.Location = new System.Drawing.Point(4, 22);
			this.tabPageGeneral.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.tabPageGeneral.Name = "tabPageGeneral";
			this.tabPageGeneral.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.tabPageGeneral.Size = new System.Drawing.Size(301, 311);
			this.tabPageGeneral.TabIndex = 0;
			this.tabPageGeneral.Text = "通用";
			this.tabPageGeneral.UseVisualStyleBackColor = true;
			// 
			// textBoxPassowrdConfirm
			// 
			this.textBoxPassowrdConfirm.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxPassowrdConfirm.Location = new System.Drawing.Point(91, 140);
			this.textBoxPassowrdConfirm.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.textBoxPassowrdConfirm.Name = "textBoxPassowrdConfirm";
			this.textBoxPassowrdConfirm.Size = new System.Drawing.Size(200, 21);
			this.textBoxPassowrdConfirm.TabIndex = 6;
			this.textBoxPassowrdConfirm.UseSystemPasswordChar = true;
			this.textBoxPassowrdConfirm.TextChanged += new System.EventHandler(this.textBox_TextChanged);
			// 
			// textBoxPassword
			// 
			this.textBoxPassword.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxPassword.Location = new System.Drawing.Point(91, 116);
			this.textBoxPassword.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.textBoxPassword.Name = "textBoxPassword";
			this.textBoxPassword.Size = new System.Drawing.Size(200, 21);
			this.textBoxPassword.TabIndex = 6;
			this.textBoxPassword.UseSystemPasswordChar = true;
			this.textBoxPassword.TextChanged += new System.EventHandler(this.textBox_TextChanged);
			// 
			// textBoxIDCard
			// 
			this.textBoxIDCard.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxIDCard.Location = new System.Drawing.Point(91, 93);
			this.textBoxIDCard.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.textBoxIDCard.Name = "textBoxIDCard";
			this.textBoxIDCard.Size = new System.Drawing.Size(200, 21);
			this.textBoxIDCard.TabIndex = 6;
			this.textBoxIDCard.TextChanged += new System.EventHandler(this.textBox_TextChanged);
			// 
			// textBoxUserName
			// 
			this.textBoxUserName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxUserName.Location = new System.Drawing.Point(91, 70);
			this.textBoxUserName.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.textBoxUserName.Name = "textBoxUserName";
			this.textBoxUserName.Size = new System.Drawing.Size(200, 21);
			this.textBoxUserName.TabIndex = 6;
			this.textBoxUserName.TextChanged += new System.EventHandler(this.textBox_TextChanged);
			// 
			// labelLoginId
			// 
			this.labelLoginId.AutoSize = true;
			this.labelLoginId.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.labelLoginId.Location = new System.Drawing.Point(44, 22);
			this.labelLoginId.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelLoginId.Name = "labelLoginId";
			this.labelLoginId.Size = new System.Drawing.Size(14, 20);
			this.labelLoginId.TabIndex = 5;
			this.labelLoginId.Text = "-";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(15, 95);
			this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(53, 12);
			this.label4.TabIndex = 4;
			this.label4.Text = "IDCard：";
			// 
			// checkBoxDisable
			// 
			this.checkBoxDisable.AutoSize = true;
			this.checkBoxDisable.Location = new System.Drawing.Point(15, 186);
			this.checkBoxDisable.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.checkBoxDisable.Name = "checkBoxDisable";
			this.checkBoxDisable.Size = new System.Drawing.Size(84, 16);
			this.checkBoxDisable.TabIndex = 3;
			this.checkBoxDisable.Text = "禁用该账号";
			this.checkBoxDisable.UseVisualStyleBackColor = true;
			this.checkBoxDisable.CheckedChanged += new System.EventHandler(this.checkBoxDisable_CheckedChanged);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(15, 142);
			this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(29, 12);
			this.label3.TabIndex = 2;
			this.label3.Text = "确认";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(15, 118);
			this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(41, 12);
			this.label2.TabIndex = 1;
			this.label2.Text = "密码：";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(15, 72);
			this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(53, 12);
			this.label1.TabIndex = 0;
			this.label1.Text = "用户名：";
			// 
			// tabPageRole
			// 
			this.tabPageRole.Controls.Add(this.dataGridViewRoles);
			this.tabPageRole.Location = new System.Drawing.Point(4, 22);
			this.tabPageRole.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.tabPageRole.Name = "tabPageRole";
			this.tabPageRole.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.tabPageRole.Size = new System.Drawing.Size(301, 311);
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
			this.dataGridViewRoles.Location = new System.Drawing.Point(2, 2);
			this.dataGridViewRoles.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.dataGridViewRoles.Name = "dataGridViewRoles";
			this.dataGridViewRoles.ReadOnly = true;
			this.dataGridViewRoles.RowTemplate.Height = 24;
			this.dataGridViewRoles.Size = new System.Drawing.Size(297, 307);
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
			this.buttonAdd.Location = new System.Drawing.Point(9, 359);
			this.buttonAdd.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.buttonAdd.Name = "buttonAdd";
			this.buttonAdd.Size = new System.Drawing.Size(56, 23);
			this.buttonAdd.TabIndex = 3;
			this.buttonAdd.Text = "添加";
			this.buttonAdd.UseVisualStyleBackColor = true;
			this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
			// 
			// buttonDelete
			// 
			this.buttonDelete.Location = new System.Drawing.Point(70, 359);
			this.buttonDelete.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.buttonDelete.Name = "buttonDelete";
			this.buttonDelete.Size = new System.Drawing.Size(56, 23);
			this.buttonDelete.TabIndex = 4;
			this.buttonDelete.Text = "删除";
			this.buttonDelete.UseVisualStyleBackColor = true;
			this.buttonDelete.Click += new System.EventHandler(this.buttonDelete_Click);
			// 
			// UserPropertyDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.buttonCancel;
			this.ClientSize = new System.Drawing.Size(327, 394);
			this.Controls.Add(this.buttonDelete);
			this.Controls.Add(this.buttonAdd);
			this.Controls.Add(this.tabControlProperty);
			this.Controls.Add(this.buttonCancel);
			this.Controls.Add(this.buttonSave);
			this.Name = "UserPropertyDlg";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "UserProperty";
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