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
using System.Collections.Generic;

namespace EBoard
{
	public partial class MainForm : Form
	{
		#region Variables and properties
		private readonly Logger logger = NLog.LogManager.GetCurrentClassLogger();

		private const string UnitM3 = @" M³";
		private const string UnitKWh = " kWh";
		private const int WorkersInShift = 2;
		private const int DefaultRefreshInterval = 2000;

		private int refreshInterval = DefaultRefreshInterval;

		private System.Threading.Timer refreshDataTimer;
		private System.Windows.Forms.Timer refreshTimeTimer;

		private string firstShiftStart;
		private string secondShiftStart;
		private System.Threading.Timer shiftLoginTimer;

		private SqlConnection connection;
				
		private DateTime? lastUpdateTime;
		/// <summary>
		/// The last time data got changed
		/// </summary>
		public DateTime? LastUpdateTime
		{
			get { return lastUpdateTime; }
			set
			{
				lastUpdateTime = value;

				if (InvokeRequired)
				{
					BeginInvoke((MethodInvoker)(() => Text = string.Format("最后更新于：{0}", (lastUpdateTime.HasValue) ? lastUpdateTime.Value.ToLongTimeString() : "")));
					return;
				}
				else
				{
					Text = string.Format("最后更新于：{0}", (lastUpdateTime.HasValue) ? lastUpdateTime.Value.ToLongTimeString() : "");
				}
			}
		}

		/// <summary>
		/// How many days in current month
		/// </summary>
		private int daysInMonth;

		private readonly object workersLock = new object();
		private SortedList<DateTime, User> currentWorkers = new SortedList<DateTime, User>();
		public SortedList<DateTime, User> CurrentWorkers
		{
			get { return currentWorkers; }
			set
			{
				lock (workersLock)
				{
					if (currentWorkers == value)
						return;

					currentWorkers = value;
					UpdateWorksOnGUI();
				}
			}
		}
		#endregion

		public MainForm()
		{
			InitializeComponent();
		}

		private void MainForm_Load(object sender, EventArgs e)
		{
			logger.Info("Electronic Board System started.");

			InitToolTips();

			try
			{
				connection = DbFactory.GetConnection();

				var now = DateTime.Now;
				daysInMonth = DateTime.DaysInMonth(now.Year, now.Month);
				InitChart(chartCurrMonth1, 1, 18);
				InitChart(chartCurrMonth2, 19, daysInMonth);

				InitTimers();

				CurrentWorkers = new Dal(connection).GetWorkersOfShift();
			}
			catch (Exception ex)
			{
				logger.Error(ex, "Error occurred while initializing.");
				MessageBox.Show(string.Format("程序初始化出错. {0}", ex));
			}
		}

		private void InitTimers()
		{
			refreshTimeTimer = new System.Windows.Forms.Timer();
			refreshTimeTimer.Interval = 1000;
			refreshTimeTimer.Tick += RefreshTimeTimer_Tick;
			refreshTimeTimer.Enabled = true;

			var dal = new Dal(connection);
			var parameters = dal.GetGeneralParameters();
			var shift1Start = parameters.FirstOrDefault(p => string.Equals(p.Category, "System", StringComparison.OrdinalIgnoreCase)
															&& string.Equals(p.Name, "ShiftStartTime1", StringComparison.OrdinalIgnoreCase));
			firstShiftStart = (shift1Start != null) ? shift1Start.Value : "8:00:00";

			var shift2Start = parameters.FirstOrDefault(p => string.Equals(p.Category, "System", StringComparison.OrdinalIgnoreCase)
															&& string.Equals(p.Name, "ShiftStartTime2", StringComparison.OrdinalIgnoreCase));
			secondShiftStart = (shift1Start != null) ? shift2Start.Value : "20:00:00";


			shiftLoginTimer = new System.Threading.Timer(LoginTimerCallback, null, GetDueTimeForLoginTimer(), Timeout.Infinite);

			var param = parameters.FirstOrDefault(p => string.Equals(p.Category, "System", StringComparison.OrdinalIgnoreCase)
													&& string.Equals(p.Name, "RefreshDataInterval", StringComparison.OrdinalIgnoreCase));
			if (param != null)
			{
				if (!int.TryParse(param.Value, out refreshInterval))
					logger.Error("Failed to convert value string '{0}' of RefreshDataInterval to integer, use the default value {1}", param.Value, DefaultRefreshInterval);
			}
			refreshDataTimer = new System.Threading.Timer(RefreshDataTimerCallback, null, 0, Timeout.Infinite);
		}

