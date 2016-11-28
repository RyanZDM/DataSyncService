using System;
using System.Data;
using System.Linq;
using System.Windows.Forms;
using System.Threading;
using NLog;
using System.Windows.Forms.DataVisualization.Charting;
using System.Drawing;
using EBoard.Common;
using System.Data.SqlClient;

namespace EBoard
{
	public partial class MainForm : Form
	{
		private readonly Logger logger = NLog.LogManager.GetCurrentClassLogger();

		private System.Threading.Timer refreshDataTimer;
		private System.Windows.Forms.Timer refreshTimeTimer;

		private SqlConnection connection;

		/// <summary>
		/// The last time data got changed
		/// </summary>
		private DateTime? lastUpdateTime;

		/// <summary>
		/// How many days in current month
		/// </summary>
		private int daysInMonth;

		public User CurrentUser { get; set; }

		public MainForm()
		{
			connection = DbFactory.GetConnection();

			var loginForm = new Login(connection);
			loginForm.AdditionalCheckAfterValidated = CheckUserForCurrentShift;
			if (loginForm.ShowDialog() != DialogResult.OK)
			{
				Close();
				return;
			}

			CurrentUser = loginForm.CurrentUser;

			InitializeComponent();
		}

		private void MainForm_Load(object sender, EventArgs e)
		{
			logger.Info("Electronic Board System started.");

			try
			{
				var now = DateTime.Now;
				labelCurrDate.Text = now.ToString("yyyy年MM月dd日 dddd", new System.Globalization.CultureInfo("zh-cn"));
				refreshTimeTimer = new System.Windows.Forms.Timer();
				refreshTimeTimer.Interval = 1000;
				refreshTimeTimer.Tick += RefreshTimeTimer_Tick;
				refreshTimeTimer.Enabled = true;

				daysInMonth = DateTime.DaysInMonth(now.Year, now.Month);
				InitChart(chartCurrMonth1, 1, 19);
				InitChart(chartCurrMonth2, 20, daysInMonth);

				refreshDataTimer = new System.Threading.Timer(RefreshDataTimerCallback, null, 0, Timeout.Infinite);
			}
			catch (Exception ex)
			{
				logger.Error(ex, "Error occurred while initializing.");
				MessageBox.Show(string.Format("程序初始化出错. {0}", ex));
			}
		}

		private void RefreshTimeTimer_Tick(object sender, EventArgs e)
		{
			labelCurrTime.Text = DateTime.Now.ToLocalTime().ToString("HH:mm:ss");
		}

		private void RefreshDataTimerCallback(object state)
		{
			refreshDataTimer.Change(Timeout.Infinite, Timeout.Infinite);
			try
			{
				var conn = DbFactory.GetConnection();
				var dal = new Dal(conn);
				var shiftStatInfo = dal.GetShiftStatInfo(lastUpdateTime);
				if (shiftStatInfo == null)
				{
					// TODO: show error info on GUI
					return;
				}

				// Need to refresh two charts the first time
				var alwaysRefresh = (!lastUpdateTime.HasValue);

				lastUpdateTime = shiftStatInfo.LastUpdateTime;

				RefreshData(shiftStatInfo);

				var reporter = new Reporter(conn);
				var ds = reporter.GetCurrentMonthDataByDay();
				RefreshCharts(ds, alwaysRefresh);
			}
			finally
			{
				refreshDataTimer.Change(2000, Timeout.Infinite);
			}
		}

