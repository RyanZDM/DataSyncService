using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using NLog;
namespace EBoard
{
    public partial class EBoard : Form
	{
		private readonly Logger logger = NLog.LogManager.GetCurrentClassLogger();

        private System.Threading.Timer refreshDataTimer;

		public EBoard()
		{
			InitializeComponent();
        }

        private void EBoard_Load(object sender, System.EventArgs e)
        {
			logger.Info("Electronic Board System started.");

	        var now = DateTime.Now;
			labelCurrDate.Text = DateTime.Now.ToString("yyyy年MM月dd日 dddd", new System.Globalization.CultureInfo("zh-cn"));
            refreshDataTimer = new System.Threading.Timer(RefreshDataTimerCallback, null, 0, Timeout.Infinite);            
        }

        private void RefreshDataTimerCallback(object state)
        {
            refreshDataTimer.Change(Timeout.Infinite, Timeout.Infinite);
            try
            {
                var conn = new SqlConnection(@"Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=OPC;Data Source=.\carestream");
                conn.Open();
                var productionData = GetData(conn);
                RefreshData(productionData);
            }
            finally
            {
                refreshDataTimer.Change(2000, Timeout.Infinite);
            }            
        }

        private void RefreshData(ProductionData data)
        {
            if (this.InvokeRequired)
            {
                BeginInvoke((MethodInvoker)delegate () { RefreshData(data); });
                return;
            }

            if (data.Workers.Count > 0)
            {
                var sb = new StringBuilder();
                data.Workers.ForEach(w => sb.Append(string.Format("姓名 {0}  工号 {1}    ", w.Id, w.Name)));
                labelWorkers.Text = sb.ToString();
            }
            else
            {
                labelWorkers.Text = "";
            }

	        labelTotalRuntime1.Text = data.TotalRuntime1.ToString();
			labelTotalRuntime2.Text = data.TotalRuntime2.ToString();
            labelBiogas1.Text = data.Biogas1.ToString();
            labelBiogas2.Text = data.Biogas2.ToString();
	        labelBiogasTotal.Text = (data.Biogas1 + data.Biogas2).ToString();
	        labelEnergyProduction1.Text = data.EnergyProduction1.ToString();
			labelEnergyProduction2.Text = data.EnergyProduction2.ToString();
	        labelEnergyProductionTotal.Text = (data.EnergyProduction1 + data.EnergyProduction2).ToString();
	        labelRuntime1.Text = data.Runtime1.ToString();
	        labelRuntime2.Text = data.Runtime2.ToString();
	        labelRuntimeTotal.Text = (data.Runtime1 + data.Runtime2).ToString();
        }

        private const string Biogas1ColName = "Biogas1";
        private const string Biogas2ColName = "Biogas2";
        private const string EnergyProduction1ColName = "EnergyProduction1";
        private const string EnergyProduction2ColName = "EnergyProduction2";
        private const string Runtime1ColName = "Runtime1";
        private const string Runtime2ColName = "Runtime2";
		private const string TotalRuntime1ColName = "TotalRuntime1";
		private const string TotalRuntime2ColName = "TotalRuntime2";

        private ProductionData GetData(SqlConnection connection)
        {
            var ds = new DataSet();  
            var sql = @"Select a.ItemId, a.Val, a.LastUpdate, a.Quality, b.Name From ItemLatestStatus a,MonitorItem b Where a.ItemID=b.ItemId";
            var adapter = new SqlDataAdapter(sql, connection);
            adapter.Fill(ds, "ItemLatestStatus");

            sql = @"Select Id,ShiftId,BeginTime,EndTime,IsCurrent,Status From ProductionStatMstr Where IsCurrent = 1";
            new SqlDataAdapter(sql, connection).Fill(ds, "ProductionStatMstr");

            sql = @"SELECT Name,SubTotal FROM ProductionStatDet, ProductionStatMstr, MonitorItem Where ProductionId=Id And ItemId = Item And IsCurrent=1";
            new SqlDataAdapter(sql, connection).Fill(ds, "ProductionStatDet");

            sql = @"SELECT WorkerId, WorkerName FROM WorkersInProduction, ProductionStatMstr Where ProductionId=Id And IsCurrent=1";
            new SqlDataAdapter(sql, connection).Fill(ds, "WorkersInProduction");

            var data = new ProductionData();

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
                    latestDict[row["Name"].ToString().ToLower()] = (double)row["Val"];
                }
                catch(Exception ex)
                {
					logger.Error("Cannot convert the value [{0}] of [{1}] to double type from table ItemLatestStatus. {2}", row["Val"], row["Name"], ex);
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
                    subtotalDict[row["Name"].ToString().ToLower()] = (double)row["SubTotal"];
                }
				catch (Exception ex)
				{
					logger.Error("Cannot convert the value [{0}] of [{1}] to double type from table ProductionStatDet. {2}", row["SubTotal"], row["Name"], ex);
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