		private void InitToolTips()
		{
			toolTip.SetToolTip(labelTitle, "可以双击鼠标，进行工班登录");
			toolTip.SetToolTip(panelIndicator, "指示当前通讯状态，绿：正常 黄：正在通讯 红：通讯失败");
			toolTip.SetToolTip(labelCurrDate, "可以双击鼠标，进行全屏与普通窗口模式切换");
			toolTip.SetToolTip(labelCurrWeekDay, "可以双击鼠标，进行全屏与普通窗口模式切换");
			toolTip.SetToolTip(labelCurrTime, "可以双击鼠标，进行全屏与普通窗口模式切换");
		}

		#region For LoginTimer
		private int GetDueTimeForLoginTimer()
		{
			var now = DateTime.Now;
			var today = now.ToString("yyyy-MM-dd") + " ";
			var firstShiftStartTime = DateTime.Parse(today + firstShiftStart);
			var secondShiftStartTime = DateTime.Parse(today + secondShiftStart);

			DateTime triggerTime;
			if (now <= firstShiftStartTime)
			{
				triggerTime = firstShiftStartTime;
			}
			else if (now > firstShiftStartTime && now <= secondShiftStartTime)
			{
				triggerTime = secondShiftStartTime;
			}
			else
			{
				triggerTime = firstShiftStartTime.AddDays(1);
			}

			var dueTime = (triggerTime - now).TotalMilliseconds;

			return (int)dueTime;
		}

		private void LoginTimerCallback(object state)
		{
			shiftLoginTimer.Change(Timeout.Infinite, Timeout.Infinite);

			try
			{
				if (CurrentWorkers != null)
				{
					CurrentWorkers.Clear();
				}

				Login("请登录当前工班 - 第一名员工");
				Login("请登录当前工班 - 第二名员工");
			}
			finally
			{
				shiftLoginTimer.Change(GetDueTimeForLoginTimer(), Timeout.Infinite);
			}
		}

		private User Login(string title)
		{
			var loginForm = new LoginDlg(connection) { Text = title, AdditionalCheckAfterValidated = CheckUserForCurrentShift };
			try
			{
				if (loginForm.ShowDialog() != DialogResult.OK)
					return null;

				return loginForm.CurrentUser;
			}
			finally
			{
				loginForm.AdditionalCheckAfterValidated = null;
				loginForm.Close();
			}
		}

		private bool CheckUserForCurrentShift(User user)
		{
			var dal = new Dal(connection);
			var currentShift = dal.GetCurrentShiftId();
			var loginUserList = dal.GetWorkersOfShift(currentShift);
			var found = loginUserList.FirstOrDefault(u => string.Equals(u.Value.LoginId, user.LoginId, StringComparison.OrdinalIgnoreCase));
			if (found.Value != null)    // User already logged in
			{
				CurrentWorkers = loginUserList;
				return true;
			}

			var ret = false;
			if ((loginUserList.Count < WorkersInShift))
			{
				ret = dal.AddWorkerInShift(currentShift, user);
			}
			else if (MessageBox.Show("已经有两个用户与当前班组关联，如果继续登录则会用当前用户覆盖较早登录者，确认登录吗？", "登录", MessageBoxButtons.YesNo) == DialogResult.Yes)
			{
				ret = dal.UpdateWorkerInShift(currentShift, loginUserList.Values[0].LoginId, user);
			}

			CurrentWorkers = dal.GetWorkersOfShift(currentShift);

			return ret;
		}
		#endregion

		private void RefreshTimeTimer_Tick(object sender, EventArgs e)
		{
			var now = DateTime.Now;
			labelCurrDate.Text = now.ToString("yyyy年MM月dd日", new System.Globalization.CultureInfo("zh-cn"));
			labelCurrWeekDay.Text = now.ToString("dddd", new System.Globalization.CultureInfo("zh-cn"));
			labelCurrTime.Text = now.ToLocalTime().ToString("HH:mm:ss");
		}
		
