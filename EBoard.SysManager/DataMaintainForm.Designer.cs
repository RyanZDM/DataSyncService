namespace EBoard.SysManager
{
	partial class DataMaintainForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DataMaintainForm));
			System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("监控项");
			System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("通用参数");
			System.Windows.Forms.TreeNode treeNode6 = new System.Windows.Forms.TreeNode("基础数据", new System.Windows.Forms.TreeNode[] {
            treeNode4,
            treeNode5});
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
			System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.operationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.addToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.refreshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
			this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.toolStripButtonSave = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.toolStripButtonAdd = new System.Windows.Forms.ToolStripButton();
			this.toolStripButtonDelete = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this.toolStripButtonRefresh = new System.Windows.Forms.ToolStripButton();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.treeView1 = new System.Windows.Forms.TreeView();
			this.panelGeneralParams = new System.Windows.Forms.Panel();
			this.dataGridViewGeneralParams = new System.Windows.Forms.DataGridView();
			this.dataGridViewMonitorItem = new System.Windows.Forms.DataGridView();
			this.ItemId = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Address = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.DisplayName = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.DataType = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.NeedAccumulate = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.UpdateHistory = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.InConverter = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.OutConverter = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Status = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.label1 = new System.Windows.Forms.Label();
			this.comboBoxCategory = new System.Windows.Forms.ComboBox();
			this.Category = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ItemName = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Value = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.DispOrder = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Memo = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.Hide = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.IsEncrypted = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.IsProtected = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.menuStrip1.SuspendLayout();
			this.toolStrip1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this.panelGeneralParams.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewGeneralParams)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewMonitorItem)).BeginInit();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.menuStrip1.Dock = System.Windows.Forms.DockStyle.None;
			this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.operationToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(884, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(57, 27);
			this.menuStrip1.TabIndex = 0;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// operationToolStripMenuItem
			// 
			this.operationToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addToolStripMenuItem,
            this.deleteToolStripMenuItem,
            this.toolStripMenuItem1,
            this.refreshToolStripMenuItem,
            this.toolStripMenuItem2,
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
			// refreshToolStripMenuItem
			// 
			this.refreshToolStripMenuItem.Name = "refreshToolStripMenuItem";
			this.refreshToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F5;
			this.refreshToolStripMenuItem.Size = new System.Drawing.Size(173, 26);
			this.refreshToolStripMenuItem.Text = "刷新";
			this.refreshToolStripMenuItem.Click += new System.EventHandler(this.refreshToolStripMenuItem_Click);
			// 
			// toolStripMenuItem2
			// 
			this.toolStripMenuItem2.Name = "toolStripMenuItem2";
			this.toolStripMenuItem2.Size = new System.Drawing.Size(170, 6);
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
            this.toolStripButtonSave,
            this.toolStripSeparator1,
            this.toolStripButtonAdd,
            this.toolStripButtonDelete,
            this.toolStripSeparator2,
            this.toolStripButtonRefresh});
			this.toolStrip1.Location = new System.Drawing.Point(13, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(165, 27);
			this.toolStrip1.TabIndex = 1;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// toolStripButtonSave
			// 
			this.toolStripButtonSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripButtonSave.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonSave.Image")));
			this.toolStripButtonSave.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripButtonSave.Margin = new System.Windows.Forms.Padding(10, 1, 5, 2);
			this.toolStripButtonSave.Name = "toolStripButtonSave";
			this.toolStripButtonSave.Size = new System.Drawing.Size(24, 24);
			this.toolStripButtonSave.Text = "保存";
			this.toolStripButtonSave.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 27);
			// 
			// toolStripButtonAdd
			// 
			this.toolStripButtonAdd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripButtonAdd.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonAdd.Image")));
			this.toolStripButtonAdd.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripButtonAdd.Margin = new System.Windows.Forms.Padding(10, 1, 5, 2);
			this.toolStripButtonAdd.Name = "toolStripButtonAdd";
			this.toolStripButtonAdd.Size = new System.Drawing.Size(24, 24);
			this.toolStripButtonAdd.Text = "添加";
			this.toolStripButtonAdd.Click += new System.EventHandler(this.addToolStripMenuItem_Click);
			// 
			// toolStripButtonDelete
			// 
			this.toolStripButtonDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripButtonDelete.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonDelete.Image")));
			this.toolStripButtonDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripButtonDelete.Margin = new System.Windows.Forms.Padding(0, 1, 5, 2);
			this.toolStripButtonDelete.Name = "toolStripButtonDelete";
			this.toolStripButtonDelete.Size = new System.Drawing.Size(24, 24);
			this.toolStripButtonDelete.Text = "删除";
			this.toolStripButtonDelete.Click += new System.EventHandler(this.deleteToolStripMenuItem_Click);
			// 
			// toolStripSeparator2
			// 
			this.toolStripSeparator2.Name = "toolStripSeparator2";
			this.toolStripSeparator2.Size = new System.Drawing.Size(6, 27);
			// 
			// toolStripButtonRefresh
			// 
			this.toolStripButtonRefresh.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.toolStripButtonRefresh.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonRefresh.Image")));
			this.toolStripButtonRefresh.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.toolStripButtonRefresh.Margin = new System.Windows.Forms.Padding(10, 1, 0, 2);
			this.toolStripButtonRefresh.Name = "toolStripButtonRefresh";
			this.toolStripButtonRefresh.Size = new System.Drawing.Size(24, 24);
			this.toolStripButtonRefresh.Text = "刷新";
			this.toolStripButtonRefresh.Click += new System.EventHandler(this.refreshToolStripMenuItem_Click);
			// 
			// splitContainer1
			// 
			this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.splitContainer1.Location = new System.Drawing.Point(0, 30);
			this.splitContainer1.Name = "splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.treeView1);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.panelGeneralParams);
			this.splitContainer1.Panel2.Controls.Add(this.dataGridViewMonitorItem);
			this.splitContainer1.Size = new System.Drawing.Size(950, 581);
			this.splitContainer1.SplitterDistance = 117;
			this.splitContainer1.TabIndex = 2;
			// 
			// treeView1
			// 
			this.treeView1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.treeView1.Location = new System.Drawing.Point(0, 0);
			this.treeView1.Name = "treeView1";
			treeNode4.Name = "MonitorItem";
			treeNode4.Text = "监控项";
			treeNode5.Name = "SysParam";
			treeNode5.Text = "通用参数";
			treeNode6.Name = "RootNode";
			treeNode6.Text = "基础数据";
			this.treeView1.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode6});
			this.treeView1.Size = new System.Drawing.Size(117, 581);
			this.treeView1.TabIndex = 0;
			this.treeView1.BeforeSelect += new System.Windows.Forms.TreeViewCancelEventHandler(this.treeView1_BeforeSelect);
			this.treeView1.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView1_AfterSelect);
			// 
			// panelGeneralParams
			// 
			this.panelGeneralParams.Controls.Add(this.comboBoxCategory);
			this.panelGeneralParams.Controls.Add(this.label1);
			this.panelGeneralParams.Controls.Add(this.dataGridViewGeneralParams);
			this.panelGeneralParams.Dock = System.Windows.Forms.DockStyle.Fill;
			this.panelGeneralParams.Location = new System.Drawing.Point(0, 0);
			this.panelGeneralParams.Name = "panelGeneralParams";
			this.panelGeneralParams.Size = new System.Drawing.Size(829, 581);
			this.panelGeneralParams.TabIndex = 2;
			// 
			// dataGridViewGeneralParams
			// 
			this.dataGridViewGeneralParams.AllowUserToAddRows = false;
			this.dataGridViewGeneralParams.AllowUserToDeleteRows = false;
			this.dataGridViewGeneralParams.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.dataGridViewGeneralParams.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridViewGeneralParams.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Category,
            this.ItemName,
            this.Value,
            this.DispOrder,
            this.Memo,
            this.Hide,
            this.IsEncrypted,
            this.IsProtected});
			this.dataGridViewGeneralParams.Location = new System.Drawing.Point(0, 50);
			this.dataGridViewGeneralParams.Name = "dataGridViewGeneralParams";
			this.dataGridViewGeneralParams.RowTemplate.Height = 24;
			this.dataGridViewGeneralParams.Size = new System.Drawing.Size(829, 528);
			this.dataGridViewGeneralParams.TabIndex = 0;
			// 
			// dataGridViewMonitorItem
			// 
			this.dataGridViewMonitorItem.AllowUserToAddRows = false;
			this.dataGridViewMonitorItem.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridViewMonitorItem.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ItemId,
            this.Address,
            this.DisplayName,
            this.DataType,
            this.NeedAccumulate,
            this.UpdateHistory,
            this.InConverter,
            this.OutConverter,
            this.Status});
			this.dataGridViewMonitorItem.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dataGridViewMonitorItem.Location = new System.Drawing.Point(0, 0);
			this.dataGridViewMonitorItem.Name = "dataGridViewMonitorItem";
			this.dataGridViewMonitorItem.RowTemplate.Height = 24;
			this.dataGridViewMonitorItem.Size = new System.Drawing.Size(829, 581);
			this.dataGridViewMonitorItem.TabIndex = 1;
			// 
			// ItemId
			// 
			this.ItemId.DataPropertyName = "ItemId";
			this.ItemId.HeaderText = "ID";
			this.ItemId.Name = "ItemId";
			// 
			// Address
			// 
			this.Address.DataPropertyName = "Address";
			this.Address.HeaderText = "地址";
			this.Address.Name = "Address";
			this.Address.Width = 120;
			// 
			// DisplayName
			// 
			this.DisplayName.DataPropertyName = "DisplayName";
			this.DisplayName.HeaderText = "显示名称";
			this.DisplayName.Name = "DisplayName";
			this.DisplayName.Width = 120;
			// 
			// DataType
			// 
			this.DataType.DataPropertyName = "DataType";
			dataGridViewCellStyle4.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
			this.DataType.DefaultCellStyle = dataGridViewCellStyle4;
			this.DataType.HeaderText = "数据类型";
			this.DataType.Name = "DataType";
			// 
			// NeedAccumulate
			// 
			this.NeedAccumulate.DataPropertyName = "NeedAccumulate";
			this.NeedAccumulate.FalseValue = "0";
			this.NeedAccumulate.HeaderText = "累积";
			this.NeedAccumulate.Name = "NeedAccumulate";
			this.NeedAccumulate.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.NeedAccumulate.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
			this.NeedAccumulate.TrueValue = "1";
			this.NeedAccumulate.Width = 50;
			// 
			// UpdateHistory
			// 
			this.UpdateHistory.DataPropertyName = "UpdateHistory";
			this.UpdateHistory.FalseValue = "0";
			this.UpdateHistory.HeaderText = "统计历史";
			this.UpdateHistory.Name = "UpdateHistory";
			this.UpdateHistory.TrueValue = "1";
			this.UpdateHistory.Width = 50;
			// 
			// InConverter
			// 
			this.InConverter.DataPropertyName = "InConverter";
			this.InConverter.HeaderText = "输入转换";
			this.InConverter.Name = "InConverter";
			this.InConverter.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.InConverter.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
			// 
			// OutConverter
			// 
			this.OutConverter.DataPropertyName = "OutConverter";
			this.OutConverter.HeaderText = "输出转换";
			this.OutConverter.Name = "OutConverter";
			// 
			// Status
			// 
			this.Status.DataPropertyName = "Status";
			this.Status.FalseValue = "X";
			this.Status.HeaderText = "状态";
			this.Status.Name = "Status";
			this.Status.TrueValue = "A";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(63, 14);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(50, 17);
			this.label1.TabIndex = 1;
			this.label1.Text = "类别：";
			// 
			// comboBoxCategory
			// 
			this.comboBoxCategory.FormattingEnabled = true;
			this.comboBoxCategory.Location = new System.Drawing.Point(119, 14);
			this.comboBoxCategory.Name = "comboBoxCategory";
			this.comboBoxCategory.Size = new System.Drawing.Size(274, 24);
			this.comboBoxCategory.TabIndex = 2;
			this.comboBoxCategory.SelectedIndexChanged += new System.EventHandler(this.comboBoxCategory_SelectedIndexChanged);
			// 
			// Category
			// 
			this.Category.DataPropertyName = "Category";
			this.Category.HeaderText = "分类";
			this.Category.Name = "Category";
			this.Category.Visible = false;
			// 
			// ItemName
			// 
			this.ItemName.DataPropertyName = "Name";
			this.ItemName.HeaderText = "名称";
			this.ItemName.MaxInputLength = 50;
			this.ItemName.Name = "ItemName";
			this.ItemName.Width = 200;
			// 
			// Value
			// 
			this.Value.DataPropertyName = "Value";
			this.Value.HeaderText = "值";
			this.Value.MaxInputLength = 200;
			this.Value.Name = "Value";
			this.Value.Width = 200;
			// 
			// DispOrder
			// 
			this.DispOrder.DataPropertyName = "DispOrder";
			dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
			this.DispOrder.DefaultCellStyle = dataGridViewCellStyle3;
			this.DispOrder.HeaderText = "序号";
			this.DispOrder.MaxInputLength = 5;
			this.DispOrder.Name = "DispOrder";
			this.DispOrder.Width = 80;
			// 
			// Memo
			// 
			this.Memo.DataPropertyName = "Memo";
			this.Memo.HeaderText = "备注";
			this.Memo.MaxInputLength = 100;
			this.Memo.Name = "Memo";
			this.Memo.Width = 300;
			// 
			// Hide
			// 
			this.Hide.DataPropertyName = "Hide";
			this.Hide.FalseValue = "0";
			this.Hide.HeaderText = "隐藏";
			this.Hide.Name = "Hide";
			this.Hide.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.Hide.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
			this.Hide.TrueValue = "1";
			this.Hide.Visible = false;
			// 
			// IsEncrypted
			// 
			this.IsEncrypted.DataPropertyName = "IsEncrypted";
			this.IsEncrypted.FalseValue = "0";
			this.IsEncrypted.HeaderText = "加密";
			this.IsEncrypted.Name = "IsEncrypted";
			this.IsEncrypted.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.IsEncrypted.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
			this.IsEncrypted.TrueValue = "1";
			this.IsEncrypted.Visible = false;
			// 
			// IsProtected
			// 
			this.IsProtected.DataPropertyName = "IsProtected";
			this.IsProtected.FalseValue = "0";
			this.IsProtected.HeaderText = "保护";
			this.IsProtected.Name = "IsProtected";
			this.IsProtected.ReadOnly = true;
			this.IsProtected.Resizable = System.Windows.Forms.DataGridViewTriState.True;
			this.IsProtected.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
			this.IsProtected.TrueValue = "0";
			// 
			// DataMaintainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(950, 611);
			this.Controls.Add(this.splitContainer1);
			this.Controls.Add(this.toolStrip1);
			this.Controls.Add(this.menuStrip1);
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "DataMaintainForm";
			this.Text = "基础数据维护";
			this.Load += new System.EventHandler(this.DataMaintainForm_Load);
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
			this.splitContainer1.ResumeLayout(false);
			this.panelGeneralParams.ResumeLayout(false);
			this.panelGeneralParams.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewGeneralParams)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewMonitorItem)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem operationToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem addToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem refreshToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
		private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripButton toolStripButtonSave;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripButton toolStripButtonAdd;
		private System.Windows.Forms.ToolStripButton toolStripButtonDelete;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStripButton toolStripButtonRefresh;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.TreeView treeView1;
		private System.Windows.Forms.DataGridView dataGridViewMonitorItem;
		private System.Windows.Forms.Panel panelGeneralParams;
		private System.Windows.Forms.DataGridView dataGridViewGeneralParams;
		private System.Windows.Forms.DataGridViewTextBoxColumn ItemId;
		private System.Windows.Forms.DataGridViewTextBoxColumn Address;
		private System.Windows.Forms.DataGridViewTextBoxColumn DisplayName;
		private System.Windows.Forms.DataGridViewTextBoxColumn DataType;
		private System.Windows.Forms.DataGridViewCheckBoxColumn NeedAccumulate;
		private System.Windows.Forms.DataGridViewCheckBoxColumn UpdateHistory;
		private System.Windows.Forms.DataGridViewTextBoxColumn InConverter;
		private System.Windows.Forms.DataGridViewTextBoxColumn OutConverter;
		private System.Windows.Forms.DataGridViewCheckBoxColumn Status;
		private System.Windows.Forms.ComboBox comboBoxCategory;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.DataGridViewTextBoxColumn Category;
		private System.Windows.Forms.DataGridViewTextBoxColumn ItemName;
		private System.Windows.Forms.DataGridViewTextBoxColumn Value;
		private System.Windows.Forms.DataGridViewTextBoxColumn DispOrder;
		private System.Windows.Forms.DataGridViewTextBoxColumn Memo;
		private System.Windows.Forms.DataGridViewCheckBoxColumn Hide;
		private System.Windows.Forms.DataGridViewCheckBoxColumn IsEncrypted;
		private System.Windows.Forms.DataGridViewCheckBoxColumn IsProtected;
	}
}