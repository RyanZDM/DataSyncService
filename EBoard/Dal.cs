using NLog;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Linq;

namespace EBoard
{
	public class Dal
	{
		private readonly Logger logger = NLog.LogManager.GetCurrentClassLogger();

		private const string Biogas1ColName = "Biogas1";
		private const string Biogas2ColName = "Biogas2";
		private const string EnergyProduction1ColName = "EnergyProduction1";
		private const string EnergyProduction2ColName = "EnergyProduction2";
		private const string Runtime1ColName = "Runtime1";
		private const string Runtime2ColName = "Runtime2";
		private const string TotalRuntime1ColName = "TotalRuntime1";
		private const string TotalRuntime2ColName = "TotalRuntime2";

		private SqlConnection connection;

		public Dal(SqlConnection conn)
		{
			connection = conn;
		}

		public ProductionData GetProductionData(DateTime? lastUpdate = null)
		{
			var ds = new DataSet();
			var sql = @"Select a.ItemId, a.Val, a.LastUpdate, a.Quality, b.Address From ItemLatestStatus a,MonitorItem b Where a.ItemID=b.ItemId;Select Max(LastUpdate) As LastUpdate From ItemLatestStatus;";
			var adapter = new SqlDataAdapter(sql, connection);
			adapter.Fill(ds, "ItemLatestStatus");

			// Check if the data changed
			var lastUpdateTable = ds.Tables["ItemLatestStatus1"];

			// Do query need if no items in table
			if (lastUpdateTable.Rows[0][0] == DBNull.Value)
				return null;

			// No data change after 'lastUpdate'
			var newUpdate = (DateTime)lastUpdateTable.Rows[0][0];
			if (lastUpdate.HasValue && lastUpdate >= newUpdate)
				return null;

			var data = new ProductionData { LastUpdate = newUpdate };

			sql = @"Select Id,ShiftId,BeginTime,EndTime,IsCurrent,Status From ProductionStatMstr Where IsCurrent = 1";
			new SqlDataAdapter(sql, connection).Fill(ds, "ProductionStatMstr");

			sql = @"SELECT ItemId,SubTotal FROM ProductionStatDet, ProductionStatMstr, MonitorItem Where ProductionId=Id And ItemId = Item And IsCurrent=1";
			new SqlDataAdapter(sql, connection).Fill(ds, "ProductionStatDet");

			sql = @"SELECT WorkerId, WorkerName FROM WorkersInProduction, ProductionStatMstr Where ProductionId=Id And IsCurrent=1";
			new SqlDataAdapter(sql, connection).Fill(ds, "WorkersInProduction");
			
			// Current shift
			var mstrTable = ds.Tables["ProductionStatMstr"];
			if ((mstrTable == null) || (mstrTable.Rows.Count < 1))
			{
				data.CurrentShift = "";
				return data;
			}

			// Workers
			data.CurrentShift = mstrTable.Rows[0]["ShiftId"].ToString();
			data.Workers = new List<Worker>();
			foreach (DataRow row in ds.Tables["WorkersInProduction"].Rows)
			{
				data.Workers.Add(new Worker
				{
					Id = row["WorkerId"].ToString(),
					Name = row["WorkerName"].ToString()
				});
			}

			var latestTable = ds.Tables["ItemLatestStatus"];
			var latestDict = new Dictionary<string, double>();
			latestTable.AsEnumerable().ToList().ForEach(row =>
			{
				try
				{
					latestDict[row["ItemId"].ToString().ToLower()] = (double)row["Val"];
				}
				catch (Exception ex)
				{
					logger.Error("Cannot convert the value [{0}] of [{1}] to double type from table ItemLatestStatus. {2}", row["Val"], row["ItemId"], ex);
				}
			});

			//Energy Production
			if (latestDict.Keys.Contains(EnergyProduction1ColName, StringComparer.OrdinalIgnoreCase))
			{
				data.EnergyProduction1 = latestDict[EnergyProduction1ColName.ToLower()];
			}

			if (latestDict.Keys.Contains(EnergyProduction2ColName, StringComparer.OrdinalIgnoreCase))
			{
				data.EnergyProduction2 = latestDict[EnergyProduction2ColName.ToLower()];
			}

			// Total run time
			if (latestDict.Keys.Contains(TotalRuntime1ColName, StringComparer.OrdinalIgnoreCase))
			{
				data.TotalRuntime1 = latestDict[TotalRuntime1ColName.ToLower()];
			}

			if (latestDict.Keys.Contains(TotalRuntime2ColName, StringComparer.OrdinalIgnoreCase))
			{
				data.TotalRuntime2 = latestDict[TotalRuntime2ColName.ToLower()];
			}

			var detTable = ds.Tables["ProductionStatDet"];
			var subtotalDict = new Dictionary<string, double>();
			detTable.AsEnumerable().ToList().ForEach(row =>
			{
				try
				{
					subtotalDict[row["ItemId"].ToString().ToLower()] = (double)row["SubTotal"];
				}
				catch (Exception ex)
				{
					logger.Error("Cannot convert the value [{0}] of [{1}] to double type from table ProductionStatDet. {2}", row["SubTotal"], row["ItemId"], ex);
				}
			});

			// Biogas
			if (subtotalDict.Keys.Contains(Biogas1ColName, StringComparer.OrdinalIgnoreCase))
			{
				data.Biogas1 = subtotalDict[Biogas1ColName.ToLower()];
			}

			if (subtotalDict.Keys.Contains(Biogas2ColName, StringComparer.OrdinalIgnoreCase))
			{
				data.Biogas2 = subtotalDict[Biogas2ColName.ToLower()];
			}

			// Run time
			if (subtotalDict.Keys.Contains(Runtime1ColName, StringComparer.OrdinalIgnoreCase))
			{
				data.Runtime1 = subtotalDict[Runtime1ColName.ToLower()];
			}

			if (subtotalDict.Keys.Contains(Runtime2ColName, StringComparer.OrdinalIgnoreCase))
			{
				data.Runtime2 = subtotalDict[Runtime2ColName.ToLower()];
			}

			return data;
		}
	}
}
