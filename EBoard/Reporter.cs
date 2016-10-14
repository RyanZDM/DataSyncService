using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace EBoard
{
	public class Reporter
	{
		public void CreateReport(int month)
		{
			throw new NotImplementedException();
		}

		public void GetCurrentMonthData()
		{
			var now = DateTime.Now;
			var month = now.Month;
			var daysInMonth = DateTime.DaysInMonth(now.Year, month);
			var beginTime = new DateTime(now.Year, month, 1, 0, 0, 0);
			var endTime = beginTime.AddMonths(1);
		}
	}
}
