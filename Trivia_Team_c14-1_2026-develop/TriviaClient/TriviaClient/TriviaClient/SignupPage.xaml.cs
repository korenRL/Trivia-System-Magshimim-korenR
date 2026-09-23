using System;
using System.Windows;
using System.Windows.Controls;

namespace TriviaClient
{
    public partial class SignupPage : Page
    {
        public SignupPage()
        {
            InitializeComponent();
        }

        private void Signup_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                NetworkClient.ConnectNew();

                var res = NetworkClient.Send(NetworkClient.SIGNUP_CODE, new
                {
                    username = UsernameBox.Text,
                    password = PasswordBox.Password,
                    email = EmailBox.Text
                });

                int status = res.Data["status"].GetInt32();

                if (status == 1)
                {
                    MainWindow.Instance.NavigateToMenu();
                }
                else
                {
                    StatusText.Text = "Signup failed";
                }
            }
            catch (Exception ex)
            {
                StatusText.Text = "Error: " + ex.Message;
                NetworkClient.Close();
            }
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            MainWindow.Instance.NavigateToLogin();
        }
    }
}