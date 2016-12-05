using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EBoard.Common
{
	public partial class UserPropertyDlg : Form
	{
		public UserPropertyDlg()
		{
			InitializeComponent();
		}

		private string loginId;
		public string LoginId
		{
			get { return loginId; }
			set
			{
				if (loginId == value)
					return;

				Text = string.Format("{0} 属性", loginId);
			}
		}

		public bool DataChanged { get; private set; }
	}
}
