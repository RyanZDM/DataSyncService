using System;
using EBoard.Common;
using System.Data.SqlClient;
using System.Data;
using NLog;

namespace GenerateLastMonthReport
{
	internal class Program
	{
		private static readonly Logger Logger = LogManager.GetCurrentClassLogger();

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
			if ((args.Length == 1) && args[0].Equals("/help", StringComparison.OrdinalIgnoreCase))
			{
				ShowHelp();
				return 0;
			}

			int? year = null;
			int? month = null;

			foreach (var arg in args)
			{
				switch (arg.Trim().ToLower())
				{
					case "/year":
						year = int.Parse(arg);
						break;
					case "/month":
						month = int.Parse(arg);
						break;
					case "/notcreateexcel":
						notCreateExcel = true;
						break;
					case "/createexcelonly":
						createExcelOnly = true;
						break;
				}
			}

			// The year and month must be specified both
			if (!(year.HasValue & month.HasValue))
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
			catch (Exception ex)
			{
				Logger.Error(ex, "ailed to create Excel for monthly report '{0}'", yearMonth);
				return -1;
			}
		}

		private static void ShowHelp()
		{
			Console.Out.WriteLine("Usage: /year /month /NotCreateExcel /CreateExcelOnly\r\nyear/month: Indicate the year and month of the report to create.\r\n/NotCreateExcel: Not create Excel while creating monthly report.\r\nCreateExcelOnly: Create Excel for a existed monthly report.");
		}

		private static int CreateMonthlyReport(int year, int month)
		{
			var yearMonth = $"{year}{month:d2}";
			try
			{
				Logger.Info("Will create the monthly report for {0}.", yearMonth);

				var connection = DbFactory.GetConnection();
				var cmd = new SqlCommand("sp_CreateMonthlyReport", connection)
				{
					CommandType = CommandType.StoredProcedure
				};

				cmd.Parameters.AddWithValue("@YearMonth", yearMonth);

				var reportIdParam = cmd.Parameters.Add(new SqlParameter
				{
					ParameterName = "@ReportId",
					SqlDbType = SqlDbType.UniqueIdentifier,
					Direction = ParameterDirection.Output
				});

				var returnParam = cmd.Parameters.Add(new SqlParameter
				{
					ParameterName = "@return",
					SqlDbType = SqlDbType.Int,
					Direction = ParameterDirection.ReturnValue
				});

				cmd.ExecuteNonQuery();

				var reportId = reportIdParam.Value.ToString();
				var result = (int)returnParam.Value;

				Logger.Info("Create the monthly report for {0}, result={1}, ReportId='{2}'", yearMonth, result, reportId);

				var sql = $@"Select IsFIleCreated From MonthReportMstr Where ReportId=Convert('{reportId} As uniqueidentifier)";
				var isFileCreated = (bool)(new SqlCommand(sql, connection).ExecuteScalar());
				Logger.Info("Excel file of monthly report for {0} {1}.", yearMonth, isFileCreated ? "is created" : "is not created yet");

				if (isFileCreated)
					return 1;
								
				if (notCreateExcel)
					return 1;

				// Create Excel file
				var reporter = new Reporter(connection);
				reporter.CreateReportFile(reportId);
				
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
