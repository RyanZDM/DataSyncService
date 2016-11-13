using System;
using System.Collections.Generic;

namespace EBoard.Common
{
	public class ProductionData
	{
		public string CurrentShift { get; set; }

		public List<Worker> Workers { get; set; }

		public DateTime LastUpdate { get; set; }

		/// <summary>
		/// 填埋场沼气流量
		/// </summary>
		public double Biogas1 { get; set; }

		/// <summary>
		/// 垃圾焚烧沼气流量
		/// </summary>
		public double Biogas2 { get; set; }

		/// <summary>
		/// #1机组发电量
		/// </summary>
		public double EnergyProduction1 { get; set; }

		/// <summary>
		/// #2机组发电量
		/// </summary>
		public double EnergyProduction2 { get; set; }

		/// <summary>
		/// #1机组运行时间
		/// </summary>
		public double Runtime1 { get; set; }

		/// <summary>
		/// #2机组运行时间
		/// </summary>
		public double Runtime2 { get; set; }

		/// <summary>
		/// #1机组累计运行时间
		/// </summary>
		public double TotalRuntime1 { get; set; }

		/// <summary>
		/// #2机组累计运行时间
		/// </summary>
		public double TotalRuntime2 { get; set; }

		//public ProductionData(IDbConnection connection)
		//{ }
	}
}
