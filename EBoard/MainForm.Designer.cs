namespace EBoard
{
	partial class EBoard
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
			System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea3 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
			System.Windows.Forms.DataVisualization.Charting.Legend legend3 = new System.Windows.Forms.DataVisualization.Charting.Legend();
			System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
			System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea4 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
			System.Windows.Forms.DataVisualization.Charting.Legend legend4 = new System.Windows.Forms.DataVisualization.Charting.Legend();
			System.Windows.Forms.DataVisualization.Charting.Series series4 = new System.Windows.Forms.DataVisualization.Charting.Series();
			this.panel1 = new System.Windows.Forms.Panel();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.chartCurrMonth1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
			this.label2 = new System.Windows.Forms.Label();
			this.chartCurrMonth2 = new System.Windows.Forms.DataVisualization.Charting.Chart();
			this.labelTotalRuntime2 = new System.Windows.Forms.Label();
			this.labelTotalRuntime1 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
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
			this.labelBiogas2 = new System.Windows.Forms.Label();
			this.labelBiogas1 = new System.Windows.Forms.Label();
			this.labelWorkers = new System.Windows.Forms.Label();
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
			this.labelCurrTime = new System.Windows.Forms.Label();
			this.labelBiogasMonth = new System.Windows.Forms.Label();
			this.label18 = new System.Windows.Forms.Label();
			this.labelEnergyProductionMonth = new System.Windows.Forms.Label();
			this.panel1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.chartCurrMonth1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.chartCurrMonth2)).BeginInit();
			this.panel2.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// panel1
			// 
			this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.panel1.Controls.Add(this.splitContainer1);
			this.panel1.Controls.Add(this.labelTotalRuntime2);
			this.panel1.Controls.Add(this.labelTotalRuntime1);
			this.panel1.Controls.Add(this.label4);
			this.panel1.Controls.Add(this.label3);
			this.panel1.Controls.Add(this.labelCurrTime);
			this.panel1.Controls.Add(this.labelCurrDate);
			this.panel1.Controls.Add(this.label1);
			this.panel1.Location = new System.Drawing.Point(38, 34);
			this.panel1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(1271, 706);
			this.panel1.TabIndex = 0;
			// 
			// splitContainer1
			// 
			this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.splitContainer1.Location = new System.Drawing.Point(11, 418);
			this.splitContainer1.Name = "splitContainer1";
			this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.chartCurrMonth1);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.labelEnergyProductionMonth);
			this.splitContainer1.Panel2.Controls.Add(this.labelBiogasMonth);
			this.splitContainer1.Panel2.Controls.Add(this.label18);
			this.splitContainer1.Panel2.Controls.Add(this.label2);
			this.splitContainer1.Panel2.Controls.Add(this.chartCurrMonth2);
			this.splitContainer1.Size = new System.Drawing.Size(1244, 274);
			this.splitContainer1.SplitterDistance = 141;
			this.splitContainer1.TabIndex = 5;
			// 
			// chartCurrMonth1
			// 
			this.chartCurrMonth1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			chartArea3.Name = "ChartArea1";
			this.chartCurrMonth1.ChartAreas.Add(chartArea3);
			legend3.Name = "Legend1";
			this.chartCurrMonth1.Legends.Add(legend3);
			this.chartCurrMonth1.Location = new System.Drawing.Point(0, 0);
			this.chartCurrMonth1.Name = "chartCurrMonth1";
			series3.ChartArea = "ChartArea1";
			series3.Legend = "Legend1";
			series3.Name = "Series1";
			this.chartCurrMonth1.Series.Add(series3);
			this.chartCurrMonth1.Size = new System.Drawing.Size(1241, 138);
			this.chartCurrMonth1.TabIndex = 0;
			this.chartCurrMonth1.Text = "chart1";
			// 
			// label2
			// 
			this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label2.Location = new System.Drawing.Point(902, 25);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(221, 29);
			this.label2.TabIndex = 1;
			this.label2.Text = "当月累计沼气消耗量：";
			// 
			// chartCurrMonth2
			// 
			this.chartCurrMonth2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			chartArea4.Name = "ChartArea1";
			this.chartCurrMonth2.ChartAreas.Add(chartArea4);
			legend4.Name = "Legend1";
			this.chartCurrMonth2.Legends.Add(legend4);
			this.chartCurrMonth2.Location = new System.Drawing.Point(0, 3);
			this.chartCurrMonth2.Name = "chartCurrMonth2";
			series4.ChartArea = "ChartArea1";
			series4.Legend = "Legend1";
			series4.Name = "Series1";
			this.chartCurrMonth2.Series.Add(series4);
			this.chartCurrMonth2.Size = new System.Drawing.Size(880, 123);
			this.chartCurrMonth2.TabIndex = 0;
			this.chartCurrMonth2.Text = "chart2";
			// 
			// labelTotalRuntime2
			// 
			this.labelTotalRuntime2.Location = new System.Drawing.Point(898, 102);
			this.labelTotalRuntime2.Name = "labelTotalRuntime2";
			this.labelTotalRuntime2.Size = new System.Drawing.Size(302, 25);
			this.labelTotalRuntime2.TabIndex = 4;
			this.labelTotalRuntime2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelTotalRuntime1
			// 
			this.labelTotalRuntime1.Location = new System.Drawing.Point(381, 102);
			this.labelTotalRuntime1.Name = "labelTotalRuntime1";
			this.labelTotalRuntime1.Size = new System.Drawing.Size(269, 17);
			this.labelTotalRuntime1.TabIndex = 4;
			this.labelTotalRuntime1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label4.Location = new System.Drawing.Point(672, 102);
			this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(219, 25);
			this.label4.TabIndex = 3;
			this.label4.Text = "#2 发电机累计运行时间";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label3.Location = new System.Drawing.Point(155, 102);
			this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(219, 25);
			this.label3.TabIndex = 2;
			this.label3.Text = "#1 发电机累计运行时间";
			// 
			// labelCurrDate
			// 
			this.labelCurrDate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.labelCurrDate.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.labelCurrDate.Location = new System.Drawing.Point(866, 33);
			this.labelCurrDate.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelCurrDate.Name = "labelCurrDate";
			this.labelCurrDate.Size = new System.Drawing.Size(281, 64);
			this.labelCurrDate.TabIndex = 1;
			this.labelCurrDate.Text = "labelCurrDate";
			this.labelCurrDate.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label1
			// 
			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 19.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label1.Location = new System.Drawing.Point(4, 33);
			this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(1266, 64);
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
			this.panel2.Location = new System.Drawing.Point(38, 183);
			this.panel2.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.panel2.Name = "panel2";
			this.panel2.Size = new System.Drawing.Size(1271, 262);
			this.panel2.TabIndex = 0;
			// 
			// groupBox1
			// 
			this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox1.Controls.Add(this.labelRuntime1);
			this.groupBox1.Controls.Add(this.labelEnergyProduction1);
			this.groupBox1.Controls.Add(this.labelBiogasTotal);
			this.groupBox1.Controls.Add(this.labelEnergyProductionTotal);
			this.groupBox1.Controls.Add(this.labelRuntimeTotal);
			this.groupBox1.Controls.Add(this.labelRuntime2);
			this.groupBox1.Controls.Add(this.labelEnergyProduction2);
			this.groupBox1.Controls.Add(this.labelBiogas2);
			this.groupBox1.Controls.Add(this.labelBiogas1);
			this.groupBox1.Controls.Add(this.labelWorkers);
			this.groupBox1.Controls.Add(this.label16);
			this.groupBox1.Controls.Add(this.label14);
			this.groupBox1.Controls.Add(this.label17);
			this.groupBox1.Controls.Add(this.label11);
			this.groupBox1.Controls.Add(this.label10);
			this.groupBox1.Controls.Add(this.label13);
			this.groupBox1.Controls.Add(this.label12);
			this.groupBox1.Controls.Add(this.label15);
			this.groupBox1.Controls.Add(this.label9);
			this.groupBox1.Controls.Add(this.label8);
			this.groupBox1.Controls.Add(this.label7);
			this.groupBox1.Controls.Add(this.label6);
			this.groupBox1.Controls.Add(this.label5);
			this.groupBox1.Location = new System.Drawing.Point(11, 17);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(1244, 233);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "当前班次生产数据";
			// 
			// labelRuntime1
			// 
			this.labelRuntime1.Location = new System.Drawing.Point(427, 193);
			this.labelRuntime1.Name = "labelRuntime1";
			this.labelRuntime1.Size = new System.Drawing.Size(128, 25);
			this.labelRuntime1.TabIndex = 6;
			this.labelRuntime1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelEnergyProduction1
			// 
			this.labelEnergyProduction1.Location = new System.Drawing.Point(427, 144);
			this.labelEnergyProduction1.Name = "labelEnergyProduction1";
			this.labelEnergyProduction1.Size = new System.Drawing.Size(128, 25);
			this.labelEnergyProduction1.TabIndex = 6;
			this.labelEnergyProduction1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelBiogasTotal
			// 
			this.labelBiogasTotal.Location = new System.Drawing.Point(1061, 101);
			this.labelBiogasTotal.Name = "labelBiogasTotal";
			this.labelBiogasTotal.Size = new System.Drawing.Size(128, 25);
			this.labelBiogasTotal.TabIndex = 6;
			this.labelBiogasTotal.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelEnergyProductionTotal
			// 
			this.labelEnergyProductionTotal.Location = new System.Drawing.Point(1061, 144);
			this.labelEnergyProductionTotal.Name = "labelEnergyProductionTotal";
			this.labelEnergyProductionTotal.Size = new System.Drawing.Size(128, 25);
			this.labelEnergyProductionTotal.TabIndex = 6;
			this.labelEnergyProductionTotal.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelRuntimeTotal
			// 
			this.labelRuntimeTotal.Location = new System.Drawing.Point(1061, 193);
			this.labelRuntimeTotal.Name = "labelRuntimeTotal";
			this.labelRuntimeTotal.Size = new System.Drawing.Size(128, 25);
			this.labelRuntimeTotal.TabIndex = 6;
			this.labelRuntimeTotal.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelRuntime2
			// 
			this.labelRuntime2.Location = new System.Drawing.Point(736, 193);
			this.labelRuntime2.Name = "labelRuntime2";
			this.labelRuntime2.Size = new System.Drawing.Size(128, 25);
			this.labelRuntime2.TabIndex = 6;
			this.labelRuntime2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelEnergyProduction2
			// 
			this.labelEnergyProduction2.Location = new System.Drawing.Point(736, 144);
			this.labelEnergyProduction2.Name = "labelEnergyProduction2";
			this.labelEnergyProduction2.Size = new System.Drawing.Size(155, 25);
			this.labelEnergyProduction2.TabIndex = 6;
			this.labelEnergyProduction2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelBiogas2
			// 
			this.labelBiogas2.Location = new System.Drawing.Point(736, 101);
			this.labelBiogas2.Name = "labelBiogas2";
			this.labelBiogas2.Size = new System.Drawing.Size(128, 25);
			this.labelBiogas2.TabIndex = 6;
			this.labelBiogas2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelBiogas1
			// 
			this.labelBiogas1.Location = new System.Drawing.Point(427, 101);
			this.labelBiogas1.Name = "labelBiogas1";
			this.labelBiogas1.Size = new System.Drawing.Size(128, 25);
			this.labelBiogas1.TabIndex = 6;
			this.labelBiogas1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// labelWorkers
			// 
			this.labelWorkers.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.labelWorkers.Location = new System.Drawing.Point(221, 59);
			this.labelWorkers.Name = "labelWorkers";
			this.labelWorkers.Size = new System.Drawing.Size(1005, 25);
			this.labelWorkers.TabIndex = 5;
			// 
			// label16
			// 
			this.label16.AutoSize = true;
			this.label16.Location = new System.Drawing.Point(574, 193);
			this.label16.Name = "label16";
			this.label16.Size = new System.Drawing.Size(174, 25);
			this.label16.TabIndex = 4;
			this.label16.Text = "#2机组运行时间：";
			// 
			// label14
			// 
			this.label14.AutoSize = true;
			this.label14.Location = new System.Drawing.Point(920, 101);
			this.label14.Name = "label14";
			this.label14.Size = new System.Drawing.Size(152, 25);
			this.label14.TabIndex = 3;
			this.label14.Text = "总计沼气流量：";
			// 
			// label17
			// 
			this.label17.AutoSize = true;
			this.label17.Location = new System.Drawing.Point(920, 193);
			this.label17.Name = "label17";
			this.label17.Size = new System.Drawing.Size(152, 25);
			this.label17.TabIndex = 3;
			this.label17.Text = "总计运行时间：";
			// 
			// label11
			// 
			this.label11.AutoSize = true;
			this.label11.Location = new System.Drawing.Point(940, 144);
			this.label11.Name = "label11";
			this.label11.Size = new System.Drawing.Size(132, 25);
			this.label11.TabIndex = 3;
			this.label11.Text = "总计发电量：";
			// 
			// label10
			// 
			this.label10.AutoSize = true;
			this.label10.Location = new System.Drawing.Point(594, 144);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(154, 25);
			this.label10.TabIndex = 2;
			this.label10.Text = "#2机组发电量：";
			// 
			// label13
			// 
			this.label13.AutoSize = true;
			this.label13.Location = new System.Drawing.Point(596, 101);
			this.label13.Name = "label13";
			this.label13.Size = new System.Drawing.Size(152, 25);
			this.label13.TabIndex = 1;
			this.label13.Tag = "";
			this.label13.Text = "垃圾焚烧沼气：";
			// 
			// label12
			// 
			this.label12.AutoSize = true;
			this.label12.Location = new System.Drawing.Point(308, 101);
			this.label12.Name = "label12";
			this.label12.Size = new System.Drawing.Size(132, 25);
			this.label12.TabIndex = 1;
			this.label12.Tag = "";
			this.label12.Text = "填埋场沼气：";
			// 
			// label15
			// 
			this.label15.AutoSize = true;
			this.label15.Location = new System.Drawing.Point(266, 193);
			this.label15.Name = "label15";
			this.label15.Size = new System.Drawing.Size(174, 25);
			this.label15.TabIndex = 1;
			this.label15.Text = "#1机组运行时间：";
			// 
			// label9
			// 
			this.label9.AutoSize = true;
			this.label9.Location = new System.Drawing.Point(286, 144);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(154, 25);
			this.label9.TabIndex = 1;
			this.label9.Text = "#1机组发电量：";
			// 
			// label8
			// 
			this.label8.AutoSize = true;
			this.label8.Location = new System.Drawing.Point(25, 193);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(172, 25);
			this.label8.TabIndex = 0;
			this.label8.Text = "当前班次运行时间";
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(45, 144);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(152, 25);
			this.label7.TabIndex = 0;
			this.label7.Text = "当前班次发电量";
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(25, 101);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(172, 25);
			this.label6.TabIndex = 0;
			this.label6.Text = "当前班次沼气流量";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(85, 59);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(92, 25);
			this.label5.TabIndex = 0;
			this.label5.Text = "当前员工";
			// 
			// labelCurrTime
			// 
			this.labelCurrTime.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.labelCurrTime.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.labelCurrTime.Location = new System.Drawing.Point(1152, 33);
			this.labelCurrTime.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.labelCurrTime.Name = "labelCurrTime";
			this.labelCurrTime.Size = new System.Drawing.Size(100, 64);
			this.labelCurrTime.TabIndex = 1;
			this.labelCurrTime.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// labelBiogasMonth
			// 
			this.labelBiogasMonth.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.labelBiogasMonth.Location = new System.Drawing.Point(1102, 25);
			this.labelBiogasMonth.Name = "labelBiogasMonth";
			this.labelBiogasMonth.Size = new System.Drawing.Size(139, 29);
			this.labelBiogasMonth.TabIndex = 1;
			// 
			// label18
			// 
			this.label18.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label18.Location = new System.Drawing.Point(902, 54);
			this.label18.Name = "label18";
			this.label18.Size = new System.Drawing.Size(194, 31);
			this.label18.TabIndex = 1;
			this.label18.Text = "当月累计发电量：";
			// 
			// labelEnergyProductionMonth
			// 
			this.labelEnergyProductionMonth.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.labelEnergyProductionMonth.Location = new System.Drawing.Point(1102, 54);
			this.labelEnergyProductionMonth.Name = "labelEnergyProductionMonth";
			this.labelEnergyProductionMonth.Size = new System.Drawing.Size(139, 29);
			this.labelEnergyProductionMonth.TabIndex = 1;
			// 
			// EBoard
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1351, 752);
			this.Controls.Add(this.panel2);
			this.Controls.Add(this.panel1);
			this.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
			this.Name = "EBoard";
			this.Text = "EBoard";
			this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
			this.Load += new System.EventHandler(this.EBoard_Load);
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
			this.splitContainer1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.chartCurrMonth1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.chartCurrMonth2)).EndInit();
			this.panel2.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label labelCurrDate;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
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
		private System.Windows.Forms.Label labelWorkers;
		private System.Windows.Forms.Label labelRuntime1;
		private System.Windows.Forms.Label labelEnergyProduction1;
		private System.Windows.Forms.Label labelBiogasTotal;
		private System.Windows.Forms.Label labelEnergyProductionTotal;
		private System.Windows.Forms.Label labelRuntimeTotal;
		private System.Windows.Forms.Label labelRuntime2;
		private System.Windows.Forms.Label labelEnergyProduction2;
		private System.Windows.Forms.Label labelBiogas2;
		private System.Windows.Forms.Label labelBiogas1;
		private System.Windows.Forms.Label labelTotalRuntime2;
		private System.Windows.Forms.Label labelTotalRuntime1;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.DataVisualization.Charting.Chart chartCurrMonth1;
		private System.Windows.Forms.DataVisualization.Charting.Chart chartCurrMonth2;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label labelCurrTime;
		private System.Windows.Forms.Label labelEnergyProductionMonth;
		private System.Windows.Forms.Label labelBiogasMonth;
		private System.Windows.Forms.Label label18;
	}
}

