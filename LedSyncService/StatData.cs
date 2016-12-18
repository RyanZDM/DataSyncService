namespace LedSyncService
{
	public class StatData
	{
		/// <summary>
		/// 总发电量
		/// </summary>
		public float TotalEnergyGenerated { get; set; }

		/// <summary>
		/// 总沼气消耗量
		/// </summary>
		public float TotalBiogasUsed { get; set; }

		/// <summary>
		/// 当前工班发电量
		/// </summary>
		public float CurrentEnergyGenerated { get; set; }

		/// <summary>
		/// 当前工班沼气消耗量
		/// </summary>
		public float CurrentBiogasUsed { get; set; }
	}
}
