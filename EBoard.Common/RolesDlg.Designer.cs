namespace EBoard.Common
{
	partial class RolesDlg
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
			this.dataGridViewRole = new System.Windows.Forms.DataGridView();
			this.buttonOK = new System.Windows.Forms.Button();
			this.buttonCancel = new System.Windows.Forms.Button();
			this.RoleId = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.RoleName = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Status = new System.Windows.Forms.DataGridViewTextBoxColumn();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewRole)).BeginInit();
			this.SuspendLayout();
			// 
			// dataGridViewRole
			// 
			this.dataGridViewRole.AllowUserToAddRows = false;
			this.dataGridViewRole.AllowUserToDeleteRows = false;
			this.dataGridViewRole.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.dataGridViewRole.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridViewRole.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.RoleId,
            this.RoleName,
            this.Status});
			this.dataGridViewRole.Location = new System.Drawing.Point(12, 12);
			this.dataGridViewRole.Name = "dataGridViewRole";
			this.dataGridViewRole.ReadOnly = true;
			this.dataGridViewRole.RowTemplate.Height = 24;
			this.dataGridViewRole.Size = new System.Drawing.Size(324, 280);
			this.dataGridViewRole.TabIndex = 0;
			this.dataGridViewRole.DoubleClick += new System.EventHandler(this.buttonOK_Click);
			// 
			// buttonOK
			// 
			this.buttonOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.buttonOK.Location = new System.Drawing.Point(231, 311);
			this.buttonOK.Name = "buttonOK";
			this.buttonOK.Size = new System.Drawing.Size(105, 38);
			this.buttonOK.TabIndex = 1;
			this.buttonOK.Text = "确定";
			this.buttonOK.UseVisualStyleBackColor = true;
			this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
			// 
			// buttonCancel
			// 
			this.buttonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.buttonCancel.Location = new System.Drawing.Point(121, 311);
			this.buttonCancel.Name = "buttonCancel";
			this.buttonCancel.Size = new System.Drawing.Size(105, 38);
			this.buttonCancel.TabIndex = 2;
			this.buttonCancel.Text = "取消";
			this.buttonCancel.UseVisualStyleBackColor = true;
			// 
			// RoleId
			// 
			this.RoleId.DataPropertyName = "RoleId";
			this.RoleId.HeaderText = "角色ID";
			this.RoleId.Name = "RoleId";
			this.RoleId.ReadOnly = true;
			// 
			// RoleName
			// 
			this.RoleName.DataPropertyName = "Name";
			this.RoleName.HeaderText = "描述";
			this.RoleName.Name = "RoleName";
			this.RoleName.ReadOnly = true;
			this.RoleName.Width = 200;
			// 
			// Status
			// 
			this.Status.DataPropertyName = "Status";
			this.Status.HeaderText = "Status";
			this.Status.Name = "Status";
			this.Status.ReadOnly = true;
			this.Status.Visible = false;
			// 
			// RolesDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.buttonCancel;
			this.ClientSize = new System.Drawing.Size(348, 361);
			this.Controls.Add(this.buttonCancel);
			this.Controls.Add(this.buttonOK);
			this.Controls.Add(this.dataGridViewRole);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Name = "RolesDlg";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "角色";
			this.Load += new System.EventHandler(this.RolesDlg_Load);
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewRole)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.DataGridView dataGridViewRole;
		private System.Windows.Forms.Button buttonOK;
		private System.Windows.Forms.Button buttonCancel;
		private System.Windows.Forms.DataGridViewTextBoxColumn RoleId;
		private System.Windows.Forms.DataGridViewTextBoxColumn RoleName;
		private System.Windows.Forms.DataGridViewTextBoxColumn Status;
	}
}