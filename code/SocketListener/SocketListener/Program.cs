using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace SocketListener
{
  class Program
  {
    static void Send(string msg, IPAddress ip, int port)
    {
      // Translate the passed message into ASCII and store it as a Byte array.
      Byte[] data = System.Text.Encoding.ASCII.GetBytes(msg);

      TcpListener server = null;
      try
      {
        server = new TcpListener(ip, port);

        // Start listening for client requests.
        server.Start();

        // acept client
        TcpClient client = server.AcceptTcpClient();

        // Get a stream object for reading and writing
        NetworkStream stream = client.GetStream();

        // Send the message to the connected TcpServer. 
        stream.Write(data, 0, data.Length);

        // Shutdown and end connection
        client.Close();
      }
      catch (SocketException e)
      {
        Console.WriteLine("SocketException: {0}", e);
      }
      finally
      {
        // Stop listening for new clients.
        server.Stop();
      }
    }

    static void Main(string[] args)
    {
      while (true)
      {
        string line = Console.ReadLine(); // Get string from user
        if (line == "q") // Check string
          break;

        Send(line + "\n", IPAddress.Any, 5001);
      }
    }

    
  }
}
