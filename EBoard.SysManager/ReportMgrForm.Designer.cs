namespace EBoard.SysManager
{
	partial class ReportMgrForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ReportMgrForm));
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.operationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.refreshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.toolStripButtonRefresh = new System.Windows.Forms.ToolStripButton();
			this.splitContainerLR1 = new System.Windows.Forms.SplitContainer();
			this.treeView1 = new System.Windows.Forms.TreeView();
			this.splitContainerUD = new System.Windows.Forms.SplitContainer();
			this.splitContainerMstr = new System.Windows.Forms.SplitContainer();
			this.labelGeneral = new System.Windows.Forms.Label();
			this.dataGridViewMstr = new System.Windows.Forms.DataGridView();
			this.item = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.value = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.buttonOpenFile = new System.Windows.Forms.Button();
			this.buttonCreateLastMonth = new System.Windows.Forms.Button();
			this.buttonCreateFile = new System.Windows.Forms.Button();
			this.splitContainerLRDet = new System.Windows.Forms.SplitContainer();
			this.labelShiftDet = new System.Windows.Forms.Label();
			this.dataGridViewReportDet = new System.Windows.Forms.DataGridView();
			this.ItemId = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ItemValue = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.labelWorkerDet = new System.Windows.Forms.Label();
			this.dataGridViewWorkerReport = new System.Windows.Forms.DataGridView();
			this.ItemName = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.SubtotalValue = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.menuStrip1.SuspendLayout();
			this.toolStrip1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainerLR1)).BeginInit();
			this.splitContainerLR1.Panel1.SuspendLayout();
			this.splitContainerLR1.Panel2.SuspendLayout();
			this.splitContainerLR1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainerUD)).BeginInit();
			this.splitContainerUD.Panel1.SuspendLayout();
			this.splitContainerUD.Panel2.SuspendLayout();
			this.splitContainerUD.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainerMstr)).BeginInit();
			this.splitContainerMstr.Panel1.SuspendLayout();
			this.splitContainerMstr.Panel2.SuspendLayout();
			this.splitContainerMstr.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewMstr)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.splitContainerLRDet)).BeginInit();
			this.splitContainerLRDet.Panel1.SuspendLayout();
			this.splitContainerLRDet.Panel2.SuspendLayout();
			this.splitContainerLRDet.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewReportDet)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewWorkerReport)).BeginInit();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.menuStrip1.Dock = System.Windows.Forms.DockStyle.None;
			this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.operationToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(1245, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Padding = new System.Windows.Forms.Padding(8, 2, 0, 2);
			this.menuStrip1.Size = new System.Drawing.Size(59, 27);
			this.menuStrip1.TabIndex = 0;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// operationToolStripMenuItem
			// 
			this.operationToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.refreshToolStripMenuItem});
			this.operationToolStripMenuItem.MergeAction = System.Windows.Forms.MergeAction.Insert;
			this.operationToolStripMenuItem.MergeIndex = 1;
			this.operationToolStripMenuItem.Name = "operationToolStripMenuItem";
			this.operationToolStripMenuItem.Size = new System.Drawing.Size(49, 23);
			this.operationToolStripMenuItem.Text = "操作";
			// 
			// refreshToolStripMenuItem
			// 
			this.refreshToolStripMenuItem.Name = "refreshToolStripMenuItem";
			this.refreshToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F5;
			this.refreshToolStripMenuItem.Size = new System.Drawing.Size(136, 26);
			this.refreshToolStripMenuItem.Text = "刷新";
			this.refreshToolStripMenuItem.Click += new System.EventHandler(this.refreshToolStripMenuItem_Click);
			// 
			// toolStrip1
			// 
			this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
			this.toolStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButtonRefresh});
			this.toolStrip1.Location = new System.Drawing.Point(1, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(36, 27);
			this.toolStrip1.TabIndex = 1;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// toolStripButtonRefresh
			// 
			this.toolStripButtonRefresh.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripButtonRefresh.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonRefresh.Image")));
			this.toolStripButtonRefresh.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripButtonRefresh.Name = "toolStripButtonRefresh";
			this.toolStripButtonRefresh.Size = new System.Drawing.Size(24, 24);
			this.toolStripButtonRefresh.Text = "刷新";
			this.toolStripButtonRefresh.Click += new System.EventHandler(this.refreshToolStripMenuItem_Click);
			// 
			// splitContainerLR1
			// 
			this.splitContainerLR1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.splitContainerLR1.Location = new System.Drawing.Point(1, 38);
			this.splitContainerLR1.Margin = new System.Windows.Forms.Padding(4);
			this.splitContainerLR1.Name = "splitContainerLR1";
			// 
			// splitContainerLR1.Panel1
			// 
			this.splitContainerLR1.Panel1.Controls.Add(this.treeView1);
			// 
			// splitContainerLR1.Panel2
			// 
			this.splitContainerLR1.Panel2.Controls.Add(this.splitContainerUD);
			this.splitContainerLR1.Size = new System.Drawing.Size(1302, 636);
			this.splitContainerLR1.SplitterDistance = 207;
			this.splitContainerLR1.SplitterWidth = 5;
			this.splitContainerLR1.TabIndex = 2;
			// 
			// treeView1
			// 
			this.treeView1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.treeView1.Location = new System.Drawing.Point(0, 0);
			this.treeView1.Margin = new System.Windows.Forms.Padding(4);
			this.treeView1.Name = "treeView1";
			this.treeView1.Size = new System.Drawing.Size(207, 636);
			this.treeView1.TabIndex = 0;
			this.treeView1.BeforeSelect += new System.Windows.Forms.TreeViewCancelEventHandler(this.treeView1_BeforeSelect);
			this.treeView1.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView1_AfterSelect);
			// 
			// splitContainerUD
			// 
			this.splitContainerUD.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainerUD.Location = new System.Drawing.Point(0, 0);
			this.splitContainerUD.Margin = new System.Windows.Forms.Padding(4);
			this.splitContainerUD.Name = "splitContainerUD";
			this.splitContainerUD.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainerUD.Panel1
			// 
			this.splitContainerUD.Panel1.Controls.Add(this.splitContainerMstr);
			// 
			// splitContainerUD.Panel2
			// 
			this.splitContainerUD.Panel2.Controls.Add(this.splitContainerLRDet);
			this.splitContainerUD.Size = new System.Drawing.Size(1090, 636);
			this.splitContainerUD.SplitterDistance = 199;
			this.splitContainerUD.SplitterWidth = 5;
			this.splitContainerUD.TabIndex = 0;
			// 
			// splitContainerMstr
			// 
			this.splitContainerMstr.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainerMstr.Location = new System.Drawing.Point(0, 0);
			this.splitContainerMstr.Margin = new System.Windows.Forms.Padding(4);
			this.splitContainerMstr.Name = "splitContainerMstr";
			// 
			// splitContainerMstr.Panel1
			// 
			this.splitContainerMstr.Panel1.Controls.Add(this.labelGeneral);
			this.splitContainerMstr.Panel1.Controls.Add(this.dataGridViewMstr);
			// 
			// splitContainerMstr.Panel2
			// 
			this.splitContainerMstr.Panel2.Controls.Add(this.buttonOpenFile);
			this.splitContainerMstr.Panel2.Controls.Add(this.buttonCreateLastMonth);
			this.splitContainerMstr.Panel2.Controls.Add(this.buttonCreateFile);
			this.splitContainerMstr.Size = new System.Drawing.Size(1090, 199);
			this.splitContainerMstr.SplitterDistance = 928;
			this.splitContainerMstr.SplitterWidth = 5;
			this.splitContainerMstr.TabIndex = 0;
			// 
			// labelGeneral
			// 
			this.labelGeneral.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.labelGeneral.BackColor = System.Drawing.Color.Gray;
			this.labelGeneral.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.labelGeneral.Location = new System.Drawing.Point(0, 0);
			this.labelGeneral.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelGeneral.Name = "labelGeneral";
			this.labelGeneral.Size = new System.Drawing.Size(928, 38);
			this.labelGeneral.TabIndex = 1;
			this.labelGeneral.Text = "  月报信息";
			this.labelGeneral.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// dataGridViewMstr
			// 
			this.dataGridViewMstr.AllowUserToAddRows = false;
			this.dataGridViewMstr.AllowUserToDeleteRows = false;
			this.dataGridViewMstr.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.dataGridViewMstr.BackgroundColor = System.Drawing.SystemColors.ButtonFace;
			this.dataGridViewMstr.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.dataGridViewMstr.ColumnHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Sunken;
			this.dataGridViewMstr.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridViewMstr.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.item,
            this.value});
			this.dataGridViewMstr.Location = new System.Drawing.Point(0, 41);
			this.dataGridViewMstr.Margin = new System.Windows.Forms.Padding(4);
			this.dataGridViewMstr.Name = "dataGridViewMstr";
			this.dataGridViewMstr.ReadOnly = true;
			this.dataGridViewMstr.RowTemplate.Height = 23;
			this.dataGridViewMstr.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
			this.dataGridViewMstr.Size = new System.Drawing.Size(928, 158);
			this.dataGridViewMstr.TabIndex = 0;
			// 
			// item
			// 
			this.item.HeaderText = "";
			this.item.Name = "item";
			this.item.ReadOnly = true;
			this.item.Resizable = System.Windows.Forms.DataGridViewTriState.False;
			this.item.Width = 150;
			// 
			// value
			// 
			this.value.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
			this.value.HeaderText = "";
			this.value.Name = "value";
			this.value.ReadOnly = true;
			this.value.Resizable = System.Windows.Forms.DataGridViewTriState.False;
			// 
			// buttonOpenFile
			// 
			this.buttonOpenFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonOpenFile.Location = new System.Drawing.Point(23, 138);
			this.buttonOpenFile.Margin = new System.Windows.Forms.Padding(4);
			this.buttonOpenFile.Name = "buttonOpenFile";
			this.buttonOpenFile.Size = new System.Drawing.Size(118, 29);
			this.buttonOpenFile.TabIndex = 1;
			this.buttonOpenFile.Text = "打开Excel";
			this.buttonOpenFile.UseVisualStyleBackColor = true;
			this.buttonOpenFile.Click += new System.EventHandler(this.buttonOpenFile_Click);
			// 
			// buttonCreateLastMonth
			// 
			this.buttonCreateLastMonth.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonCreateLastMonth.Enabled = false;
			this.buttonCreateLastMonth.Location = new System.Drawing.Point(23, 12);
			this.buttonCreateLastMonth.Margin = new System.Windows.Forms.Padding(4);
			this.buttonCreateLastMonth.Name = "buttonCreateLastMonth";
			this.buttonCreateLastMonth.Size = new System.Drawing.Size(118, 29);
			this.buttonCreateLastMonth.TabIndex = 0;
			this.buttonCreateLastMonth.Text = "创建上月报表";
			this.buttonCreateLastMonth.UseVisualStyleBackColor = true;
			this.buttonCreateLastMonth.Click += new System.EventHandler(this.buttonCreateLastMonth_Click);
			// 
			// buttonCreateFile
			// 
			this.buttonCreateFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonCreateFile.Location = new System.Drawing.Point(23, 102);
			this.buttonCreateFile.Margin = new System.Windows.Forms.Padding(4);
			this.buttonCreateFile.Name = "buttonCreateFile";
			this.buttonCreateFile.Size = new System.Drawing.Size(118, 29);
			this.buttonCreateFile.TabIndex = 0;
			this.buttonCreateFile.Text = "创建Excel";
			this.buttonCreateFile.UseVisualStyleBackColor = true;
			this.buttonCreateFile.Click += new System.EventHandler(this.buttonCreateFile_Click);
			// 
			// splitContainerLRDet
			// 
			this.splitContainerLRDet.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainerLRDet.Location = new System.Drawing.Point(0, 0);
			this.splitContainerLRDet.Margin = new System.Windows.Forms.Padding(4);
			this.splitContainerLRDet.Name = "splitContainerLRDet";
			// 
			// splitContainerLRDet.Panel1
			// 
			this.splitContainerLRDet.Panel1.Controls.Add(this.labelShiftDet);
			this.splitContainerLRDet.Panel1.Controls.Add(this.dataGridViewReportDet);
			// 
			// splitContainerLRDet.Panel2
			// 
			this.splitContainerLRDet.Panel2.Controls.Add(this.labelWorkerDet);
			this.splitContainerLRDet.Panel2.Controls.Add(this.dataGridViewWorkerReport);
			this.splitContainerLRDet.Size = new System.Drawing.Size(1090, 432);
			this.splitContainerLRDet.SplitterDistance = 526;
			this.splitContainerLRDet.SplitterWidth = 5;
			this.splitContainerLRDet.TabIndex = 0;
			// 
			// labelShiftDet
			// 
			this.labelShiftDet.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.labelShiftDet.BackColor = System.Drawing.Color.Gray;
			this.labelShiftDet.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.labelShiftDet.Location = new System.Drawing.Point(0, 0);
			this.labelShiftDet.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelShiftDet.Name = "labelShiftDet";
			this.labelShiftDet.Size = new System.Drawing.Size(526, 36);
			this.labelShiftDet.TabIndex = 1;
			this.labelShiftDet.Text = "  按工班统计";
			this.labelShiftDet.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// dataGridViewReportDet
			// 
			this.dataGridViewReportDet.AllowUserToAddRows = false;
			this.dataGridViewReportDet.AllowUserToDeleteRows = false;
			this.dataGridViewReportDet.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.dataGridViewReportDet.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridViewReportDet.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ItemId,
            this.ItemValue});
			this.dataGridViewReportDet.Location = new System.Drawing.Point(0, 40);
			this.dataGridViewReportDet.Margin = new System.Windows.Forms.Padding(4);
			this.dataGridViewReportDet.Name = "dataGridViewReportDet";
			this.dataGridViewReportDet.ReadOnly = true;
			this.dataGridViewReportDet.RowTemplate.Height = 23;
			this.dataGridViewReportDet.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
			this.dataGridViewReportDet.Size = new System.Drawing.Size(526, 392);
			this.dataGridViewReportDet.TabIndex = 0;
			// 
			// ItemId
			// 
			this.ItemId.HeaderText = "项目";
			this.ItemId.Name = "ItemId";
			this.ItemId.ReadOnly = true;
			this.ItemId.Width = 200;
			// 
			// ItemValue
			// 
			this.ItemValue.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
			this.ItemValue.HeaderText = "累计值";
			this.ItemValue.Name = "ItemValue";
			this.ItemValue.ReadOnly = true;
			// 
			// labelWorkerDet
			// 
			this.labelWorkerDet.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.labelWorkerDet.BackColor = System.Drawing.Color.Gray;
			this.labelWorkerDet.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.labelWorkerDet.Location = new System.Drawing.Point(0, 0);
			this.labelWorkerDet.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelWorkerDet.Name = "labelWorkerDet";
			this.labelWorkerDet.Size = new System.Drawing.Size(558, 36);
			this.labelWorkerDet.TabIndex = 1;
			this.labelWorkerDet.Text = "  按值班人员统计";
			this.labelWorkerDet.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// dataGridViewWorkerReport
			// 
			this.dataGridViewWorkerReport.AllowUserToAddRows = false;
			this.dataGridViewWorkerReport.AllowUserToDeleteRows = false;
			this.dataGridViewWorkerReport.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.dataGridViewWorkerReport.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridViewWorkerReport.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ItemName,
            this.SubtotalValue});
			this.dataGridViewWorkerReport.Location = new System.Drawing.Point(0, 40);
			this.dataGridViewWorkerReport.Margin = new System.Windows.Forms.Padding(4);
			this.dataGridViewWorkerReport.Name = "dataGridViewWorkerReport";
			this.dataGridViewWorkerReport.ReadOnly = true;
			this.dataGridViewWorkerReport.RowTemplate.Height = 23;
			this.dataGridViewWorkerReport.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
			this.dataGridViewWorkerReport.Size = new System.Drawing.Size(558, 392);
			this.dataGridViewWorkerReport.TabIndex = 0;
			// 
			// ItemName
			// 
			this.ItemName.HeaderText = "项目";
			this.ItemName.Name = "ItemName";
			this.ItemName.ReadOnly = true;
			this.ItemName.Width = 200;
			// 
			// SubtotalValue
			// 
			this.SubtotalValue.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
			this.SubtotalValue.HeaderText = "累计值";
			this.SubtotalValue.Name = "SubtotalValue";
			this.SubtotalValue.ReadOnly = true;
			// 
			// ReportMgrForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(120F, 120F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
			this.ClientSize = new System.Drawing.Size(1302, 674);
			this.Controls.Add(this.splitContainerLR1);
			this.Controls.Add(this.toolStrip1);
			this.Controls.Add(this.menuStrip1);
			this.MainMenuStrip = this.menuStrip1;
			this.Margin = new System.Windows.Forms.Padding(4);
			this.Name = "ReportMgrForm";
			this.Text = "报表管理";
			this.Load += new System.EventHandler(this.ReportMgrForm_Load);
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.splitContainerLR1.Panel1.ResumeLayout(false);
			this.splitContainerLR1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainerLR1)).EndInit();
			this.splitContainerLR1.ResumeLayout(false);
			this.splitContainerUD.Panel1.ResumeLayout(false);
			this.splitContainerUD.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainerUD)).EndInit();
			this.splitContainerUD.ResumeLayout(false);
			this.splitContainerMstr.Panel1.ResumeLayout(false);
			this.splitContainerMstr.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainerMstr)).EndInit();
			this.splitContainerMstr.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewMstr)).EndInit();
			this.splitContainerLRDet.Panel1.ResumeLayout(false);
			this.splitContainerLRDet.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainerLRDet)).EndInit();
			this.splitContainerLRDet.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewReportDet)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewWorkerReport)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem operationToolStripMenuItem;
		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripButton toolStripButtonRefresh;
		private System.Windows.Forms.SplitContainer splitContainerLR1;
		private System.Windows.Forms.TreeView treeView1;
		private System.Windows.Forms.SplitContainer splitContainerUD;
		private System.Windows.Forms.ToolStripMenuItem refreshToolStripMenuItem;
		private System.Windows.Forms.SplitContainer splitContainerLRDet;
		private System.Windows.Forms.DataGridView dataGridViewReportDet;
		private System.Windows.Forms.DataGridView dataGridViewWorkerReport;
		private System.Windows.Forms.SplitContainer splitContainerMstr;
		private System.Windows.Forms.DataGridView dataGridViewMstr;
		private System.Windows.Forms.DataGridViewTextBoxColumn item;
		private System.Windows.Forms.DataGridViewTextBoxColumn value;
		private System.Windows.Forms.Button buttonOpenFile;
		private System.Windows.Forms.Button buttonCreateFile;
		private System.Windows.Forms.DataGridViewTextBoxColumn ItemId;
		private System.Windows.Forms.DataGridViewTextBoxColumn ItemValue;
		private System.Windows.Forms.DataGridViewTextBoxColumn ItemName;
		private System.Windows.Forms.DataGridViewTextBoxColumn SubtotalValue;
		private System.Windows.Forms.Label labelShiftDet;
		private System.Windows.Forms.Label labelWorkerDet;
		private System.Windows.Forms.Label labelGeneral;
		private System.Windows.Forms.Button buttonCreateLastMonth;
	}
}