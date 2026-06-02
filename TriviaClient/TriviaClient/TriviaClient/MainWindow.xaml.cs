using System.Windows;

namespace TriviaClient
{
    public partial class MainWindow : Window
    {
        public static MainWindow Instance { get; private set; }

        public MainWindow()
        {
            InitializeComponent();
            Instance = this;
            MainFrame.Navigate(new LoginPage());
        }

        public void NavigateToLogin()
        {
            MainFrame.Navigate(new LoginPage());
        }

        public void NavigateToSignup()
        {
            MainFrame.Navigate(new SignupPage());
        }

        public void NavigateToMenu()
        {
            MainFrame.Navigate(new MenuPage());
        }

        public void NavigateToHighScores()
        {
            MainFrame.Navigate(new HighScoresPage());
        }

        public void NavigateToPersonalStats()
        {
            MainFrame.Navigate(new PersonalStatsPage());
        }

        public void NavigateToCreateRoom()
        {
            MainFrame.Navigate(new CreateRoomPage());
        }

        private void MainFrame_Navigated(object sender, System.Windows.Navigation.NavigationEventArgs e)
        {

        }
    }
}