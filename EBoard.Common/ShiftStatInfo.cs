using System;
using System.Collections.Generic;

namespace EBoard.Common
{
	public class ShiftStatInfo
	{
		/// <summary>填埋场沼气流量</summary>
		public const string Biogas1ColName = "Biogas1";
		/// <summary>垃圾焚烧沼气流量</summary>
		public const string Biogas2ColName = "Biogas2";
		/// <summary>#1机组发电量</summary>
		public const string EnergyProduction1ColName = "EnergyProduction1";
		/// <summary>#2机组发电量</summary>
		public const string EnergyProduction2ColName = "EnergyProduction2";
		/// <summary>#1机组运行时间</summary>
		public const string Runtime1ColName = "Runtime1";
		/// <summary>#2机组运行时间</summary>
		public const string Runtime2ColName = "Runtime2";
		/// <summary>#1机组累计运行时间</summary>
		public const string TotalRuntime1ColName = "TotalRuntime1";
		/// <summary>#机组累计运行时间</summary>
		public const string TotalRuntime2ColName = "TotalRuntime2";

		public string ShiftId { get; set; }

		public string LastLoginId { get; set; }

		public string LastLoginName { get; set; }

		public DateTime BeginTime { get; set; }

		public DateTime ActualBeginTime { get; set; }

		public DateTime EndTime { get; set; }

		public DateTime LastUpdate { get; set; }

		public IDictionary<string, double> MonitorItems { get; set; }

		public IDictionary<string, double> StatInfo { get; set; }
	}
}
