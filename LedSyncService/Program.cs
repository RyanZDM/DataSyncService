using Topshelf;

namespace LedSyncService
{
	class Program
	{
		static void Main(string[] args)
		{
			var dal = new Dal(DbFactory.GetConnection());
			var statData = dal.GetStatData();

			const int Len = 18;
			var info = string.Format("{0}{1,10} KW\r\n{2}{3,10} M³\r\n{4}{5,10} KW\r\n{6}{7,10} M³"
									,"总发电量:".PadRight(Len), 1.0
									, "总沼气消耗量:".PadRight(Len), 2.0
									, "当前工班发电量:".PadRight(Len), 3.0
									, "当前工班沼气消耗量:".PadRight(Len), 4.0);

			var info1 = string.Format("{0}\t\t{1} KW\r\n{2}\t\t{3} M³\r\n{4}\t\t{5} KW\r\n{6}\t{7} M³"
									, "总发电量:", 1.0
									, "总沼气消耗量:", 2.0
									, "当前工班发电量:", 3.0
									, "当前工班沼气消耗量:", 4.0);

			return;
			var host = HostFactory.New(config =>
			{
				config.UseNLog();
				config.SetServiceName("LEDSyncService");
				config.SetDisplayName("LED Info Sync Service");
				config.SetDescription("Synchronize the infomatio to LED.");
				config.StartAutomatically();
				config.RunAsLocalSystem();

				config.EnableServiceRecovery(sr =>
				{
					sr.RestartService(1);
					sr.RestartService(1);
					sr.RestartService(1);
				});

				config.Service<LedSyncSvc>(x =>
				{
					x.ConstructUsing(setting => new LedSyncSvc());
					x.WhenStarted(s => s.Start());
					x.WhenStopped(s => s.Stop());
					x.WhenShutdown(s => s.Shutdown());
				});
			});

			host.Run();
		}
	}
}
