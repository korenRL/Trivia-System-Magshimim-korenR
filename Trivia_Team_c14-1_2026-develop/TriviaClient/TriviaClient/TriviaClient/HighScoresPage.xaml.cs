using System;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;

namespace TriviaClient
{
    public partial class HighScoresPage : Page
    {
        public HighScoresPage()
        {
            InitializeComponent();
            LoadScores();
        }

        private void LoadScores()
        {
            try
            {
                ScoresList.Items.Clear();

                var res = NetworkClient.Send(NetworkClient.HIGH_SCORE_CODE, new { });

                foreach (JsonElement item in res.Data["statistics"].EnumerateArray())
                {
                    ScoresList.Items.Add(item.GetString());
                }

                StatusText.Text = "Loaded from DB";
            }
            catch (Exception ex)
            {
                StatusText.Text = "Error: " + ex.Message;
            }
        }

        private void Refresh_Click(object sender, RoutedEventArgs e)
        {
            LoadScores();
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            MainWindow.Instance.NavigateToMenu();
        }
    }
}