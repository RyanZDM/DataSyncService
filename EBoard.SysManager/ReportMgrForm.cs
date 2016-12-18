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
		private readonly Logger logger = NLog.LogManager.GetCurrentClassLogger();

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

			var nodeToSelect = InitTree();
			if (nodeToSelect != null)
			{
				treeView1.SelectedNode = nodeToSelect;
			}

			//reporter.CreateReportFile(2016, 11);
		}
		
		private TreeNode InitTree()
		{
			TreeNode nodeToSelect = null;
			
			var rootNode = treeView1.Nodes.Add("月报表");

			var reportList = reporter.GetMonthltReportList();
			foreach (var year in reportList.Keys)
			{
				var yearNode = rootNode.Nodes.Add(year.ToString(), year.ToString());
				foreach (var month in reportList[year].Keys)
				{
					var monthNode = yearNode.Nodes.Add(reportList[year][month], month.ToString());
					monthNode.Tag = monthNode.Name;     // Report Id
					if (nodeToSelect == null)
						nodeToSelect = monthNode;       // Select the first node
				}
			}

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
				logger.Debug("GetMonthReprotStatDet did not return data, ReportId={0}, table count={1}", reportId, ds.Tables.Count);
				return;
			}

			var shiftTable = ds.Tables["Shift"];
			var detailTable = ds.Tables["StatDet"];

			foreach (DataRow row in shiftTable.Rows)
			{
				var rowIndex = dataGridViewReportDet.Rows.Add("班次开始时间", row["BeginTime"].ToString());
				dataGridViewReportDet.Rows[rowIndex].DefaultCellStyle.BackColor = Color.Gray;

				detailTable.DefaultView.RowFilter = string.Format("ShiftId='{0}'", row["ShiftId"].ToString());
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
				logger.Debug("GetMonthReportWorkerDet did not return data, ReportId={0}, table count={1}", reportId, ds.Tables.Count);
				return;
			}

			var workerTable = ds.Tables["Worker"];
			var detailTable = ds.Tables["StatDet"];

			foreach (DataRow row in workerTable.Rows)
			{
				var workerId = row["WorkerId"].ToString();
				var rowIndex = dataGridViewWorkerReport.Rows.Add(string.Format(@"{0}/{1}", workerId, row["WorkerName"].ToString()), "");
				dataGridViewWorkerReport.Rows[rowIndex].DefaultCellStyle.BackColor = Color.Gray;

				detailTable.DefaultView.RowFilter = string.Format("WorkerId='{0}'", workerId);
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
				MessageBox.Show(string.Format("无法保存数据。{0}", ex.ToString()));
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
			RetrieveMonthReport();
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

			try
			{
				reporter.CreateReportFile(reportId);
				RetrieveMstr(reportId);
			}
			catch (Exception ex)
			{
				buttonCreateFile.Enabled = true;
				logger.Error(ex, "Error occurred while creating report file.");
				MessageBox.Show(string.Format("创建报表文件时出错。{0}", ex.ToString()));
			}
			
		}

		private void buttonOpenFile_Click(object sender, EventArgs e)
		{
			foreach (DataGridViewRow row in dataGridViewMstr.Rows)
			{
				if (string.Equals("FilePath", row.Cells[0].Value.ToString(), StringComparison.OrdinalIgnoreCase))
				{
					var val = row.Cells[1].Value;
					if (val.GetType() == typeof(DBNull))
					{
						MessageBox.Show("没有发现月报文件");
						return;
					}

					var path = val.ToString();
					if (!File.Exists(path))
					{
						MessageBox.Show(string.Format("月报文件[{0}]并不存在。", path));
						return;
					}

					Process.Start(path);
					return;
				}
			}
		}
	}
}
