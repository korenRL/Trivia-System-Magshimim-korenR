using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;

namespace TriviaClient
{
    public class PlayerResult
    {
        public string username { get; set; }
        
        public uint correctAnswerCount { get; set; }
        public uint wrongAnswerCount { get; set; }
        public uint averageAnswerTime { get; set; }
    }

    public partial class AfterGamePage : Page
    {
        public AfterGamePage()
        {
            InitializeComponent();
            LoadResults();
        }

        private void LoadResults()
        {
            try
            {
                ServerResponse response = NetworkClient.Send(NetworkClient.GET_GAME_RESULT_REQ, new { });
                if (response.Data != null && response.Data.ContainsKey("results"))
                {
                    var options = new JsonSerializerOptions { PropertyNameCaseInsensitive = true };
                    List<PlayerResult> results = JsonSerializer.Deserialize<List<PlayerResult>>(response.Data["results"].GetRawText(), options);

                    foreach (var player in results)
                    {
                        ResultsList.Items.Add(player.username + " - Correct: " + player.correctAnswerCount + " | Wrong: " + player.wrongAnswerCount + " | Avg Time: " + player.averageAnswerTime + "s");
                    }
                }
            }
            catch (Exception)
            {
            }
        }

        private void Back_Click(object sender, EventArgs e)
        {
            MainWindow.Instance.NavigateToMenu();
        }
    }
}