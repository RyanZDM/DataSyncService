namespace EBoard
{
	partial class MainForm
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
			System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
			System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
			System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
			System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
			System.Windows.Forms.DataVisualization.Charting.Legend legend2 = new System.Windows.Forms.DataVisualization.Charting.Legend();
			System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
			this.panel1 = new System.Windows.Forms.Panel();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.chartCurrMonth1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
			this.buttonReLogin = new System.Windows.Forms.Button();
			this.labelEnergyProductionMonth = new System.Windows.Forms.Label();
			this.labelBiogasMonth = new System.Windows.Forms.Label();
			this.label18 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.chartCurrMonth2 = new System.Windows.Forms.DataVisualization.Charting.Chart();
			this.labelCurrTime = new System.Windows.Forms.Label();
			this.labelCurrDate = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.panel2 = new System.Windows.Forms.Panel();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.labelRuntime1 = new System.Windows.Forms.Label();
			this.labelEnergyProduction1 = new System.Windows.Forms.Label();
			this.labelBiogasTotal = new System.Windows.Forms.Label();
			this.labelEnergyProductionTotal = new System.Windows.Forms.Label();
			this.labelRuntimeTotal = new System.Windows.Forms.Label();
			this.labelRuntime2 = new System.Windows.Forms.Label();
			this.labelEnergyProduction2 = new System.Windows.Forms.Label();
			this.labelBiogas2Torch = new System.Windows.Forms.Label();
			this.labelBiogas2Gen = new System.Windows.Forms.Label();
			this.label16 = new System.Windows.Forms.Label();
			this.label14 = new System.Windows.Forms.Label();
			this.label17 = new System.Windows.Forms.Label();
			this.label11 = new System.Windows.Forms.Label();
			this.label10 = new System.Windows.Forms.Label();
			this.label13 = new System.Windows.Forms.Label();
			this.label12 = new System.Windows.Forms.Label();
			this.label15 = new System.Windows.Forms.Label();
			this.label9 = new System.Windows.Forms.Label();
			this.label8 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.panelIndicator = new System.Windows.Forms.Panel();
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
			this.labelWorker1 = new System.Windows.Forms.Label();
			this.labelWorker2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.labelTotalRuntime1 = new System.Windows.Forms.Label();
			this.labelTotalRuntime2 = new System.Windows.Forms.Label();
			this.panel1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.chartCurrMonth1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.chartCurrMonth2)).BeginInit();
			this.panel2.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.tableLayoutPanel1.SuspendLayout();
			this.tableLayoutPanel2.SuspendLayout();
			this.SuspendLayout();
			// 
			// panel1
			// 
			this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.panel1.Controls.Add(this.tableLayoutPanel1);
			this.panel1.Controls.Add(this.labelCurrDate);
			this.panel1.Controls.Add(this.panelIndicator);
			this.panel1.Controls.Add(this.splitContainer1);
			this.panel1.Controls.Add(this.labelCurrTime);
			this.panel1.Controls.Add(this.label1);
			this.panel1.Location = new System.Drawing.Point(30, 6);
			this.panel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(1167, 682);
			this.panel1.TabIndex = 0;
			// 
			// splitContainer1
			// 
			this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.splitContainer1.Location = new System.Drawing.Point(9, 334);
			this.splitContainer1.Margin = new System.Windows.Forms.Padding(2);
			this.splitContainer1.Name = "splitContainer1";
			this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.chartCurrMonth1);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.buttonReLogin);
			this.splitContainer1.Panel2.Controls.Add(this.labelEnergyProductionMonth);
			this.splitContainer1.Panel2.Controls.Add(this.labelBiogasMonth);
			this.splitContainer1.Panel2.Controls.Add(this.label18);
			this.splitContainer1.Panel2.Controls.Add(this.label2);
			this.splitContainer1.Panel2.Controls.Add(this.chartCurrMonth2);
			this.splitContainer1.Size = new System.Drawing.Size(1145, 336);
			this.splitContainer1.SplitterDistance = 171;
			this.splitContainer1.SplitterWidth = 3;
			this.splitContainer1.TabIndex = 5;
			// 
			// chartCurrMonth1
			// 
			this.chartCurrMonth1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			chartArea1.Name = "ChartArea1";
			this.chartCurrMonth1.ChartAreas.Add(chartArea1);
			legend1.Name = "Legend1";
			this.chartCurrMonth1.Legends.Add(legend1);
			this.chartCurrMonth1.Location = new System.Drawing.Point(0, 0);
			this.chartCurrMonth1.Margin = new System.Windows.Forms.Padding(2);
			this.chartCurrMonth1.Name = "chartCurrMonth1";
			series1.ChartArea = "ChartArea1";
			series1.Legend = "Legend1";
			series1.Name = "Series1";
			this.chartCurrMonth1.Series.Add(series1);
			this.chartCurrMonth1.Size = new System.Drawing.Size(1143, 168);
			this.chartCurrMonth1.TabIndex = 0;
			this.chartCurrMonth1.Text = "chart1";
			// 
			// buttonReLogin
			// 
			this.buttonReLogin.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonReLogin.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.buttonReLogin.Location = new System.Drawing.Point(1036, 114);
			this.buttonReLogin.Margin = new System.Windows.Forms.Padding(2);
			this.buttonReLogin.Name = "buttonReLogin";
			this.buttonReLogin.Size = new System.Drawing.Size(102, 35);
			this.buttonReLogin.TabIndex = 2;
			this.buttonReLogin.Text = "重新登录";
			this.buttonReLogin.UseVisualStyleBackColor = true;
			this.buttonReLogin.Click += new System.EventHandler(this.buttonReLogin_Click);
			// 
			// labelEnergyProductionMonth
			// 
			this.labelEnergyProductionMonth.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.labelEnergyProductionMonth.Font = new System.Drawing.Font("KaiTi", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.labelEnergyProductionMonth.Location = new System.Drawing.Point(1032, 43);
			this.labelEnergyProductionMonth.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelEnergyProductionMonth.Name = "labelEnergyProductionMonth";
			this.labelEnergyProductionMonth.Size = new System.Drawing.Size(111, 23);
			this.labelEnergyProductionMonth.TabIndex = 1;
			// 
			// labelBiogasMonth
			// 
			this.labelBiogasMonth.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.labelBiogasMonth.Font = new System.Drawing.Font("KaiTi", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.labelBiogasMonth.Location = new System.Drawing.Point(1032, 20);
			this.labelBiogasMonth.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelBiogasMonth.Name = "labelBiogasMonth";
			this.labelBiogasMonth.Size = new System.Drawing.Size(111, 23);
			this.labelBiogasMonth.TabIndex = 1;
			// 
			// label18
			// 
			this.label18.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label18.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label18.Location = new System.Drawing.Point(872, 43);
			this.label18.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label18.Name = "label18";
			this.label18.Size = new System.Drawing.Size(155, 25);
			this.label18.TabIndex = 1;
			this.label18.Text = "当月累计发电量：";
			// 
			// label2
			// 
			this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label2.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label2.Location = new System.Drawing.Point(872, 20);
			this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(177, 23);
			this.label2.TabIndex = 1;
			this.label2.Text = "当月累计沼气消耗量：";
			// 
			// chartCurrMonth2
			// 
			this.chartCurrMonth2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			chartArea2.Name = "ChartArea1";
			this.chartCurrMonth2.ChartAreas.Add(chartArea2);
			legend2.Name = "Legend1";
			this.chartCurrMonth2.Legends.Add(legend2);
			this.chartCurrMonth2.Location = new System.Drawing.Point(0, 2);
			this.chartCurrMonth2.Margin = new System.Windows.Forms.Padding(2);
			this.chartCurrMonth2.Name = "chartCurrMonth2";
			series2.ChartArea = "ChartArea1";
			series2.Legend = "Legend1";
			series2.Name = "Series1";
			this.chartCurrMonth2.Series.Add(series2);
			this.chartCurrMonth2.Size = new System.Drawing.Size(854, 159);
			this.chartCurrMonth2.TabIndex = 0;
			this.chartCurrMonth2.Text = "chart2";
			// 
			// labelCurrTime
			// 
			this.labelCurrTime.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.labelCurrTime.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.labelCurrTime.Location = new System.Drawing.Point(1034, 4);
			this.labelCurrTime.Name = "labelCurrTime";
			this.labelCurrTime.Size = new System.Drawing.Size(118, 51);
			this.labelCurrTime.TabIndex = 1;
			this.labelCurrTime.Text = "CurrTIme";
			this.labelCurrTime.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// labelCurrDate
			// 
			this.labelCurrDate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.labelCurrDate.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.labelCurrDate.Location = new System.Drawing.Point(719, 4);
			this.labelCurrDate.Name = "labelCurrDate";
			this.labelCurrDate.Size = new System.Drawing.Size(310, 51);
			this.labelCurrDate.TabIndex = 1;
			this.labelCurrDate.Text = "CurrDate";
			this.labelCurrDate.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label1
			// 
			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.label1.Font = new System.Drawing.Font("STXingkai", 36F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label1.Location = new System.Drawing.Point(3, 4);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(1163, 78);
			this.label1.TabIndex = 0;
			this.label1.Text = "黎明沼气发电厂生产数据看板";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// panel2
			// 
			this.panel2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.panel2.Controls.Add(this.groupBox1);
			this.panel2.Location = new System.Drawing.Point(30, 131);
			this.panel2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.panel2.Name = "panel2";
			this.panel2.Size = new System.Drawing.Size(1167, 204);
			this.panel2.TabIndex = 0;
			// 
			// groupBox1
			// 
			this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox1.Controls.Add(this.tableLayoutPanel2);
			this.groupBox1.Font = new System.Drawing.Font("STXingkai", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.groupBox1.Location = new System.Drawing.Point(9, 14);
			this.groupBox1.Margin = new System.Windows.Forms.Padding(2);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Padding = new System.Windows.Forms.Padding(2);
			this.groupBox1.Size = new System.Drawing.Size(1145, 180);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "当前班次生产数据";
			// 
			// labelRuntime1
			// 
			this.labelRuntime1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.labelRuntime1.Font = new System.Drawing.Font("KaiTi", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.labelRuntime1.Location = new System.Drawing.Point(285, 93);
			this.labelRuntime1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelRuntime1.Name = "labelRuntime1";
			this.labelRuntime1.Size = new System.Drawing.Size(166, 34);
			this.labelRuntime1.TabIndex = 6;
			this.labelRuntime1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelEnergyProduction1
			// 
			this.labelEnergyProduction1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.labelEnergyProduction1.Font = new System.Drawing.Font("KaiTi", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.labelEnergyProduction1.Location = new System.Drawing.Point(285, 62);
			this.labelEnergyProduction1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelEnergyProduction1.Name = "labelEnergyProduction1";
			this.labelEnergyProduction1.Size = new System.Drawing.Size(166, 31);
			this.labelEnergyProduction1.TabIndex = 6;
			this.labelEnergyProduction1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelBiogasTotal
			// 
			this.labelBiogasTotal.Dock = System.Windows.Forms.DockStyle.Fill;
			this.labelBiogasTotal.Font = new System.Drawing.Font("KaiTi", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.labelBiogasTotal.Location = new System.Drawing.Point(965, 31);
			this.labelBiogasTotal.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelBiogasTotal.Name = "labelBiogasTotal";
			this.labelBiogasTotal.Size = new System.Drawing.Size(168, 31);
			this.labelBiogasTotal.TabIndex = 6;
			this.labelBiogasTotal.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelEnergyProductionTotal
			// 
			this.labelEnergyProductionTotal.Dock = System.Windows.Forms.DockStyle.Fill;
			this.labelEnergyProductionTotal.Font = new System.Drawing.Font("KaiTi", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.labelEnergyProductionTotal.Location = new System.Drawing.Point(965, 62);
			this.labelEnergyProductionTotal.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelEnergyProductionTotal.Name = "labelEnergyProductionTotal";
			this.labelEnergyProductionTotal.Size = new System.Drawing.Size(168, 31);
			this.labelEnergyProductionTotal.TabIndex = 6;
			this.labelEnergyProductionTotal.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelRuntimeTotal
			// 
			this.labelRuntimeTotal.Dock = System.Windows.Forms.DockStyle.Fill;
			this.labelRuntimeTotal.Font = new System.Drawing.Font("KaiTi", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.labelRuntimeTotal.Location = new System.Drawing.Point(965, 93);
			this.labelRuntimeTotal.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelRuntimeTotal.Name = "labelRuntimeTotal";
			this.labelRuntimeTotal.Size = new System.Drawing.Size(168, 34);
			this.labelRuntimeTotal.TabIndex = 6;
			this.labelRuntimeTotal.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelRuntime2
			// 
			this.labelRuntime2.Dock = System.Windows.Forms.DockStyle.Fill;
			this.labelRuntime2.Font = new System.Drawing.Font("KaiTi", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.labelRuntime2.Location = new System.Drawing.Point(625, 93);
			this.labelRuntime2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelRuntime2.Name = "labelRuntime2";
			this.labelRuntime2.Size = new System.Drawing.Size(166, 34);
			this.labelRuntime2.TabIndex = 6;
			this.labelRuntime2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelEnergyProduction2
			// 
			this.labelEnergyProduction2.Dock = System.Windows.Forms.DockStyle.Fill;
			this.labelEnergyProduction2.Font = new System.Drawing.Font("KaiTi", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.labelEnergyProduction2.Location = new System.Drawing.Point(625, 62);
			this.labelEnergyProduction2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelEnergyProduction2.Name = "labelEnergyProduction2";
			this.labelEnergyProduction2.Size = new System.Drawing.Size(166, 31);
			this.labelEnergyProduction2.TabIndex = 6;
			this.labelEnergyProduction2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelBiogas2Torch
			// 
			this.labelBiogas2Torch.Dock = System.Windows.Forms.DockStyle.Fill;
			this.labelBiogas2Torch.Font = new System.Drawing.Font("KaiTi", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.labelBiogas2Torch.Location = new System.Drawing.Point(625, 31);
			this.labelBiogas2Torch.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelBiogas2Torch.Name = "labelBiogas2Torch";
			this.labelBiogas2Torch.Size = new System.Drawing.Size(166, 31);
			this.labelBiogas2Torch.TabIndex = 6;
			this.labelBiogas2Torch.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelBiogas2Gen
			// 
			this.labelBiogas2Gen.Dock = System.Windows.Forms.DockStyle.Fill;
			this.labelBiogas2Gen.Font = new System.Drawing.Font("KaiTi", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.labelBiogas2Gen.Location = new System.Drawing.Point(285, 31);
			this.labelBiogas2Gen.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.labelBiogas2Gen.Name = "labelBiogas2Gen";
			this.labelBiogas2Gen.Size = new System.Drawing.Size(166, 31);
			this.labelBiogas2Gen.TabIndex = 6;
			this.labelBiogas2Gen.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// label16
			// 
			this.label16.AutoSize = true;
			this.label16.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label16.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label16.Location = new System.Drawing.Point(455, 93);
			this.label16.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label16.Name = "label16";
			this.label16.Size = new System.Drawing.Size(166, 34);
			this.label16.TabIndex = 4;
			this.label16.Text = "2#机组运行时间：";
			this.label16.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label14
			// 
			this.label14.AutoSize = true;
			this.label14.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label14.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label14.Location = new System.Drawing.Point(795, 31);
			this.label14.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label14.Name = "label14";
			this.label14.Size = new System.Drawing.Size(166, 31);
			this.label14.TabIndex = 3;
			this.label14.Text = "总计沼气流量：";
			this.label14.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label17
			// 
			this.label17.AutoSize = true;
			this.label17.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label17.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label17.Location = new System.Drawing.Point(795, 93);
			this.label17.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label17.Name = "label17";
			this.label17.Size = new System.Drawing.Size(166, 34);
			this.label17.TabIndex = 3;
			this.label17.Text = "总计运行时间：";
			this.label17.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label11
			// 
			this.label11.AutoSize = true;
			this.label11.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label11.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label11.Location = new System.Drawing.Point(795, 62);
			this.label11.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label11.Name = "label11";
			this.label11.Size = new System.Drawing.Size(166, 31);
			this.label11.TabIndex = 3;
			this.label11.Text = "总计发电量：";
			this.label11.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label10
			// 
			this.label10.AutoSize = true;
			this.label10.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label10.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label10.Location = new System.Drawing.Point(455, 62);
			this.label10.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(166, 31);
			this.label10.TabIndex = 2;
			this.label10.Text = "2#机组发电量：";
			this.label10.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label13
			// 
			this.label13.AutoSize = true;
			this.label13.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label13.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label13.Location = new System.Drawing.Point(455, 31);
			this.label13.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label13.Name = "label13";
			this.label13.Size = new System.Drawing.Size(166, 31);
			this.label13.TabIndex = 1;
			this.label13.Tag = "";
			this.label13.Text = "火炬沼气：";
			this.label13.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label12
			// 
			this.label12.AutoSize = true;
			this.label12.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label12.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label12.Location = new System.Drawing.Point(115, 31);
			this.label12.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label12.Name = "label12";
			this.label12.Size = new System.Drawing.Size(166, 31);
			this.label12.TabIndex = 1;
			this.label12.Tag = "";
			this.label12.Text = "发电机沼气：";
			this.label12.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label15
			// 
			this.label15.AutoSize = true;
			this.label15.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label15.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label15.Location = new System.Drawing.Point(115, 93);
			this.label15.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label15.Name = "label15";
			this.label15.Size = new System.Drawing.Size(166, 34);
			this.label15.TabIndex = 1;
			this.label15.Text = "1#机组运行时间：";
			this.label15.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label9
			// 
			this.label9.AutoSize = true;
			this.label9.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label9.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label9.Location = new System.Drawing.Point(115, 62);
			this.label9.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(166, 31);
			this.label9.TabIndex = 1;
			this.label9.Text = "1#机组发电量：";
			this.label9.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label8
			// 
			this.label8.AutoSize = true;
			this.label8.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label8.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label8.Location = new System.Drawing.Point(2, 93);
			this.label8.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(109, 34);
			this.label8.TabIndex = 0;
			this.label8.Text = "当前班次运行时间";
			this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label7.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label7.Location = new System.Drawing.Point(2, 62);
			this.label7.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(109, 31);
			this.label7.TabIndex = 0;
			this.label7.Text = "当前班次发电量";
			this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label6.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label6.Location = new System.Drawing.Point(2, 31);
			this.label6.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(109, 31);
			this.label6.TabIndex = 0;
			this.label6.Text = "当前班次沼气流量";
			this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label5.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label5.Location = new System.Drawing.Point(2, 0);
			this.label5.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(109, 31);
			this.label5.TabIndex = 0;
			this.label5.Text = "当前员工";
			this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// panelIndicator
			// 
			this.panelIndicator.BackgroundImage = global::EBoard.Properties.Resources.green;
			this.panelIndicator.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
			this.panelIndicator.Location = new System.Drawing.Point(9, 4);
			this.panelIndicator.Margin = new System.Windows.Forms.Padding(2);
			this.panelIndicator.Name = "panelIndicator";
			this.panelIndicator.Size = new System.Drawing.Size(26, 25);
			this.panelIndicator.TabIndex = 1;
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tableLayoutPanel1.ColumnCount = 6;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 13F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 12F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 13F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 12F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
			this.tableLayoutPanel1.Controls.Add(this.label3, 1, 0);
			this.tableLayoutPanel1.Controls.Add(this.label4, 3, 0);
			this.tableLayoutPanel1.Controls.Add(this.labelTotalRuntime1, 2, 0);
			this.tableLayoutPanel1.Controls.Add(this.labelTotalRuntime2, 4, 0);
			this.tableLayoutPanel1.Location = new System.Drawing.Point(-1, 82);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 1;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.tableLayoutPanel1.Size = new System.Drawing.Size(1163, 40);
			this.tableLayoutPanel1.TabIndex = 6;
			// 
			// tableLayoutPanel2
			// 
			this.tableLayoutPanel2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tableLayoutPanel2.ColumnCount = 7;
			this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 10F));
			this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 15F));
			this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 15F));
			this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 15F));
			this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 15F));
			this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 15F));
			this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 15F));
			this.tableLayoutPanel2.Controls.Add(this.label8, 0, 3);
			this.tableLayoutPanel2.Controls.Add(this.labelRuntimeTotal, 6, 3);
			this.tableLayoutPanel2.Controls.Add(this.label5, 0, 0);
			this.tableLayoutPanel2.Controls.Add(this.labelEnergyProductionTotal, 6, 2);
			this.tableLayoutPanel2.Controls.Add(this.labelEnergyProduction1, 2, 2);
			this.tableLayoutPanel2.Controls.Add(this.label6, 0, 1);
			this.tableLayoutPanel2.Controls.Add(this.labelBiogasTotal, 6, 1);
			this.tableLayoutPanel2.Controls.Add(this.label15, 1, 3);
			this.tableLayoutPanel2.Controls.Add(this.labelEnergyProduction2, 4, 2);
			this.tableLayoutPanel2.Controls.Add(this.label12, 1, 1);
			this.tableLayoutPanel2.Controls.Add(this.label11, 5, 2);
			this.tableLayoutPanel2.Controls.Add(this.labelBiogas2Gen, 2, 1);
			this.tableLayoutPanel2.Controls.Add(this.label13, 3, 1);
			this.tableLayoutPanel2.Controls.Add(this.labelBiogas2Torch, 4, 1);
			this.tableLayoutPanel2.Controls.Add(this.label10, 3, 2);
			this.tableLayoutPanel2.Controls.Add(this.label14, 5, 1);
			this.tableLayoutPanel2.Controls.Add(this.label7, 0, 2);
			this.tableLayoutPanel2.Controls.Add(this.label9, 1, 2);
			this.tableLayoutPanel2.Controls.Add(this.labelRuntime1, 2, 3);
			this.tableLayoutPanel2.Controls.Add(this.label16, 3, 3);
			this.tableLayoutPanel2.Controls.Add(this.labelRuntime2, 4, 3);
			this.tableLayoutPanel2.Controls.Add(this.label17, 5, 3);
			this.tableLayoutPanel2.Controls.Add(this.labelWorker1, 1, 0);
			this.tableLayoutPanel2.Controls.Add(this.labelWorker2, 2, 0);
			this.tableLayoutPanel2.Location = new System.Drawing.Point(5, 24);
			this.tableLayoutPanel2.Name = "tableLayoutPanel2";
			this.tableLayoutPanel2.RowCount = 4;
			this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
			this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
			this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
			this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
			this.tableLayoutPanel2.Size = new System.Drawing.Size(1135, 127);
			this.tableLayoutPanel2.TabIndex = 7;
			// 
			// labelWorker1
			// 
			this.labelWorker1.AutoSize = true;
			this.labelWorker1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.labelWorker1.Font = new System.Drawing.Font("STKaiti", 15F);
			this.labelWorker1.Location = new System.Drawing.Point(116, 0);
			this.labelWorker1.Name = "labelWorker1";
			this.labelWorker1.Size = new System.Drawing.Size(164, 31);
			this.labelWorker1.TabIndex = 7;
			this.labelWorker1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// labelWorker2
			// 
			this.labelWorker2.AutoSize = true;
			this.labelWorker2.Dock = System.Windows.Forms.DockStyle.Fill;
			this.labelWorker2.Font = new System.Drawing.Font("STKaiti", 15F);
			this.labelWorker2.Location = new System.Drawing.Point(286, 0);
			this.labelWorker2.Name = "labelWorker2";
			this.labelWorker2.Size = new System.Drawing.Size(164, 31);
			this.labelWorker2.TabIndex = 8;
			this.labelWorker2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label3.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label3.Location = new System.Drawing.Point(293, 0);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(145, 40);
			this.label3.TabIndex = 4;
			this.label3.Text = "1# 发电机累计运行时间";
			this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label4.Font = new System.Drawing.Font("STXingkai", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
			this.label4.Location = new System.Drawing.Point(583, 0);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(145, 40);
			this.label4.TabIndex = 5;
			this.label4.Text = "2# 发电机累计运行时间";
			this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// labelTotalRuntime1
			// 
			this.labelTotalRuntime1.AutoSize = true;
			this.labelTotalRuntime1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.labelTotalRuntime1.Location = new System.Drawing.Point(444, 0);
			this.labelTotalRuntime1.Name = "labelTotalRuntime1";
			this.labelTotalRuntime1.Size = new System.Drawing.Size(133, 40);
			this.labelTotalRuntime1.TabIndex = 6;
			this.labelTotalRuntime1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelTotalRuntime2
			// 
			this.labelTotalRuntime2.AutoSize = true;
			this.labelTotalRuntime2.Dock = System.Windows.Forms.DockStyle.Fill;
			this.labelTotalRuntime2.Location = new System.Drawing.Point(734, 0);
			this.labelTotalRuntime2.Name = "labelTotalRuntime2";
			this.labelTotalRuntime2.Size = new System.Drawing.Size(133, 40);
			this.labelTotalRuntime2.TabIndex = 7;
			this.labelTotalRuntime2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
			this.ClientSize = new System.Drawing.Size(1231, 698);
			this.ControlBox = false;
			this.Controls.Add(this.panel2);
			this.Controls.Add(this.panel1);
			this.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
			this.Name = "MainForm";
			this.Text = "EBoard";
			this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
			this.Load += new System.EventHandler(this.MainForm_Load);
			this.panel1.ResumeLayout(false);
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
			this.splitContainer1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.chartCurrMonth1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.chartCurrMonth2)).EndInit();
			this.panel2.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.tableLayoutPanel1.ResumeLayout(false);
			this.tableLayoutPanel1.PerformLayout();
			this.tableLayoutPanel2.ResumeLayout(false);
			this.tableLayoutPanel2.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label labelCurrDate;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label11;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.Label label14;
		private System.Windows.Forms.Label label13;
		private System.Windows.Forms.Label label12;
		private System.Windows.Forms.Label label16;
		private System.Windows.Forms.Label label15;
		private System.Windows.Forms.Label label17;
		private System.Windows.Forms.Label labelRuntime1;
		private System.Windows.Forms.Label labelEnergyProduction1;
		private System.Windows.Forms.Label labelBiogasTotal;
		private System.Windows.Forms.Label labelEnergyProductionTotal;
		private System.Windows.Forms.Label labelRuntimeTotal;
		private System.Windows.Forms.Label labelRuntime2;
		private System.Windows.Forms.Label labelEnergyProduction2;
		private System.Windows.Forms.Label labelBiogas2Torch;
		private System.Windows.Forms.Label labelBiogas2Gen;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.DataVisualization.Charting.Chart chartCurrMonth1;
		private System.Windows.Forms.DataVisualization.Charting.Chart chartCurrMonth2;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label labelCurrTime;
		private System.Windows.Forms.Label labelEnergyProductionMonth;
		private System.Windows.Forms.Label labelBiogasMonth;
		private System.Windows.Forms.Label label18;
		private System.Windows.Forms.Panel panelIndicator;
		private System.Windows.Forms.Button buttonReLogin;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
		private System.Windows.Forms.Label labelWorker1;
		private System.Windows.Forms.Label labelWorker2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label labelTotalRuntime1;
		private System.Windows.Forms.Label labelTotalRuntime2;
	}
}

