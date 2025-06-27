using System;
using System.Net.Sockets;
using System.Text;

namespace WpfApp2
{
    public class Communicator
    {
        private static Communicator _instance;
        private TcpClient _client;
        private NetworkStream _stream;

        public static Communicator Instance
        {
            get
            {
                if (_instance == null)
                {
                    _instance = new Communicator();
                }
                return _instance;
            }
        }

        private Communicator()
        {
            _client = new TcpClient("127.0.0.1", 20235);
            _stream = _client.GetStream();
        }

        public void SendRequest(byte requestCode, string json)
        {
            EnsureConnected();

            if (_client == null || !_client.Connected)
            {
                throw new Exception("Client is not connected to server.");
            }

            byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
            byte[] lengthBytes = BitConverter.GetBytes(jsonBytes.Length);
            byte[] buffer = new byte[1 + 4 + jsonBytes.Length];

            buffer[0] = requestCode;
            lengthBytes.CopyTo(buffer, 1);
            jsonBytes.CopyTo(buffer, 5);

            Console.WriteLine($"[Client] Sending request:");
            Console.WriteLine($"[Client]  → Code: {requestCode}");
            Console.WriteLine($"[Client]  → JSON: {json}");
            Console.WriteLine($"[Client]  → Byte Length: {jsonBytes.Length}");

            _stream.Write(buffer, 0, buffer.Length);
            Console.WriteLine("[Client] Request sent.");
        }

        public string ReceiveResponse()
        {
            try
            {
                Console.WriteLine("[Client] Waiting for server response...");

                byte[] responseCodeBytes = new byte[1];
                int totalBytesRead = _stream.Read(responseCodeBytes, 0, 1);
                if (totalBytesRead == 0)
                {
                    Console.WriteLine("[Client][ERROR] Connection closed while reading response code");
                    EnsureConnected();
                    return "";
                }
                byte responseCode = responseCodeBytes[0];
                Console.WriteLine($"[Client] Received response code: {responseCode}");

                byte[] lengthBytes = new byte[4];
                totalBytesRead = 0;
                while (totalBytesRead < 4)
                {
                    int bytesRead = _stream.Read(lengthBytes, totalBytesRead, 4 - totalBytesRead);
                    if (bytesRead == 0)
                    {
                        Console.WriteLine("[Client][ERROR] Connection closed while reading response length");
                        EnsureConnected();
                        return "";
                    }
                    totalBytesRead += bytesRead;
                }
                int length = BitConverter.ToInt32(lengthBytes, 0);
                Console.WriteLine($"[Client] Response length: {length}");

                if (length == 0)
                {
                    Console.WriteLine("[Client][WARNING] Server returned zero-length response");
                    return "";
                }

                byte[] jsonBytes = new byte[length];
                totalBytesRead = 0;
                while (totalBytesRead < length)
                {
                    int bytesRead = _stream.Read(jsonBytes, totalBytesRead, length - totalBytesRead);
                    if (bytesRead == 0)
                    {
                        Console.WriteLine("[Client][ERROR] Connection closed while reading JSON data");
                        EnsureConnected();
                        return "";
                    }
                    totalBytesRead += bytesRead;
                }

                Console.WriteLine($"[Client] Raw JSON bytes: {BitConverter.ToString(jsonBytes)}");
                string respJson = Encoding.UTF8.GetString(jsonBytes);
                Console.WriteLine($"[Client] Response JSON: {respJson}");

                return respJson;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[Client][EXCEPTION] ReceiveResponse failed: {ex.Message}");
                EnsureConnected();
                return "";
            }
        }

        public void EnsureConnected()
        {
            if (_client == null || !_client.Connected)
            {
                Console.WriteLine("[Client] Reconnecting to server...");
                if (_client != null)
                {
                    _stream?.Close();
                    _client.Close();
                }
                _client = new TcpClient("127.0.0.1", 20235);
                _stream = _client.GetStream();
            }
        }
    }
}