		private void RefreshDataTimerCallback(object state)
		{
			refreshDataTimer.Change(Timeout.Infinite, Timeout.Infinite);
			try
			{
				SetCommunicateState(CommunicationState.Querying);
				var conn = DbFactory.GetConnection();
				var dal = new Dal(conn);
				var shiftStatInfo = dal.GetShiftStatInfo(LastUpdateTime);
				if (shiftStatInfo == null)
				{
					SetCommunicateState(CommunicationState.ErrorOccurred);
					return;
				}

				// Need to refresh two charts the first time
				var alwaysRefresh = (!LastUpdateTime.HasValue);

				LastUpdateTime = shiftStatInfo.LastUpdateTime;

				RefreshData(shiftStatInfo);

				var reporter = new Reporter(conn);
				var ds = reporter.GetCurrentMonthDataByDay();
				RefreshCharts(ds, alwaysRefresh);
				SetCommunicateState(CommunicationState.Ready);
			}
			catch (OPCCommunicationBrokeException)
			{
				SetCommunicateState(CommunicationState.CommunicationBroke);
			}
			catch (Exception ex)
			{
				logger.Error("Error occurred while getting data. {0}", ex.ToString());
				SetCommunicateState(CommunicationState.ErrorOccurred);
			}
			finally
			{
				refreshDataTimer.Change(refreshInterval, Timeout.Infinite);
			}
		}

		private void SetCommunicateState(CommunicationState state)
		{
			switch (state)
			{
				case CommunicationState.Ready:
					panelIndicator.BackgroundImage = global::EBoard.Properties.Resources.green;
					break;
				case CommunicationState.Querying:
					panelIndicator.BackgroundImage = global::EBoard.Properties.Resources.yellow;
					break;
				case CommunicationState.CommunicationBroke:
				case CommunicationState.ErrorOccurred:
					panelIndicator.BackgroundImage = global::EBoard.Properties.Resources.red;
					break;
			}
		}

