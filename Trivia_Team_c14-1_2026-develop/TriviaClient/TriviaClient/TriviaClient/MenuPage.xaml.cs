using System.Windows;
using System.Windows.Controls;

namespace TriviaClient
{
    public partial class MenuPage : Page
    {
        public MenuPage()
        {
            InitializeComponent();
        }

        private void HighScores_Click(object sender, RoutedEventArgs e)
        {
            MainWindow.Instance.NavigateToHighScores();
        }

        private void PersonalStats_Click(object sender, RoutedEventArgs e)
        {
            MainWindow.Instance.NavigateToPersonalStats();
        }

        private void Logout_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                NetworkClient.Send(NetworkClient.LOGOUT_CODE, new { });
            }
            catch
            {
            }

            NetworkClient.Close();
            MainWindow.Instance.NavigateToLogin();
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            MainWindow.Instance.NavigateToCreateRoom();
        }

        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            MainWindow.Instance.NavigateToJoinRoom();
        }
    }
}