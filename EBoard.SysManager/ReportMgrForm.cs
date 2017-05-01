using EBoard.Common;
using NLog;
using System;
using System.Data;
using System.Data.SqlClient;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace EBoard.SysManager
{
	public partial class ReportMgrForm : FormBase
	{
		private readonly Logger logger = LogManager.GetCurrentClassLogger();

		private SqlConnection connection;

		private Reporter reporter;

		public ReportMgrForm()
		{
			InitializeComponent();
		}

		private void ReportMgrForm_Load(object sender, EventArgs e)
		{
			connection = DbFactory.GetConnection();
			reporter = new Reporter(connection);

			RefreshData();
		}

		private void RefreshData(string reportId = null)
		{
			var original = Cursor.Current;
			Cursor = Cursors.WaitCursor;

			var nodeToSelect = InitTree(reportId);
			if (nodeToSelect != null)
			{
				treeView1.SelectedNode = nodeToSelect;
			}

			Cursor = original;
		}
		
		private TreeNode InitTree(string reportId = null)
		{
			// Will also check if need to enable the button for ceating last month report
			var lastMonth = DateTime.Now.AddMonths(-1);
			var yearForLastMonth = lastMonth.Year;
			var monthForLastMonth = lastMonth.Month;
			var lastMonthReportCreated = false;

			TreeNode nodeToSelect = null;
			treeView1.Nodes.Clear();

			var rootNode = treeView1.Nodes.Add("月报表");

			var reportList = reporter.GetMonthltReportList();
			foreach (var year in reportList.Keys)
			{
				var yearNode = rootNode.Nodes.Add(year.ToString(), year.ToString());
				foreach (var month in reportList[year].Keys)
				{
					if ((year == yearForLastMonth) && (month == monthForLastMonth))
					{
						lastMonthReportCreated = true;
					}

					var monthNode = yearNode.Nodes.Add(reportList[year][month], month.ToString());
					monthNode.Tag = monthNode.Name;     // Report Id
														
					// Select the last node or if the node specified by reportId
					if (string.IsNullOrEmpty(reportId))
					{
						nodeToSelect = monthNode;
					}
					else
					{
						if (string.Equals(reportId, monthNode.Name, StringComparison.InvariantCultureIgnoreCase))
						{
							nodeToSelect = monthNode;
						}
					}
				}
			}

			buttonCreateLastMonth.Enabled = !lastMonthReportCreated;
			
			return nodeToSelect;

		}

		private void RetrieveMonthReport()
		{
			var currentNode = treeView1.SelectedNode;
			if (currentNode == null)
				return;

			var reportId = currentNode.Tag as string;
			if (string.IsNullOrWhiteSpace(reportId))
				return;

			RetrieveMstr(reportId);

			RetrieveStatDet(reportId);

			RetrieveWorkerDet(reportId);
		}


		private void RetrieveMstr(string reportId)
		{
			dataGridViewMstr.Rows.Clear();
			buttonCreateFile.Enabled = false;
			buttonOpenFile.Enabled = false;

			var mstr = reporter.GetMonthReportMstr(reportId);
			if ((mstr == null) || (mstr.Tables.Count < 1) || (mstr.Tables[0].Rows.Count < 1))
			{
				logger.Debug("GetMonthReportMstr did not return data. ReportId={0},table count={1}.", reportId, (mstr == null) ? "null" : mstr.Tables.Count.ToString());
				return;
			}

			var srcRow = mstr.Tables[0].Rows[0];
			dataGridViewMstr.Rows.Add("BeginTime", srcRow["BeginTime"].ToString());
			dataGridViewMstr.Rows.Add("EndTime", srcRow["EndTime"].ToString());
			dataGridViewMstr.Rows.Add("CreateTime", srcRow["CreateTime"].ToString());

			var isFIleCreated = (bool)srcRow["IsFileCreated"];
			dataGridViewMstr.Rows.Add("IsFileCreated", isFIleCreated ? "是" : "否");
			dataGridViewMstr.Rows.Add("FilePath", (srcRow["FilePath"].GetType() != typeof(DBNull)) ? srcRow["FilePath"].ToString() : "");
			dataGridViewMstr.Rows.Add("FileCreateTime", (srcRow["FileCreateTime"].GetType() != typeof(DBNull)) ? srcRow["FileCreateTime"].ToString() : "");

			buttonCreateFile.Enabled = !isFIleCreated;
			buttonOpenFile.Enabled = isFIleCreated;
		}

		private void RetrieveStatDet(string reportId)
		{
			dataGridViewReportDet.Rows.Clear();

			var ds = reporter.GetMonthReportStatDet(reportId);
			if (ds == null || ds.Tables.Count < 2 || ds.Tables["Shift"].Rows.Count < 1)
			{
				logger.Debug("GetMonthReprotStatDet did not return data, ReportId={0}, table count={1}", reportId, ds?.Tables.Count);
				return;
			}

			var shiftTable = ds.Tables["Shift"];
			var detailTable = ds.Tables["StatDet"];

			foreach (DataRow row in shiftTable.Rows)
			{
				var rowIndex = dataGridViewReportDet.Rows.Add("班次开始时间", row["BeginTime"].ToString());
				dataGridViewReportDet.Rows[rowIndex].DefaultCellStyle.BackColor = Color.Gray;

				detailTable.DefaultView.RowFilter = $"ShiftId='{row["ShiftId"].ToString()}'";
				foreach (DataRow detRow in detailTable.Rows)
				{
					var index = dataGridViewReportDet.Rows.Add(detRow["DisplayName"].ToString(), detRow["Subtotal"].ToString());
					dataGridViewReportDet.Rows[index].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
				}
			}
		}

		private void RetrieveWorkerDet(string reportId)
		{
			dataGridViewWorkerReport.Rows.Clear();

			var ds = reporter.GetMonthReportWorkerDet(reportId);
			if (ds == null || ds.Tables.Count < 2 || ds.Tables["Worker"].Rows.Count < 1)
			{
				logger.Debug("GetMonthReportWorkerDet did not return data, ReportId={0}, table count={1}", reportId, ds?.Tables.Count);
				return;
			}

			var workerTable = ds.Tables["Worker"];
			var detailTable = ds.Tables["StatDet"];

			foreach (DataRow row in workerTable.Rows)
			{
				var workerId = row["WorkerId"].ToString();
				var rowIndex = dataGridViewWorkerReport.Rows.Add($@"{workerId}/{row["WorkerName"].ToString()}", "");
				dataGridViewWorkerReport.Rows[rowIndex].DefaultCellStyle.BackColor = Color.Gray;

				detailTable.DefaultView.RowFilter = $"WorkerId='{workerId}'";
				foreach (DataRow detRow in detailTable.Rows)
				{
					var index = dataGridViewWorkerReport.Rows.Add(detRow["DisplayName"].ToString(), detRow["Subtotal"].ToString());
					dataGridViewWorkerReport.Rows[index].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
				}
			}
		}

		#region Override methods

		protected override void Cleanup()
		{
			base.Cleanup();

			try
			{
				connection.Close();
				connection.Dispose();
			}
			catch (Exception ex) { logger.Error(ex, "Error occurred in Cleanup method"); }
		}

		public override bool Save()
		{
			if (!base.Save())
				return false;

			try
			{
				// todo
				HasDirtyData = false;
				return true;
			}
			catch (Exception ex)
			{
				MessageBox.Show($"无法保存数据。{ex}");
				return false;
			}
		}

		protected override void UpdateMenuState()
		{
			base.UpdateMenuState();

			//saveToolStripMenuItem.Enabled = HasDirtyData;
			//saveToolStripButton.Enabled = HasDirtyData;

			//var hasData = dataGridViewUser.Rows.Count > 0;
			//deleteToolStripMenuItem.Enabled = hasData;
			//deleteToolStripButton.Enabled = hasData;
			//changePwdToolStripMenuItem.Enabled = hasData;
			//changePwdToolStripButton.Enabled = hasData;
		}
		#endregion

		private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
		{
			var original = Cursor.Current;
			Cursor = Cursors.WaitCursor;

			RetrieveMonthReport();

			Cursor = original;
		}

		private void treeView1_BeforeSelect(object sender, TreeViewCancelEventArgs e)
		{
			if (e.Node == null)
				return;

			var reportId = e.Node.Tag as string;
			if (string.IsNullOrWhiteSpace(reportId))
			{
				e.Cancel = true;
			}
		}

		private void buttonCreateFile_Click(object sender, EventArgs e)
		{
			var currentNode = treeView1.SelectedNode;
			if (currentNode == null)
				return;

			var reportId = currentNode.Tag as string;
			if (string.IsNullOrWhiteSpace(reportId))
				return;

			buttonCreateFile.Enabled = false;

			var original = Cursor.Current;
			Cursor = Cursors.WaitCursor;
			try
			{
				reporter.CreateReportFile(reportId);
				RetrieveMstr(reportId);
			}
			catch (Exception ex)
			{
				buttonCreateFile.Enabled = true;
				logger.Error(ex, "Error occurred while creating report file.");
				MessageBox.Show($"创建报表文件时出错。{ex}");
			}

			Cursor = original;
		}

		private void buttonOpenFile_Click(object sender, EventArgs e)
		{
			foreach (DataGridViewRow row in dataGridViewMstr.Rows)
			{
				if (string.Equals("FilePath", row.Cells[0].Value.ToString(), StringComparison.OrdinalIgnoreCase))
				{
					var val = row.Cells[1].Value;
					if (val is DBNull)
					{
						MessageBox.Show("没有发现月报文件");
						return;
					}

					var path = val.ToString();
					if (!File.Exists(path))
					{
						MessageBox.Show($"月报文件[{path}]并不存在。");
						return;
					}

					Process.Start(path);
					return;
				}
			}
		}

		private void buttonCreateLastMonth_Click(object sender, EventArgs e)
		{
			var original = Cursor.Current;
			Cursor = Cursors.WaitCursor;

			buttonCreateLastMonth.Enabled = false;
			CreateLastMonthReport();

			Cursor = original;
		}

		private void CreateLastMonthReport()
		{
			// Create the report of last month if has not creae yet
			var lastMonth = DateTime.Now.AddMonths(-1);
			var year = lastMonth.Year;
			var month = lastMonth.Month;
			string reportId = null;

			try
			{
				var connection = DbFactory.GetConnection();
				var reporter = new Reporter(connection);
				reportId = reporter.CreateMonthlyReport(year, month);

			}
			catch (ReportFileAlreadyCreatedException faa)
			{
				logger.Warn(faa.Message);
				MessageBox.Show($"[{year}-{month}]月报Excel文件已经创建创建了，不能重复创建。", "警告");
			}
			catch (FileNotFoundException fnf)
			{
				logger.Error(fnf, "Error occurred while creating monthly report.");
				MessageBox.Show($"月报模板没有找到。\r\n{fnf.Message}", "错误");
			}
			catch (Exception ex)
			{
				logger.Error(ex, $"Error occurred while creating monthly report for {year}-{month}.");
				MessageBox.Show($"[{year}-{month}]月报创建出错，请重试。\r\n{ex.ToString()}", "错误");
			}

			RefreshData(reportId);
		}

		private void refreshToolStripMenuItem_Click(object sender, EventArgs e)
		{
			RefreshData();
		}
	}
}
