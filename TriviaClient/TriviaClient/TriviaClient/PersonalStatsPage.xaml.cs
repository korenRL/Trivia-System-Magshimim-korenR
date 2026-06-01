using System;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;

namespace TriviaClient
{
    public partial class PersonalStatsPage : Page
    {
        public PersonalStatsPage()
        {
            InitializeComponent();
            LoadStats();
        }

        private void LoadStats()
        {
            try
            {
                StatsList.Items.Clear();

                var res = NetworkClient.Send(NetworkClient.PERSONAL_STATS_CODE, new { });

                foreach (JsonElement item in res.Data["statistics"].EnumerateArray())
                {
                    StatsList.Items.Add(item.GetString());
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
            LoadStats();
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            MainWindow.Instance.NavigateToMenu();
        }
    }
}