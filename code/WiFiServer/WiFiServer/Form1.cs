using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace WiFiServer
{
  public partial class Form1 : Form
  {
    public int temp = 75;
    public int contrast = 0;
    public int offset = 0;
    public int clear = 0;

    public Form1()
    {
      InitializeComponent();
    }

    private void tempSlider_Scroll(object sender, EventArgs e)
    {
      tempLbl.Text = tempSlider.Value.ToString();
      temp = tempSlider.Value;
      sendValues();
    }

    private void contrastSlider_Scroll(object sender, EventArgs e)
    {
      contrastLbl.Text = contrastSlider.Value.ToString();
      contrast = contrastSlider.Value;
      sendValues();
    }

    private void offsetSlider_Scroll(object sender, EventArgs e)
    {
      offsetLbl.Text = offsetSlider.Value.ToString();
      offset = offsetSlider.Value & 0xFF;
      sendValues();
    }

    private void tempSlider_MouseUp(object sender, MouseEventArgs e)
    {
      //sendValues();
    }

    private void contrastSlider_MouseUp(object sender, MouseEventArgs e)
    {
      //sendValues();
    }

    private void offsetSlider_MouseUp(object sender, MouseEventArgs e)
    {
      //sendValues();
    }

    private void tempSlider_KeyUp(object sender, KeyEventArgs e)
    {
      //sendValues();
    }

    private void contrastSlider_KeyUp(object sender, KeyEventArgs e)
    {
      //sendValues();
    }

    private void offsetSlider_KeyUp(object sender, KeyEventArgs e)
    {
      //sendValues();
    }

    private void sendValues()
    {
      // Translate the passed message into ASCII and store it as a Byte array.
      Byte[] data = new Byte[]{
        Convert.ToByte(temp),
        Convert.ToByte(clear),
        Convert.ToByte(contrast),
        Convert.ToByte(offset & 0xff),
        Convert.ToByte(255)
      };

      // Send the message to the connected TcpServer. 
      stream.Write(data, 0, data.Length);
      stream.Flush();
    }

    public TcpClient client = null;
    public NetworkStream stream = null;

    private void Form1_Load(object sender, EventArgs e)
    {
      try
      {
        client = new TcpClient("10.0.243.195", 2000);

        // Get a stream object for reading and writing
        stream = client.GetStream();

        sendValues();
      }
      catch (SocketException exc)
      {
        Console.WriteLine("SocketException: {0}", exc);
      }
    }

    private void Form1_SizeChanged(object sender, EventArgs e)
    {
      if (WindowState == FormWindowState.Minimized)
      {
        this.Hide();
      }
    }

    private void notifyIcon1_MouseClick(object sender, MouseEventArgs e)
    {
      this.Show();
      this.WindowState = FormWindowState.Normal;
    }

    private void clearBtn_Click(object sender, EventArgs e)
    {
      clear = 1;
      sendValues();
      clear = 0;
    }
  }
}
