using System;
using System.Windows.Forms;
using System.ComponentModel;
using System.Drawing.Drawing2D;
using System.Drawing;

namespace EBoard.Common
{
	public class RoundPanel : Panel
	{
		public RoundPanel()
		{
			//InitializeComponent();

			Padding = new Padding(0, 0, 0, 0);
			Margin = new Padding(0, 0, 0, 0);
			BackgroundImageLayout = ImageLayout.Stretch;
		}

		private int radius;

		/// <summary>Angle of round</summary>
		[Browsable(true)]
		[Description("Angle of round")]
		public int RoundRadius
		{
			get { return radius; }
			set
			{
				if (radius == value)
					return;

				radius = (value < 0) ? 0 : value;

				Refresh();
			}
		}

		private RoundCornerEnum roundCorners = RoundCornerEnum.All;
		[Browsable(true)]
		[Description("Which corner to round")]
		public RoundCornerEnum RoundCorners
		{
			get { return roundCorners; }
			set
			{
				if (roundCorners == value)
					return;

				roundCorners = value;
				Refresh();
			}
		}

		public void Round(Region region)
		{
			// -----------------------------------------------------------------------------------------------
			// 已经是.net提供给我们的最容易的改窗体的属性了(以前要自己调API)
			var oPath = new GraphicsPath();
			var x = 0;
			var y = 0;
			var thisWidth = Width;
			var thisHeight = Height;
			var angle = radius;
			if (angle > 0)
			{
				var g = CreateGraphics();
				if ((roundCorners & RoundCornerEnum.TopLeft) == RoundCornerEnum.TopLeft)
				{
					oPath.AddArc(x, y, angle, angle, 180, 90);                                  // Top left
				}
				else
				{
					oPath.AddLine(x + angle, y, thisWidth - angle, y);
				}

				if ((roundCorners & RoundCornerEnum.TopRight) == RoundCornerEnum.TopRight)
				{
					oPath.AddArc(thisWidth - angle, y, angle, angle, 270, 90);                 // Top right
				}
				else
				{
					oPath.AddLine(thisWidth, y + angle, thisWidth, thisHeight - angle);
				}

				if ((roundCorners & RoundCornerEnum.BottomRight) == RoundCornerEnum.BottomRight)
				{
					oPath.AddArc(thisWidth - angle, thisHeight - angle, angle, angle, 0, 90);  // Bottom right
				}
				else
				{
					oPath.AddLine(thisWidth - angle, thisHeight, x + angle, thisHeight);
				}

				if ((roundCorners & RoundCornerEnum.BottomLeft) == RoundCornerEnum.BottomLeft)
				{
					oPath.AddArc(x, thisHeight - angle, angle, angle, 90, 90);                 // Bottom left
				}
				else
				{
					oPath.AddLine(x, y + angle, x, thisHeight - angle);
				}

				oPath.CloseAllFigures();
				Region = new Region(oPath);
			}
			// -----------------------------------------------------------------------------------------------
			else
			{
				oPath.AddLine(x + angle, y, thisWidth - angle, y);                         // Top
				oPath.AddLine(thisWidth, y + angle, thisWidth, thisHeight - angle);        // Right
				oPath.AddLine(thisWidth - angle, thisHeight, x + angle, thisHeight);       // Bottom
				oPath.AddLine(x, y + angle, x, thisHeight - angle);                        // Left
				oPath.CloseAllFigures();
				Region = new Region(oPath);
			}
		}
		// ===============================================================================================


		public RoundPanel(IContainer container)
		{
			container.Add(this);

			//InitializeComponent();
		}

		protected override void OnPaint(PaintEventArgs pe)
		{
			base.OnPaint(pe);
			Round(Region);
		}

		protected override void OnResize(EventArgs eventargs)
		{
			base.OnResize(eventargs);
			Refresh();
		}
	}

	public enum RoundCornerEnum
	{
		TopLeft = 0x1,
		TopRight = 0x2,
		BottomRight = 0x4,
		BottomLeft = 0x8,
		All = TopLeft | TopRight | BottomRight | BottomLeft
	}
}