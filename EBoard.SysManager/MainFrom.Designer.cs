namespace EBoard.SysManager
{
	partial class MainFrom
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
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.functionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.userMgrToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.dataMgrToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
			this.rptMgrToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
			this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.windowsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.tileHorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.tileVerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.cascadeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.menuStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.functionToolStripMenuItem,
            this.windowsToolStripMenuItem,
            this.helpToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Padding = new System.Windows.Forms.Padding(4, 2, 0, 2);
			this.menuStrip1.Size = new System.Drawing.Size(1070, 25);
			this.menuStrip1.TabIndex = 2;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// functionToolStripMenuItem
			// 
			this.functionToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.userMgrToolStripMenuItem,
            this.toolStripMenuItem1,
            this.dataMgrToolStripMenuItem,
            this.toolStripMenuItem2,
            this.rptMgrToolStripMenuItem,
            this.toolStripMenuItem3,
            this.exitToolStripMenuItem});
			this.functionToolStripMenuItem.Name = "functionToolStripMenuItem";
			this.functionToolStripMenuItem.Size = new System.Drawing.Size(50, 21);
			this.functionToolStripMenuItem.Text = "&F功能";
			// 
			// userMgrToolStripMenuItem
			// 
			this.userMgrToolStripMenuItem.Enabled = false;
			this.userMgrToolStripMenuItem.Name = "userMgrToolStripMenuItem";
			this.userMgrToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.userMgrToolStripMenuItem.Text = "用户管理";
			this.userMgrToolStripMenuItem.Click += new System.EventHandler(this.userMgrToolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(149, 6);
			// 
			// dataMgrToolStripMenuItem
			// 
			this.dataMgrToolStripMenuItem.Enabled = false;
			this.dataMgrToolStripMenuItem.Name = "dataMgrToolStripMenuItem";
			this.dataMgrToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.dataMgrToolStripMenuItem.Text = "基础数据维护";
			this.dataMgrToolStripMenuItem.Click += new System.EventHandler(this.dataMgrToolStripMenuItem_Click);
			// 
			// toolStripMenuItem2
			// 
			this.toolStripMenuItem2.Name = "toolStripMenuItem2";
			this.toolStripMenuItem2.Size = new System.Drawing.Size(149, 6);
			// 
			// rptMgrToolStripMenuItem
			// 
			this.rptMgrToolStripMenuItem.Enabled = false;
			this.rptMgrToolStripMenuItem.Name = "rptMgrToolStripMenuItem";
			this.rptMgrToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.rptMgrToolStripMenuItem.Text = " 报表管理";
			this.rptMgrToolStripMenuItem.Click += new System.EventHandler(this.rptMgrToolStripMenuItem_Click);
			// 
			// toolStripMenuItem3
			// 
			this.toolStripMenuItem3.Name = "toolStripMenuItem3";
			this.toolStripMenuItem3.Size = new System.Drawing.Size(149, 6);
			// 
			// exitToolStripMenuItem
			// 
			this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
			this.exitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.exitToolStripMenuItem.Text = "退出";
			this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
			// 
			// windowsToolStripMenuItem
			// 
			this.windowsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tileHorToolStripMenuItem,
            this.tileVerToolStripMenuItem,
            this.cascadeToolStripMenuItem});
			this.windowsToolStripMenuItem.Name = "windowsToolStripMenuItem";
			this.windowsToolStripMenuItem.Size = new System.Drawing.Size(73, 21);
			this.windowsToolStripMenuItem.Text = "Windows";
			// 
			// tileHorToolStripMenuItem
			// 
			this.tileHorToolStripMenuItem.Name = "tileHorToolStripMenuItem";
			this.tileHorToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
			this.tileHorToolStripMenuItem.Text = "水平平铺";
			this.tileHorToolStripMenuItem.Click += new System.EventHandler(this.tileHorToolStripMenuItem_Click);
			// 
			// tileVerToolStripMenuItem
			// 
			this.tileVerToolStripMenuItem.Name = "tileVerToolStripMenuItem";
			this.tileVerToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
			this.tileVerToolStripMenuItem.Text = "垂直平铺";
			this.tileVerToolStripMenuItem.Click += new System.EventHandler(this.tileVerToolStripMenuItem_Click);
			// 
			// cascadeToolStripMenuItem
			// 
			this.cascadeToolStripMenuItem.Name = "cascadeToolStripMenuItem";
			this.cascadeToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
			this.cascadeToolStripMenuItem.Text = "层叠排列";
			this.cascadeToolStripMenuItem.Click += new System.EventHandler(this.cascadeToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
			this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
			this.helpToolStripMenuItem.Size = new System.Drawing.Size(47, 21);
			this.helpToolStripMenuItem.Text = "Help";
			// 
			// aboutToolStripMenuItem
			// 
			this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
			this.aboutToolStripMenuItem.Size = new System.Drawing.Size(111, 22);
			this.aboutToolStripMenuItem.Text = "About";
			// 
			// MainFrom
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
			this.ClientSize = new System.Drawing.Size(1070, 687);
			this.Controls.Add(this.menuStrip1);
			this.IsMdiContainer = true;
			this.MainMenuStrip = this.menuStrip1;
			this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.Name = "MainFrom";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "系统管理";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem functionToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem windowsToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem userMgrToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem dataMgrToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
		private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem tileHorToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem tileVerToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem cascadeToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem rptMgrToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
	}
}

