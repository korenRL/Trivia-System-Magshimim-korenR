using System;
using System.Windows;
using System.Windows.Controls;

namespace TriviaClient
{
    public partial class CreateRoomPage : Page
    {
        public CreateRoomPage()
        {
            InitializeComponent();
        }

        private void Create_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var res = NetworkClient.Send(NetworkClient.CREATE_ROOM_CODE, new
                {
                    roomName = RoomNameBox.Text,
                    MaxPlayersBox = int.Parse(MaxPlayersBox.Text),
                    questionCount = int.Parse(QuestionsCountBox.Text),
                    answerCooldown = int.Parse(TimePerQuestionBox.Text)
                });

                int status = res.Data["status"].GetInt32();

                if (status == 1)
                {
                    MainWindow.Instance.NavigateToMenu();
                }
                else
                {
                    StatusText.Text = "Failed to create room";
                }
            }
            catch (Exception ex)
            {
                StatusText.Text = "Error: " + ex.Message;
            }
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            MainWindow.Instance.NavigateToMenu();
        }
    }
}