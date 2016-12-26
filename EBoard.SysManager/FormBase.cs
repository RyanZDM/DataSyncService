using System.ComponentModel;
using System.Windows.Forms;

namespace EBoard.SysManager
{
	public partial class FormBase : Form
	{
		private bool hasDirtyData;

		protected bool HasDirtyData
		{
			get { return hasDirtyData; }
			set
			{
				//if (hasDirtyData == value)
				//	return;

				hasDirtyData = value;
				UpdateMenuState();
			}
		}

		public FormBase()
		{
			InitializeComponent();
		}

		protected virtual void UpdateMenuState() {	}

		public virtual bool CheckDirtyData()
		{
			if (!hasDirtyData)
				return true;

			switch (MessageBox.Show("数据已修改，需要保存吗？", "Save", MessageBoxButtons.YesNoCancel))
			{
				case DialogResult.Yes:
					return Save();
				case DialogResult.No:
					RollbackChanges();
					return true;
				case DialogResult.Cancel:
					return false;
				default:
					return true;
			}
		}

		public virtual void RollbackChanges() { }

		public virtual bool Save()
		{
			return true;
		}

		protected override void OnClosing(CancelEventArgs e)
		{
			base.OnClosing(e);

			if (!e.Cancel)
			{
				if (CheckDirtyData())
				{
					Cleanup();
				}
				else
				{
					e.Cancel = true;
				}
			}
		}

		protected virtual void Cleanup() { }
	}
}
