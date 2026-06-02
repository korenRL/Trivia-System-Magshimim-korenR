using System;
using System.Windows;
using System.Windows.Controls;

namespace TriviaClient
{
    public partial class LoginPage : Page
    {
        public LoginPage()
        {
            InitializeComponent();
        }

        private void Login_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                NetworkClient.ConnectNew();

                var res = NetworkClient.Send(NetworkClient.LOGIN_CODE, new
                {
                    username = UsernameBox.Text,
                    password = PasswordBox.Password
                });

                int status = res.Data["status"].GetInt32();

                if (status == 1)
                {
                    MainWindow.Instance.NavigateToMenu();
                }
                else
                {
                    StatusText.Text = "Login failed";
                }
            }
            catch (Exception ex)
            {
                StatusText.Text = "Error: " + ex.Message;
                NetworkClient.Close();
            }
        }

        private void GoSignup_Click(object sender, RoutedEventArgs e)
        {
            MainWindow.Instance.NavigateToSignup();
        }
    }
}