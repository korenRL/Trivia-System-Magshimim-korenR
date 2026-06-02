using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Text.Json;

namespace TriviaClient
{
    public class ServerResponse
    {
        public int Code { get; set; }
        public Dictionary<string, JsonElement> Data { get; set; }
    }

    public static class NetworkClient
    {
        private static TcpClient client;
        private static NetworkStream stream;
        private const string HOST = "127.0.0.1";
        private const int PORT = 8826;

        public const int LOGIN_CODE = 1;
        public const int SIGNUP_CODE = 2;
        public const int HIGH_SCORE_CODE = 8;
        public const int PERSONAL_STATS_CODE = 9;
        public const int CREATE_ROOM_CODE = 3;

        public static void ConnectNew()
        {
            Close();

            client = new TcpClient();

            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(HOST), PORT);
            client.Connect(serverEndPoint);

            stream = client.GetStream();
        }

        public static void Close()
        {
            try
            {
                if(stream != null)
                {
                    stream.Close();
                    stream = null;
                }

                if(client != null)
                {
                    client.Close();
                    client = null;
                }
            }
            catch { }
        }

        public static ServerResponse Send(int code, object data)
        {
            if(client == null || !client.Connected)
            {
                ConnectNew();
            }

            string json = JsonSerializer.Serialize(data);

            byte[] codeBytes = Encoding.UTF8.GetBytes(code.ToString());
            byte[] lengthBytes = Encoding.UTF8.GetBytes(json.Length.ToString());
            byte[] jsonBytes = Encoding.UTF8.GetBytes(json);

            byte[] packet = new byte[codeBytes.Length + lengthBytes.Length + jsonBytes.Length];

            Buffer.BlockCopy(codeBytes, 0, packet, 0, codeBytes.Length);
            Buffer.BlockCopy(lengthBytes, 0, packet, codeBytes.Length, lengthBytes.Length);
            Buffer.BlockCopy(jsonBytes, 0, packet, codeBytes.Length + lengthBytes.Length, jsonBytes.Length);

            stream.Write(packet, 0, packet.Length);
            stream.Flush();
            return Receive();
        }

        public static ServerResponse Receive()
        {
            byte[] codeBuffer = ReceiveExact(1);
            int code = codeBuffer[0];

            byte[] lengthBuffer = ReceiveExact(4);
            int length = BitConverter.ToInt32(lengthBuffer, 0);

            byte[] dataBuffer = ReceiveExact(length);
            string json = Encoding.UTF8.GetString(dataBuffer);

            return new ServerResponse
            {
                Code = code,
                Data = JsonSerializer.Deserialize<Dictionary<string, JsonElement>>(json)
            };
        }

        private static byte[] ReceiveExact(int size)
        {
            byte[] buffer = new byte[size];
            int received = 0;

            while (received < size)
            {
                int current = stream.Read(buffer, received, size - received);

                if (current == 0)
                {
                    throw new Exception("Server disconnected.");
                }

                received += current;
            }

            return buffer;
        }
    }
}