		private void RefreshData(ShiftStatInfo data)
		{
			if (InvokeRequired)
			{
				BeginInvoke((MethodInvoker)(() => RefreshData(data)));
				return;
			}

			labelWorkers.Text = string.Format("姓名 {0}  工号 {1}    ", data.LastLoginId, data.LastLoginName);

			double? biogas1 = null,
					biogas2 = null,
					biogasSubtotal1 = null,
					biogasSubtotal2 = null,
					totalRuntime1 = null,
					totalRuntime2 = null,
					energyProduction1 = null,
					energyProduction2 = null,
					generatorPower1 = null,
					generatorPower2 = null;

			// instantaneous values
			if (data.MonitorItems.ContainsKey(ShiftStatInfo.Biogas1ColName))
				biogas1 = data.MonitorItems[ShiftStatInfo.Biogas1ColName];

			if (data.MonitorItems.ContainsKey(ShiftStatInfo.Biogas2ColName))
				biogas1 = data.MonitorItems[ShiftStatInfo.Biogas2ColName];

			if (data.MonitorItems.ContainsKey(ShiftStatInfo.GeneratorPower1ColName))
				generatorPower1 = data.MonitorItems[ShiftStatInfo.GeneratorPower1ColName];

			if (data.MonitorItems.ContainsKey(ShiftStatInfo.GeneratorPower2ColName))
				generatorPower2 = data.MonitorItems[ShiftStatInfo.GeneratorPower2ColName];

			// accumulated values
			if (data.StatInfo.ContainsKey(ShiftStatInfo.BiogasSubtotal1ColName))
				biogasSubtotal1 = data.StatInfo[ShiftStatInfo.BiogasSubtotal1ColName];

			if (data.StatInfo.ContainsKey(ShiftStatInfo.BiogasSubtotal2ColName))
				biogasSubtotal2 = data.StatInfo[ShiftStatInfo.BiogasSubtotal2ColName];

			if (data.StatInfo.ContainsKey(ShiftStatInfo.EnergyProduction1ColName))
				energyProduction1 = data.StatInfo[ShiftStatInfo.EnergyProduction1ColName];

			if (data.StatInfo.ContainsKey(ShiftStatInfo.EnergyProduction2ColName))
				energyProduction2 = data.StatInfo[ShiftStatInfo.EnergyProduction2ColName];

			if (data.StatInfo.ContainsKey(ShiftStatInfo.TotalRuntime1ColName))
				totalRuntime1 = data.StatInfo[ShiftStatInfo.TotalRuntime1ColName];

			if (data.StatInfo.ContainsKey(ShiftStatInfo.TotalRuntime2ColName))
				totalRuntime2 = data.StatInfo[ShiftStatInfo.TotalRuntime2ColName];

			// Update labels on GUI

			labelTotalRuntime1.Text = totalRuntime1.HasValue ? totalRuntime1.ToString() : "";
			labelTotalRuntime2.Text = totalRuntime2.HasValue ? totalRuntime2.ToString() : "";

			labelBiogas1.Text = biogasSubtotal1.HasValue ? biogasSubtotal1.ToString() : "";
			labelBiogas2.Text = biogasSubtotal2.HasValue ? biogasSubtotal2.ToString() : "";
			labelBiogasTotal.Text = ((biogasSubtotal1 ?? 0.0) + (biogasSubtotal2 ?? 0.0)).ToString();

			labelEnergyProduction1.Text = energyProduction1.HasValue ? energyProduction1.ToString() : "";
			labelEnergyProduction2.Text = energyProduction2.HasValue ? energyProduction2.ToString() : "";
			labelEnergyProductionTotal.Text = ((energyProduction1 ?? 0.0) + (energyProduction2 ?? 0.0)).ToString();

			labelRuntime1.Text = totalRuntime1.HasValue ? totalRuntime1.ToString() : "";
			labelRuntime2.Text = totalRuntime2.HasValue ? totalRuntime2.ToString() : "";
			labelRuntimeTotal.Text = ((totalRuntime1 ?? 0.0) + (totalRuntime2 ?? 0.0)).ToString();
		}

		private bool CheckUserForCurrentShift(User user)
		{
			var dal = new Dal(connection);
			var lastLoginUser = dal.GetUserOfCurrentShift();
			if ((lastLoginUser == null))
			{
				dal.SetUserOfCurrentShift(CurrentUser);
				return true;
			}

			if (string.Equals(user.LoginId, lastLoginUser.LoginId, StringComparison.OrdinalIgnoreCase))
				return true;

			if (MessageBox.Show("已经有一个用户与当前班组关联，如果继续登录则会用当前用户覆盖关联关系，确认登录吗？", "登录", MessageBoxButtons.YesNo) == DialogResult.Yes)
			{
				dal.SetUserOfCurrentShift(user);
				return true;
			}

			return false;
		}

