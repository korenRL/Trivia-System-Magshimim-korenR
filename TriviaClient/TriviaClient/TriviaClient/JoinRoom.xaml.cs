using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace TriviaClient
{
    public class RoomData
    {
        public uint id { get; set; }
        public string name { get; set; }
        public uint maxPlayers { get; set; }
        public uint numOfQuestionsInGame { get; set; }
        public uint timePerQuestion { get; set; }
        public uint isActive { get; set; }
    }

    public partial class JoinRoom : Page
    {
        private bool _keepRefreshing = true;

        public JoinRoom()
        {
            InitializeComponent();

            Thread refreshThread = new Thread(RefreshRoomsLoop);
            refreshThread.IsBackground = true;
            refreshThread.Start();

            this.Unloaded += Page_Unloaded;
        }

        private void RefreshRoomsLoop()
        {
            while (_keepRefreshing)
            {
                try
                {
                    ServerResponse response = NetworkClient.Send(NetworkClient.GET_ROOMS_CODE, new { });

                    if (response.Data != null && (response.Data.ContainsKey("rooms") || response.Data.ContainsKey("Rooms")))
                    {
                        string key = response.Data.ContainsKey("rooms") ? "rooms" : "Rooms";
                        string roomsJson = response.Data[key].GetRawText();

                        var options = new JsonSerializerOptions { PropertyNameCaseInsensitive = true };
                        List<RoomData> activeRooms = JsonSerializer.Deserialize<List<RoomData>>(roomsJson, options);

                        Dispatcher.Invoke(() =>
                        {
                            RoomsList.ItemsSource = activeRooms;
                            RoomsList.DisplayMemberPath = "name";
                        });
                    }
                }
                catch (Exception)
                {
                }

                Thread.Sleep(3000);
            }
        }

        private void Page_Unloaded(object sender, RoutedEventArgs e)
        {
            _keepRefreshing = false;
        }

        private void Join_Click(object sender, RoutedEventArgs e)
        {
            if (RoomsList.SelectedItem != null)
            {
                StatusText.Foreground = new SolidColorBrush(Colors.Blue);
                StatusText.Text = "Joining room...";

                RoomData selectedRoom = (RoomData)RoomsList.SelectedItem;

                try
                {
                    ServerResponse response = NetworkClient.Send(NetworkClient.JOIN_ROOM_CODE, new { roomId = selectedRoom.id });

                    if (response.Data != null && response.Data.ContainsKey("status"))
                    {
                        int status = response.Data["status"].GetInt32();

                        if (status == 1)
                        {
                            NavigationService.Navigate(new Lobby());
                        }
                        else
                        {
                            StatusText.Foreground = new SolidColorBrush(Colors.Red);
                            StatusText.Text = "Room is full or unavailable.";
                        }
                    }
                }
                catch (Exception)
                {
                    StatusText.Foreground = new SolidColorBrush(Colors.Red);
                    StatusText.Text = "Connection error.";
                }
            }
            else
            {
                StatusText.Foreground = new SolidColorBrush(Colors.Red);
                StatusText.Text = "Please select a room from the list to join.";
            }
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            NavigationService.Navigate(new MenuPage());
        }
    }
}