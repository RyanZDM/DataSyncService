namespace Demo
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.button7 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(0, 2);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(587, 51);
            this.button1.TabIndex = 0;
            this.button1.Text = "设置屏参（注意：只需根据屏的宽高点数的颜色设置一次，发送节目时无需设置）";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(0, 59);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(587, 51);
            this.button2.TabIndex = 1;
            this.button2.Text = "一个节目下只有一个连接左移的单行文本区域";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(0, 114);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(587, 51);
            this.button3.TabIndex = 2;
            this.button3.Text = "一个节目下只有一个多行文本区";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(0, 171);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(587, 51);
            this.button4.TabIndex = 3;
            this.button4.Text = "一个节目下只有一个图片区(表格的显示通过自绘图片并通过此方式添加发送)";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(0, 228);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(587, 51);
            this.button5.TabIndex = 4;
            this.button5.Text = "一个节目下有一个连续左移的单行文本区和一个数字时钟区(多个区域都通过此方法添加)";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(0, 285);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(587, 51);
            this.button6.TabIndex = 5;
            this.button6.Text = "两个节目下各有一个单行文本区和一个数字时钟区(多节目通过此方法添加)";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(10, 342);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(567, 59);
            this.label1.TabIndex = 6;
            this.label1.Text = "提示：每个按钮下功能代码都是独立的，互不影响，可根据自己的需求选择性去看并更改其中的代码为己用。代码附详细的注示说明，如需demo里没有列出的功能，请查看LedD" +
                "ll.h文件，内有函数的详细说明,有什么不明白加QQ 2355291262";
            // 
            // button7
            // 
            this.button7.Location = new System.Drawing.Point(235, 409);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(138, 46);
            this.button7.TabIndex = 7;
            this.button7.Text = "test";
            this.button7.UseVisualStyleBackColor = true;
            this.button7.Click += new System.EventHandler(this.button7_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(588, 467);
            this.Controls.Add(this.button7);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button7;
    }
}

