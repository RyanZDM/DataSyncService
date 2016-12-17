using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Topshelf;

namespace LedSyncService
{
	class Program
	{
		static void Main(string[] args)
		{
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
