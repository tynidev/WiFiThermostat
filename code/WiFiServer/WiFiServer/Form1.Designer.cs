namespace WiFiServer
{
  partial class Form1
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.tempSlider = new System.Windows.Forms.TrackBar();
      this.contrastSlider = new System.Windows.Forms.TrackBar();
      this.label1 = new System.Windows.Forms.Label();
      this.pageSetupDialog1 = new System.Windows.Forms.PageSetupDialog();
      this.label2 = new System.Windows.Forms.Label();
      this.tempLbl = new System.Windows.Forms.Label();
      this.contrastLbl = new System.Windows.Forms.Label();
      this.offsetLbl = new System.Windows.Forms.Label();
      this.label4 = new System.Windows.Forms.Label();
      this.offsetSlider = new System.Windows.Forms.TrackBar();
      ((System.ComponentModel.ISupportInitialize)(this.tempSlider)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.contrastSlider)).BeginInit();
      ((System.ComponentModel.ISupportInitialize)(this.offsetSlider)).BeginInit();
      this.SuspendLayout();
      // 
      // tempSlider
      // 
      this.tempSlider.Location = new System.Drawing.Point(12, 26);
      this.tempSlider.Maximum = 80;
      this.tempSlider.Minimum = 60;
      this.tempSlider.Name = "tempSlider";
      this.tempSlider.Orientation = System.Windows.Forms.Orientation.Vertical;
      this.tempSlider.Size = new System.Drawing.Size(45, 223);
      this.tempSlider.TabIndex = 0;
      this.tempSlider.TickStyle = System.Windows.Forms.TickStyle.Both;
      this.tempSlider.Value = 72;
      this.tempSlider.Scroll += new System.EventHandler(this.tempSlider_Scroll);
      this.tempSlider.KeyUp += new System.Windows.Forms.KeyEventHandler(this.tempSlider_KeyUp);
      this.tempSlider.MouseUp += new System.Windows.Forms.MouseEventHandler(this.tempSlider_MouseUp);
      // 
      // contrastSlider
      // 
      this.contrastSlider.Location = new System.Drawing.Point(63, 26);
      this.contrastSlider.Maximum = 63;
      this.contrastSlider.Name = "contrastSlider";
      this.contrastSlider.Orientation = System.Windows.Forms.Orientation.Vertical;
      this.contrastSlider.Size = new System.Drawing.Size(45, 223);
      this.contrastSlider.TabIndex = 1;
      this.contrastSlider.TickFrequency = 5;
      this.contrastSlider.TickStyle = System.Windows.Forms.TickStyle.Both;
      this.contrastSlider.Scroll += new System.EventHandler(this.contrastSlider_Scroll);
      this.contrastSlider.KeyUp += new System.Windows.Forms.KeyEventHandler(this.contrastSlider_KeyUp);
      this.contrastSlider.MouseUp += new System.Windows.Forms.MouseEventHandler(this.contrastSlider_MouseUp);
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(16, 10);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(34, 13);
      this.label1.TabIndex = 2;
      this.label1.Text = "Temp";
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(60, 10);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(46, 13);
      this.label2.TabIndex = 3;
      this.label2.Text = "Contrast";
      // 
      // tempLbl
      // 
      this.tempLbl.AutoSize = true;
      this.tempLbl.Location = new System.Drawing.Point(23, 252);
      this.tempLbl.Name = "tempLbl";
      this.tempLbl.Size = new System.Drawing.Size(19, 13);
      this.tempLbl.TabIndex = 4;
      this.tempLbl.Text = "75";
      // 
      // contrastLbl
      // 
      this.contrastLbl.AutoSize = true;
      this.contrastLbl.Location = new System.Drawing.Point(77, 252);
      this.contrastLbl.Name = "contrastLbl";
      this.contrastLbl.Size = new System.Drawing.Size(13, 13);
      this.contrastLbl.TabIndex = 5;
      this.contrastLbl.Text = "0";
      // 
      // offsetLbl
      // 
      this.offsetLbl.AutoSize = true;
      this.offsetLbl.Location = new System.Drawing.Point(125, 252);
      this.offsetLbl.Name = "offsetLbl";
      this.offsetLbl.Size = new System.Drawing.Size(13, 13);
      this.offsetLbl.TabIndex = 8;
      this.offsetLbl.Text = "0";
      // 
      // label4
      // 
      this.label4.AutoSize = true;
      this.label4.Location = new System.Drawing.Point(118, 10);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size(35, 13);
      this.label4.TabIndex = 7;
      this.label4.Text = "Offset";
      // 
      // offsetSlider
      // 
      this.offsetSlider.Location = new System.Drawing.Point(114, 26);
      this.offsetSlider.Minimum = -10;
      this.offsetSlider.Name = "offsetSlider";
      this.offsetSlider.Orientation = System.Windows.Forms.Orientation.Vertical;
      this.offsetSlider.Size = new System.Drawing.Size(45, 223);
      this.offsetSlider.TabIndex = 6;
      this.offsetSlider.TickStyle = System.Windows.Forms.TickStyle.Both;
      this.offsetSlider.Scroll += new System.EventHandler(this.offsetSlider_Scroll);
      this.offsetSlider.KeyUp += new System.Windows.Forms.KeyEventHandler(this.offsetSlider_KeyUp);
      this.offsetSlider.MouseUp += new System.Windows.Forms.MouseEventHandler(this.offsetSlider_MouseUp);
      // 
      // Form1
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(171, 280);
      this.Controls.Add(this.offsetLbl);
      this.Controls.Add(this.label4);
      this.Controls.Add(this.offsetSlider);
      this.Controls.Add(this.contrastLbl);
      this.Controls.Add(this.tempLbl);
      this.Controls.Add(this.label2);
      this.Controls.Add(this.label1);
      this.Controls.Add(this.contrastSlider);
      this.Controls.Add(this.tempSlider);
      this.Name = "Form1";
      this.Text = "Thermostat Control";
      this.Load += new System.EventHandler(this.Form1_Load);
      ((System.ComponentModel.ISupportInitialize)(this.tempSlider)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.contrastSlider)).EndInit();
      ((System.ComponentModel.ISupportInitialize)(this.offsetSlider)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.TrackBar tempSlider;
    private System.Windows.Forms.TrackBar contrastSlider;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.PageSetupDialog pageSetupDialog1;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.Label tempLbl;
    private System.Windows.Forms.Label contrastLbl;
    private System.Windows.Forms.Label offsetLbl;
    private System.Windows.Forms.Label label4;
    private System.Windows.Forms.TrackBar offsetSlider;
  }
}

