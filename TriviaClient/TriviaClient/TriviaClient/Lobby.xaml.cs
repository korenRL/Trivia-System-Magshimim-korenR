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

        private bool _isAdmin;
        private int _answerTimeout = 10;

        public Lobby(bool isAdmin = false, int answerTimeout = 10)
        {
            InitializeComponent();
            _isAdmin = isAdmin;
            _answerTimeout = answerTimeout;

            if (_isAdmin)
            {
                LeaveButton.Visibility = Visibility.Collapsed;
                StartGameButton.Visibility = Visibility.Visible;
                CloseRoomButton.Visibility = Visibility.Visible;
            }

            Thread refreshThread = new Thread(RefreshPlayersLoop);
            refreshThread.IsBackground = true;
            refreshThread.Start();

            this.Unloaded += Page_Unloaded;
        }

        /*
         * The lobby checks the server until the admin starts the game
         * or closes the room.
        */
        private void RefreshPlayersLoop()
        {
            while (_keepRefreshing)
            {
                try
                {
                    ServerResponse response = NetworkClient.Send(NetworkClient.GET_ROOM_STATE_REQ, new { });

                    if (response.Data != null)
                    {
                        if (response.Data.ContainsKey("answerTimeOut"))
                        {
                            _answerTimeout = response.Data["answerTimeOut"].GetInt32();
                        }
                        else if (response.Data.ContainsKey("answerTimeout"))
                        {
                            _answerTimeout = response.Data["answerTimeout"].GetInt32();
                        }

                        if (response.Data.ContainsKey("status") && response.Data["status"].GetInt32() == 0)
                        {
                            _keepRefreshing = false;

                            Dispatcher.Invoke(() =>
                            {
                                NavigationService.Navigate(new MenuPage());
                            });

                            return;
                        }

                        if (response.Data.ContainsKey("hasGameBegun") && response.Data["hasGameBegun"].GetBoolean())
                        {
                            _keepRefreshing = false;

                            Dispatcher.Invoke(() =>
                            {
                                NavigationService.Navigate(new GameScreen(_answerTimeout));
                            });

                            return;
                        }

                        if (response.Data.ContainsKey("players") || response.Data.ContainsKey("Players"))
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
                }
                catch (Exception)
                {
                }

                Thread.Sleep(3000);
            }
        }

        /*
         * If the page closes before the player finished, it should notify
         * the server so other players will not wait forever.
        */
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

        private void CloseRoom_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                ServerResponse response = NetworkClient.Send(NetworkClient.CLOSE_ROOM_REQ, new { });

                _keepRefreshing = false;
                NavigationService.Navigate(new MenuPage());
            }
            catch (Exception)
            {
                StatusText.Text = "Error closing room.";
            }
        }

        private void StartGame_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                ServerResponse response = NetworkClient.Send(NetworkClient.START_GAME_REQ, new { });

                if (response.Data != null &&
                    response.Data.ContainsKey("status") &&
                    response.Data["status"].GetInt32() == 1)
                {
                    _keepRefreshing = false;
                    NavigationService.Navigate(new GameScreen(_answerTimeout));
                }
                else
                {
                    StatusText.Text = "Error starting game.";
                }
            }
            catch (Exception)
            {
                StatusText.Text = "Error starting game.";
            }
        }
    }
}