		private void RefreshData(ShiftStatInfo data)
		{
			if (InvokeRequired)
			{
				BeginInvoke((MethodInvoker)(() => RefreshData(data)));
				return;
			}

			double? 
					//biogas2Torch = null,
					//biogas2Gen = null,
					biogas2TorchSubtotal = null,
					biogas2GenSubtotal = null,
					subtotalRuntime1 = null,
					subtotalRuntime2 = null,
					energyProduction1 = null,
					energyProduction2 = null,
					//generatorPower1 = null,
					//generatorPower2 = null,
					totalRuntime1 = null,
					totalRuntime2 = null;
			bool? generator1Running = null,
				generator2Running = null;

			if (data.MonitorItems.ContainsKey(ShiftStatInfo.Generator1RunningColName))
				generator1Running = ((int)data.MonitorItems[ShiftStatInfo.Generator1RunningColName]) != 0;

			if (data.MonitorItems.ContainsKey(ShiftStatInfo.Generator2RunningColName))
				generator2Running = ((int)data.MonitorItems[ShiftStatInfo.Generator2RunningColName]) != 0;

			/// 1. instantaneous values
			// 1.1 Biogas
			//if (data.MonitorItems.ContainsKey(ShiftStatInfo.Biogas2TorchColName))
			//	biogas2Torch = data.MonitorItems[ShiftStatInfo.Biogas2TorchColName];

			//if (data.MonitorItems.ContainsKey(ShiftStatInfo.Biogas2GenColName))
			//	biogas2Gen = data.MonitorItems[ShiftStatInfo.Biogas2GenColName];

			// 1.2 Generator power P
			//if (data.MonitorItems.ContainsKey(ShiftStatInfo.GeneratorPower1ColName))
			//	generatorPower1 = data.MonitorItems[ShiftStatInfo.GeneratorPower1ColName];

			//if (data.MonitorItems.ContainsKey(ShiftStatInfo.GeneratorPower2ColName))
			//	generatorPower2 = data.MonitorItems[ShiftStatInfo.GeneratorPower2ColName];

			/// 2. accumulated values
			// 2.1 Subtotal biogas of current shift
			if (data.StatInfo.ContainsKey(ShiftStatInfo.Biogas2TorchSubtotalColName))
				biogas2TorchSubtotal = data.StatInfo[ShiftStatInfo.Biogas2TorchSubtotalColName];

			if (data.StatInfo.ContainsKey(ShiftStatInfo.Biogas2GenSubtotalColName))
				biogas2GenSubtotal = data.StatInfo[ShiftStatInfo.Biogas2GenSubtotalColName];

			// 2.2 Energy production of current shift
			if (data.StatInfo.ContainsKey(ShiftStatInfo.EnergyProduction1ColName))
				energyProduction1 = data.StatInfo[ShiftStatInfo.EnergyProduction1ColName];

			if (data.StatInfo.ContainsKey(ShiftStatInfo.EnergyProduction2ColName))
				energyProduction2 = data.StatInfo[ShiftStatInfo.EnergyProduction2ColName];

			// 2.3 subtotal runtime of current shift
			if (data.StatInfo.ContainsKey(ShiftStatInfo.SubtotalRuntime1ColName))
				subtotalRuntime1 = data.StatInfo[ShiftStatInfo.SubtotalRuntime1ColName];
						
			if (data.StatInfo.ContainsKey(ShiftStatInfo.SubtotalRuntime2ColName))
				subtotalRuntime2 = data.StatInfo[ShiftStatInfo.SubtotalRuntime2ColName];

			// 2.4 total runtime
			if (data.StatInfo.ContainsKey(ShiftStatInfo.TotalRuntime1ColName))
				totalRuntime1 = data.StatInfo[ShiftStatInfo.TotalRuntime1ColName];

			if (data.StatInfo.ContainsKey(ShiftStatInfo.TotalRuntime2ColName))
				totalRuntime2 = data.StatInfo[ShiftStatInfo.TotalRuntime2ColName];

			// Update labels on GUI
			// 2.4
			labelTotalRuntime1.Text = totalRuntime1.HasValue ? totalRuntime1.ToString() : "";
			labelTotalRuntime2.Text = totalRuntime2.HasValue ? totalRuntime2.ToString() : "";

			// 2.1
			labelBiogas2Torch.Text = biogas2TorchSubtotal.HasValue ? biogas2TorchSubtotal.ToString() + UnitM3 : "";
			labelBiogas2Gen.Text = biogas2GenSubtotal.HasValue ? biogas2GenSubtotal.ToString() + UnitM3 : "";
			labelBiogasTotal.Text = ((biogas2TorchSubtotal ?? 0.0) + (biogas2GenSubtotal ?? 0.0)).ToString() + UnitM3;

			// 2.2
			labelEnergyProduction1.Text = energyProduction1.HasValue ? energyProduction1.ToString() + UnitKWh : "";
			labelEnergyProduction2.Text = energyProduction2.HasValue ? energyProduction2.ToString() + UnitKWh : "";
			labelEnergyProductionTotal.Text = ((energyProduction1 ?? 0.0) + (energyProduction2 ?? 0.0)).ToString() + UnitKWh;

			// 2.3
			labelRuntime1.Text = subtotalRuntime1.HasValue ? subtotalRuntime1.ToString() : "";
			labelRuntime2.Text = subtotalRuntime2.HasValue ? subtotalRuntime2.ToString() : "";
			labelRuntimeTotal.Text = ((subtotalRuntime1 ?? 0.0) + (subtotalRuntime2 ?? 0.0)).ToString();
		}

		private void UpdateWorksOnGUI()
		{
			if (CurrentWorkers.Count < 1)
			{
				labelWorker.Text = "";
				return;
			}

			if (CurrentWorkers.Count > 0)
			{
				labelWorker.Text = string.Format("姓名：{0}    工号：{1}", CurrentWorkers.Values[0].Name, CurrentWorkers.Values[0].LoginId);
			}

			if (CurrentWorkers.Count > 1)
			{
				labelWorker.Text = string.Format("{0}        姓名：{1}    工号：{2}", labelWorker.Text, CurrentWorkers.Values[1].Name, CurrentWorkers.Values[1].LoginId);
			}
		}

