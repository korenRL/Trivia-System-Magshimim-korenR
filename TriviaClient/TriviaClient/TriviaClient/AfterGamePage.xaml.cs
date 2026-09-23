using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;

namespace TriviaClient
{
    public class PlayerResult
    {
        public string username { get; set; }

        public uint correctAnswerCount { get; set; }
        public uint wrongAnswerCount { get; set; }
        public double averageAnswerTime { get; set; }
    }

    public partial class AfterGamePage : Page
    {
        private DispatcherTimer _resultsTimer;
        private bool _gotResults = false;

        public AfterGamePage()
        {
            InitializeComponent();

            _resultsTimer = new DispatcherTimer();
            _resultsTimer.Interval = TimeSpan.FromSeconds(2);
            _resultsTimer.Tick += ResultsTimer_Tick;
            _resultsTimer.Start();

            LoadResults();
        }

        // Checks until all players have finished and results are ready to go.
        private void ResultsTimer_Tick(object sender, EventArgs e)
        {
            if (!_gotResults)
            {
                LoadResults();
            }
        }

        private void LoadResults()
        {
            try
            {
                ServerResponse response = NetworkClient.Send(NetworkClient.GET_GAME_RESULT_REQ, new { });

                if (response.Data == null)
                {
                    ResultsList.Items.Clear();
                    ResultsList.Items.Add("Could not load results.");
                    return;
                }

                if (response.Data.ContainsKey("status") && response.Data["status"].GetInt32() == 0)
                {
                    ResultsList.Items.Clear();
                    ResultsList.Items.Add("Waiting for all players to finish...");
                    return;
                }

                if (response.Data.ContainsKey("results"))
                {
                    var options = new JsonSerializerOptions { PropertyNameCaseInsensitive = true };
                    List<PlayerResult> results = JsonSerializer.Deserialize<List<PlayerResult>>(response.Data["results"].GetRawText(), options);

                    if (results == null || results.Count == 0)
                    {
                        ResultsList.Items.Clear();
                        ResultsList.Items.Add("No results yet.");
                        return;
                    }

                    ResultsList.Items.Clear();

                    foreach (var player in results)
                    {
                        ResultsList.Items.Add(player.username + " - Correct: " + player.correctAnswerCount + " | Wrong: " + player.wrongAnswerCount + " | Avg Time: " + player.averageAnswerTime.ToString("0.00") + "s");
                    }

                    _gotResults = true;
                    if (_resultsTimer != null)
                    {
                        _resultsTimer.Stop();
                    }
                }
            }
            catch (Exception)
            {
                ResultsList.Items.Clear();
                ResultsList.Items.Add("Could not load results.");
            }
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                NetworkClient.Send(NetworkClient.LEAVE_GAME_REQ, new { });
            }
            catch (Exception)
            {
            }

            if (_resultsTimer != null)
            {
                _resultsTimer.Stop();
            }

            MainWindow.Instance.NavigateToMenu();
        }
    }
}