using System;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.ComponentModel;

namespace EBoard.Common
{
    public class RoundPanel : Panel
    {
        public RoundPanel()
        {
            //InitializeComponent();

            this.Padding = new System.Windows.Forms.Padding(0, 0, 0, 0);
            this.Margin = new System.Windows.Forms.Padding(0, 0, 0, 0);
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
        }


        // Բ��
        // ===============================================================================================
        private int _Radius;  // Բ�ǻ���

        /// <summary>Բ�ǻ���(0Ϊ��ҪԲ��)</summary>
        [Browsable(true)]
        [Description("Բ�ǻ���(0Ϊ��ҪԲ��)")]
        public int _setRoundRadius
        {
            get
            {
                return _Radius;
            }
            set
            {
                if (value < 0) { _Radius = 0; }
                else { _Radius = value; }
                base.Refresh();
            }
        }


        // Բ�Ǵ���
        public void Round(System.Drawing.Region region)
        {
            // -----------------------------------------------------------------------------------------------
            // �Ѿ���.net�ṩ�����ǵ������׵ĸĴ����������(��ǰҪ�Լ���API)
            System.Drawing.Drawing2D.GraphicsPath oPath = new System.Drawing.Drawing2D.GraphicsPath();
            int x = 0;
            int y = 0;
            int thisWidth = this.Width;
            int thisHeight = this.Height;
            int angle = _Radius;
            if (angle > 0)
            {
                System.Drawing.Graphics g = CreateGraphics();
                oPath.AddArc(x, y, angle, angle, 180, 90);                                 // ���Ͻ�
                oPath.AddArc(thisWidth - angle, y, angle, angle, 270, 90);                 // ���Ͻ�
                oPath.AddArc(thisWidth - angle, thisHeight - angle, angle, angle, 0, 90);  // ���½�
                oPath.AddArc(x, thisHeight - angle, angle, angle, 90, 90);                 // ���½�
                oPath.CloseAllFigures();
                Region = new System.Drawing.Region(oPath);
            }
            // -----------------------------------------------------------------------------------------------
            else
            {
                oPath.AddLine(x + angle, y, thisWidth - angle, y);                         // ����
                oPath.AddLine(thisWidth, y + angle, thisWidth, thisHeight - angle);        // �ұ�
                oPath.AddLine(thisWidth - angle, thisHeight, x + angle, thisHeight);       // �ױ�
                oPath.AddLine(x, y + angle, x, thisHeight - angle);                        // ���
                oPath.CloseAllFigures();
                Region = new System.Drawing.Region(oPath);
            }
        }
        // ===============================================================================================


        public RoundPanel(IContainer container)
        {
            container.Add(this);

            //InitializeComponent();
        }

        protected override void OnPaint(System.Windows.Forms.PaintEventArgs pe)
        {
            base.OnPaint(pe);
            Round(this.Region);  // Բ��
        }

        protected override void OnResize(EventArgs eventargs)
        {
            base.OnResize(eventargs);
            base.Refresh();
        }
    }
}