		#region For Chart
		private void InitChart(Chart chart, int begin, int end)
		{
			chart.ChartAreas[0].AxisX.Minimum = begin - 1;
			chart.ChartAreas[0].AxisX.Maximum = end;
			chart.ChartAreas[0].AxisX.Interval = 1;

			while (chart.Series.Count > 0)
			{
				chart.Series.RemoveAt(0);
			}

			var series1 = chart.Series.Add("沼气量");

			series1.XValueMember = "Day";
			series1.YValueMembers = "Biogas";
			series1.BackHatchStyle = ChartHatchStyle.DarkUpwardDiagonal;
			series1.Color = Color.Black;
			series1.BorderColor = Color.Black;

			var series2 = chart.Series.Add("发电量");
			series2.XValueMember = "Day";
			series2.YValueMembers = "EngeryProduction";
			series2.BackHatchStyle = ChartHatchStyle.SmallGrid;
			series2.Color = Color.Black;
			series2.BorderColor = Color.Black;

			while (chart.ChartAreas[0].AxisX.CustomLabels.Count > 0)
			{
				chart.ChartAreas[0].AxisX.CustomLabels.RemoveAt(0);
			}

			chart.ChartAreas[0].AxisX.CustomLabels.Add(begin - 1 - 0.5, begin - 0.5, "日期", 0, LabelMarkStyle.None);
			chart.ChartAreas[0].AxisX.CustomLabels.Add(begin - 1 - 0.5, begin - 0.5, "沼气量", 1, LabelMarkStyle.None);
			chart.ChartAreas[0].AxisX.CustomLabels.Add(begin - 1 - 0.5, begin - 0.5, "发电量", 2, LabelMarkStyle.None);

			for (var i = begin; i <= end; i++)
			{
				var from = i - 0.5;
				var to = i + 0.5;

				chart.ChartAreas[0].AxisX.CustomLabels.Add(from, to, i.ToString(), 0, LabelMarkStyle.None);
				chart.ChartAreas[0].AxisX.CustomLabels.Add(from, to, "", 1, LabelMarkStyle.None).Tag = i;
				chart.ChartAreas[0].AxisX.CustomLabels.Add(from, to, "", 2, LabelMarkStyle.None).Tag = i;
			}
		}

		private void RefreshCharts(DataSet ds, bool alwaysRefresh = false)
		{
			if (InvokeRequired)
			{
				BeginInvoke((MethodInvoker)(() => RefreshCharts(ds, alwaysRefresh)));
				return;
			}

			var now = DateTime.Now;

			// If month changed, then last day may changed, call InitChart()
			var lastDay = DateTime.DaysInMonth(now.Year, now.Month);
			if (daysInMonth != lastDay)
			{
				InitChart(chartCurrMonth2, 20, lastDay);
				daysInMonth = lastDay;
			}

			if (alwaysRefresh)
			{
				RefreshChart(chartCurrMonth1, ds);
				RefreshChart(chartCurrMonth2, ds);
				return;
			}

			// No need to refresh another series if today is not in that period
			var currDay = now.Day;
			if (currDay <= chartCurrMonth1.ChartAreas[0].AxisX.Maximum)
			{
				RefreshChart(chartCurrMonth1, ds);
			}
			else
			{
				RefreshChart(chartCurrMonth2, ds);
			}
		}

		private void RefreshChart(Chart chart, DataSet ds)
		{
			var axis = chart.ChartAreas[0].AxisX;
			var begin = axis.Minimum + 1;
			var end = axis.Maximum;

			chart.DataSource = ds.Tables[0].Select(string.Format("Day>={0} And Day<={1}", begin, end));

			for (var i = begin; i <= end; i++)
			{
				var row = ds.Tables[0].AsEnumerable().FirstOrDefault(d => (int)d["Day"] == i);
				if (row == null) continue;

				var labels = axis.CustomLabels.Where(c => c.RowIndex > 0 && c.Tag != null && (int)c.Tag == i).ToList();
				foreach (var label in labels)
				{
					if (label.RowIndex == 1)
					{
						label.Text = row["Biogas"].ToString();
					}
					else
					{
						label.Text = row["EngeryProduction"].ToString();
					}
				}
			}

			if (ds.Tables.Count < 1 || ds.Tables[1].Rows.Count < 1)
				return;

			var sumRow = ds.Tables[1].Rows[0];
			labelBiogasMonth.Text = sumRow["Biogas"].ToString();
			labelEnergyProductionMonth.Text = sumRow["EngeryProduction"].ToString();
		}
		#endregion
	}
}
