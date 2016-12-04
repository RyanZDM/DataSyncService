using System;
using System.Collections.Generic;

namespace EBoard.Common
{
	public class ShiftStatInfo
	{
		/// <summary>填埋场沼气瞬时流量</summary>
		public static readonly string Biogas2TorchColName = "Biogas2Torch".ToLower();
		public static readonly string Biogas2TorchSubtotalColName = "Biogas2TorchSubtotal".ToLower();
		/// <summary>垃圾焚烧沼气瞬时流量</summary>
		public static readonly string Biogas2GenColName = "Biogas2Gen".ToLower();
		public static readonly string Biogas2GenSubtotalColName = "Biogas2GenSubtotal".ToLower();
		/// <summary>#1机组累计发电量</summary>
		public static readonly string EnergyProduction1ColName = "EnergyProduction1".ToLower();
		/// <summary>#2机组累计发电量</summary>
		public static readonly string EnergyProduction2ColName = "EnergyProduction2".ToLower();
		/// <summary>#1机组瞬时功率</summary>
		public static readonly string GeneratorPower1ColName = "GeneratorPower1".ToLower();
		/// <summary>#2机组瞬时功率</summary>
		public static readonly string GeneratorPower2ColName = "GeneratorPower2".ToLower();
		/// <summary>#1机组累计运行时间-当前工班</summary>
		public static readonly string SubtotalRuntime1ColName = "SubtotalRuntime1".ToLower();
		/// <summary>#机组累计运行时间-当前工班</summary>
		public static readonly string SubtotalRuntime2ColName = "SubtotalRuntime2".ToLower();
		/// <summary>#1机组累计运行时间</summary>
		public static readonly string TotalRuntime1ColName = "TotalRuntime1".ToLower();
		/// <summary>#2机组累计运行时间</summary>
		public static readonly string TotalRuntime2ColName = "TotalRuntime2".ToLower();
		/// <summary>#1机组运行状态</summary>
		public static readonly string Generator1RunningColName = "Generator1Running".ToLower();
		/// <summary>#2机组运行状态</summary>
		public static readonly string Generator2RunningColName = "Generator2Running".ToLower();

		public string ShiftId { get; set; }

		public DateTime BeginTime { get; set; }

		public DateTime ActualBeginTime { get; set; }

		public DateTime LastUpdateTime { get; set; }

		public DateTime EndTime { get; set; }

		public IDictionary<string, double> MonitorItems { get; set; }

		public IDictionary<string, double> StatInfo { get; set; }
	}
}
