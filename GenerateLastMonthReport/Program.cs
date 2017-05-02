using System;
using System.Data.SqlClient;
using System.Linq;
using EBoard.Common;
using NLog;

namespace GenerateMonthReport
{
	internal class Program
	{
		private static readonly Logger Logger = LogManager.GetCurrentClassLogger();

		private static bool autoCreateReport;

		private static bool notCreateExcel;

		private static bool createExcelOnly;

		/// <summary>
		/// Create report for a special month, and/or create Excel for it
		/// </summary>
		/// <param name="args"></param>
		/// <returns></returns>
		/// <remarks>
		/// Arguments:
		///		- /year /month /NotCreateExcel /CreateExcelOnly
		/// </remarks>
		public static int Main(string[] args)
		{
			if ((args.Length == 1) && 
				(args[0].Equals("/help", StringComparison.OrdinalIgnoreCase) || args[0].Equals("/?")))
			{
				ShowHelp();
				return 0;
			}

			int? year = null;
			int? month = null;

			for (var i = 0; i < args.Length; i++)
			{
				switch (args[i].Trim().ToLower())
				{
					case "/auto":
						autoCreateReport = true;
						break;
					case "/year":
						if (i + 1 < args.Length)
						{
							year = int.Parse(args[i + 1]);
						}
						break;
					case "/month":
						if (i + 1 < args.Length)
						{
							month = int.Parse(args[i + 1]);
						}
						break;
					case "/notcreateexcel":
						notCreateExcel = true;
						break;
					case "/createexcelonly":
						createExcelOnly = true;
						break;
				}
			}

			if (autoCreateReport)
			{
				// temp
				Logger.Info("Auto create mode.");

				// Ignore all other arguments
				// Under auto mode, will check the settings in db when to create monthly report
				// Do nothing if the predefined time has not arrived yet, will create monthly
				// report for last month if the current time passed predefined time 
				var dal = new Dal(DbFactory.GetConnection());
				var param = dal.GetGeneralParameters().FirstOrDefault(s => string.Equals("System", s.Category, StringComparison.InvariantCultureIgnoreCase) && string.Equals("CreateMonthReportTime", s.Name, StringComparison.InvariantCultureIgnoreCase));
				if (param == null)
				{
					Logger.Error("Did not find the settings fro creating monthly report.");
					return -2;
				}

				var now = DateTime.Now;
				var day = int.Parse(param.Value);
				var targetDay = new DateTime(now.Year, now.Month, day);
				if (now < targetDay)
				{
					Logger.Trace($"Skip to create monthly report since the predefined time has not arrived yet. [{targetDay}]");
					return 0;
				}

				// Create the report of last month if has not creae yet
				var lastMonth = DateTime.Now.AddMonths(-1);
				year = lastMonth.Year;
				month = lastMonth.Month;
				return CreateMonthlyReport(year.Value, month.Value);
			}
			else
			{
				// The year and month must be specified both
				if (year.HasValue ^ month.HasValue)
				{
					Logger.Error("Must specify the year and month both, or do not specif them at all.");
					return -1;
				}

				if (!year.HasValue)
				{
					// Create the report of last month if no arguments specified
					var lastMonth = DateTime.Now.AddMonths(-1);
					year = lastMonth.Year;
					month = lastMonth.Month;
				}

				if (createExcelOnly)
				{
					return CreateExcel(year.Value, month.Value);
				}

				// Create report
				return CreateMonthlyReport(year.Value, month.Value);
			}
		}

		private static int CreateExcel(int year, int month)
		{
			var yearMonth = $"{year}{month:d2}";
			try
			{
				var connection = DbFactory.GetConnection();
				var reporter = new Reporter(connection);
				var excelFilePath = reporter.CreateReportFile(year, month);

				var sql =
					$@"Update MonthReportMstr Set IsFileCreated=1,FilePath='{excelFilePath}',FileCreateTime=GetDate() Where YearMonth='{yearMonth}'";
				if (new SqlCommand(sql, connection).ExecuteNonQuery() < 1)
				{
					Logger.Error("Failed to create Excel for monthly report '{0}'", yearMonth);
					return 0;
				}

				return 1;
			}
			catch (ReportFileAlreadyCreatedException)
			{
				Logger.Error("The monthly report for {0} has been created already, directly return.", yearMonth);
				return -2;
			}
			catch (Exception ex)
			{
				Logger.Error(ex, "Failed to create Excel for monthly report '{0}'", yearMonth);
				return -1;
			}
		}

		private static void ShowHelp()
		{
			Console.Out.WriteLine("Usage: /auto /year <year> /month <month> /NotCreateExcel /CreateExcelOnly\r\n\t- auto: Automatically create monthly report.\r\n\t- year/month: Indicate the year and month of the report to create.\r\n\t- /NotCreateExcel: Not create Excel while creating monthly report.\r\n\t- /CreateExcelOnly: Create Excel for a existed monthly report.");
		}

		private static int CreateMonthlyReport(int year, int month)
		{
			var yearMonth = $"{year}{month:d2}";
			try
			{
				Logger.Info("Will create the monthly report for {0}.", yearMonth);

				var connection = DbFactory.GetConnection();
				var reporter = new Reporter(connection);
				reporter.CreateMonthlyReport(year, month, !notCreateExcel);
				
				return 1;
			}
			catch (Exception ex)
			{
				Logger.Error(ex, "Error occurred while creating monthly report for {0}", yearMonth);
				return -1;
			}
		}
	}
}