		#region For Chart
		private void InitChart(Chart chart, int begin, int end)
		{
			chart.ChartAreas[0].AxisX.Minimum = begin;
			chart.ChartAreas[0].AxisX.Maximum = end;
			chart.ChartAreas[0].AxisX.Interval = 1;
			chart.BackColor = Color.Transparent;
			chart.Legends.Clear();

			while (chart.Series.Count > 0)
			{
				chart.Series.RemoveAt(0);
			}

			var series1 = chart.Series.Add("沼气量日");
			series1.XValueMember = "Day";
			series1.YValueMembers = "DayBiogas";
			series1.BackHatchStyle = ChartHatchStyle.None;
			series1.Color = Color.FromArgb(72, 130, 189);
			series1.BorderColor = Color.Black;
			series1.LabelBackColor = Color.Transparent;
			series1.CustomProperties = "PointWidth=0.8";

			var series2 = chart.Series.Add("发电量日");
			series2.XValueMember = "Day";
			series2.YValueMembers = "DayEngeryProduction";
			series2.BackHatchStyle = ChartHatchStyle.None;
			series2.Color = Color.FromArgb(147, 37, 32);
			series2.BorderColor = Color.Black;
			series2.CustomProperties = "PointWidth=0.8";

			var series3 = chart.Series.Add("沼气量夜");
			series3.XValueMember = "Day";
			series3.YValueMembers = "NightBiogas";
			series3.BackHatchStyle = ChartHatchStyle.None;
			series3.Color = Color.FromArgb(22, 52, 121);
			series3.BorderColor = Color.Black;
			series3.CustomProperties = "PointWidth=0.8";

			var series4 = chart.Series.Add("发电量夜");
			series4.XValueMember = "Day";
			series4.YValueMembers = "NightEngeryProduction";
			series4.BackHatchStyle = ChartHatchStyle.None;
			series4.Color = Color.FromArgb(120, 30, 25);
			series4.BorderColor = Color.Black;
			series4.CustomProperties = "PointWidth=0.8";

			while (chart.ChartAreas[0].AxisX.CustomLabels.Count > 0)
			{
				chart.ChartAreas[0].AxisX.CustomLabels.RemoveAt(0);
			}

			//chart.ChartAreas[0].AxisX.CustomLabels.Add(begin - 1 - 0.5, begin - 0.5, "日期", 0, LabelMarkStyle.None);
			//chart.ChartAreas[0].AxisX.CustomLabels.Add(begin - 1 - 0.5, begin - 0.5, "沼气量日", 1, LabelMarkStyle.None);
			//chart.ChartAreas[0].AxisX.CustomLabels.Add(begin - 1 - 0.5, begin - 0.5, "发电量日", 2, LabelMarkStyle.None);
			//chart.ChartAreas[0].AxisX.CustomLabels.Add(begin - 1 - 0.5, begin - 0.5, "当班人员", 3, LabelMarkStyle.None);
			//chart.ChartAreas[0].AxisX.CustomLabels.Add(begin - 1 - 0.5, begin - 0.5, "", 4, LabelMarkStyle.None);
			//chart.ChartAreas[0].AxisX.CustomLabels.Add(begin - 1 - 0.5, begin - 0.5, "沼气量夜", 5, LabelMarkStyle.None);
			//chart.ChartAreas[0].AxisX.CustomLabels.Add(begin - 1 - 0.5, begin - 0.5, "发电量夜", 6, LabelMarkStyle.None);
			//chart.ChartAreas[0].AxisX.CustomLabels.Add(begin - 1 - 0.5, begin - 0.5, "当班人员", 7, LabelMarkStyle.None);
			//chart.ChartAreas[0].AxisX.CustomLabels.Add(begin - 1 - 0.5, begin - 0.5, "", 8, LabelMarkStyle.None);

			for (var i = begin; i <= end; i++)
			{
				var from = i - 0.5;
				var to = i + 0.5;

				chart.ChartAreas[0].AxisX.CustomLabels.Add(from, to, i.ToString(), 0, LabelMarkStyle.None);     // day
				chart.ChartAreas[0].AxisX.CustomLabels.Add(from, to, "", 1, LabelMarkStyle.None).Tag = i;       // biogas day
				chart.ChartAreas[0].AxisX.CustomLabels.Add(from, to, "", 2, LabelMarkStyle.None).Tag = i;       // kwh day
				chart.ChartAreas[0].AxisX.CustomLabels.Add(from, to, "", 3, LabelMarkStyle.None).Tag = i;       // worker1
				chart.ChartAreas[0].AxisX.CustomLabels.Add(from, to, "", 4, LabelMarkStyle.None).Tag = i;       // worker2

				chart.ChartAreas[0].AxisX.CustomLabels.Add(from, to, "", 5, LabelMarkStyle.None).Tag = i;       // biogas night
				chart.ChartAreas[0].AxisX.CustomLabels.Add(from, to, "", 6, LabelMarkStyle.None).Tag = i;       // kwh night
				chart.ChartAreas[0].AxisX.CustomLabels.Add(from, to, "", 7, LabelMarkStyle.None).Tag = i;       // worker1
				chart.ChartAreas[0].AxisX.CustomLabels.Add(from, to, "", 8, LabelMarkStyle.None).Tag = i;       // worker2

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
				InitChart(chartCurrMonth2, 19, lastDay);
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

		private Dictionary<int, string> ChartLabelMapping = new Dictionary<int, string>
		{
			{1, "DayBiogas" }
			,{2, "DayEngeryProduction" }
			,{3, "DayWorkers1" }
			,{4, "DayWorkers2" }
			,{5, "NightBiogas" }
			,{6, "NightEngeryProduction" }
			,{7, "NightWorkers1" }
			,{8, "NightWorkers2" }
		};

		private void RefreshChart(Chart chart, DataSet ds)
		{
			try
			{
				// Refresh to total data
				if (ds.Tables.Count > 1 && ds.Tables[1].Rows.Count > 0)
				{
					var sumRow = ds.Tables[1].Rows[0];
					labelBiogasMonth.Text = sumRow["Biogas"].ToString() + UnitM3;
					labelEnergyProductionMonth.Text = sumRow["EngeryProduction"].ToString() + UnitKWh;
				}

				// Refresh the chart label
				var axis = chart.ChartAreas[0].AxisX;
				var begin = axis.Minimum;
				var end = axis.Maximum;
				var today = DateTime.Now.Day;

				chart.DataSource = ds.Tables[0].Select(string.Format("Day>={0} And Day<={1}", begin, end));

				for (var i = begin; i <= end; i++)
				{
					var row = ds.Tables[0].AsEnumerable().FirstOrDefault(d => (int)d["Day"] == i);
					if (row == null) continue;

					var labels = axis.CustomLabels.Where(c => c.RowIndex > 0 && c.Tag != null && (int)c.Tag == i).ToList();
					foreach (var label in labels)
					{
						if (today < i)
						{
							label.Text = "";    // No need to write the value since the day is later than today
							continue;
						}

						if (!ChartLabelMapping.ContainsKey(label.RowIndex))
							continue;

						var colName = ChartLabelMapping[label.RowIndex];
						if (colName.EndsWith("1") || colName.EndsWith("2"))
						{
							// Need to split the workers
							var realColumn = colName.Substring(0, colName.Length - 1);
							var workerIndex = int.Parse(colName.Substring(colName.Length - 1));
							var workers = row[realColumn].ToString();
							if (string.IsNullOrWhiteSpace(workers))
								continue;

							var workerList = workers.Split(new string[] { "," }, StringSplitOptions.RemoveEmptyEntries);
							if (workerList.Length >= workerIndex)
								label.Text = workerList[workerIndex - 1];
						}
						else
						{
							label.Text = row[ChartLabelMapping[label.RowIndex]].ToString();
						}
					}
				}
			}
			catch (Exception ex)
			{
				logger.Error(ex, "Error occurred while refreshing chart.");
			}
		}
		#endregion

		private void label1_DoubleClick(object sender, EventArgs e)
		{
			if (currentWorkers.Count < 1)
			{
				Login("请登录当前工班 - 第一名员工");
			}

			Login("请登录当前工班 - 第二名员工");
		}

		private void labelCurrent_DoubleClick(object sender, EventArgs e)
		{
			var newStyle = (FormBorderStyle == FormBorderStyle.None) ? FormBorderStyle.Sizable : FormBorderStyle.None;
			FormBorderStyle = newStyle;
		}
	}

	public enum CommunicationState
	{
		Querying,
		Ready,
		CommunicationBroke,
		ErrorOccurred
	}
}
