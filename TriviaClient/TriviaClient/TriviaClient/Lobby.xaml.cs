using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace TriviaClient
{
    public partial class Lobby : Page
    {
        private bool _keepRefreshing = true;

        public Lobby()
        {
            InitializeComponent();

            Thread refreshThread = new Thread(RefreshPlayersLoop);
            refreshThread.IsBackground = true;
            refreshThread.Start();

            this.Unloaded += Page_Unloaded;
        }

        private void RefreshPlayersLoop()
        {
            while (_keepRefreshing)
            {
                try
                {
                    ServerResponse response = NetworkClient.Send(NetworkClient.GET_ROOM_STATE_REQ, new { });

                    if (response.Data != null && (response.Data.ContainsKey("players") || response.Data.ContainsKey("Players")))
                    {
                        string key = response.Data.ContainsKey("players") ? "players" : "Players";
                        string playersJson = response.Data[key].GetRawText();

                        List<string> players = JsonSerializer.Deserialize<List<string>>(playersJson);

                        Dispatcher.Invoke(() =>
                        {
                            PlayersList.ItemsSource = players;
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

        private void Leave_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                ServerResponse response = NetworkClient.Send(NetworkClient.LEAVE_ROOM_REQ, new { });

                _keepRefreshing = false;
                NavigationService.Navigate(new JoinRoom());
            }
            catch (Exception)
            {
                StatusText.Text = "Error leaving room.";
            }
        }
    }
}