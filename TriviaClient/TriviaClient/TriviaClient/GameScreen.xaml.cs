using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;

namespace TriviaClient
{
    public partial class GameScreen : Page
    {
        private DispatcherTimer _timer;
        private int _timeLeft;
        private int _correctAnswers;

        public GameScreen()
        {
            InitializeComponent();
            _correctAnswers = 0;

            _timer = new DispatcherTimer();
            _timer.Interval = TimeSpan.FromSeconds(1);
            _timer.Tick += Timer_Tick;

            GetNextQuestion();
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            _timeLeft--;
            TimeLeftText.Text = $"Time: {_timeLeft}s";

            if (_timeLeft <= 0)
            {
                _timer.Stop();
                SubmitAnswer("");
            }
        }

        private void GetNextQuestion()
        {
            try
            {
                ServerResponse response = NetworkClient.Send(NetworkClient.GET_QUESTION_REQ, new { });

                if (response.Data != null && response.Data.ContainsKey("question"))
                {
                    string questionStr = response.Data["question"].GetString();

                    var options = new JsonSerializerOptions { PropertyNameCaseInsensitive = true };
                    List<string> answers = JsonSerializer.Deserialize<List<string>>(response.Data["answers"].GetRawText(), options);

                    QuestionText.Text = questionStr;
                    Answer1.Content = answers.Count > 0 ? answers[0] : "";
                    Answer2.Content = answers.Count > 1 ? answers[1] : "";
                    Answer3.Content = answers.Count > 2 ? answers[2] : "";
                    Answer4.Content = answers.Count > 3 ? answers[3] : "";

                    _timeLeft = 10;
                    TimeLeftText.Text = $"Time: {_timeLeft}s";
                    _timer.Start();
                }
                else
                {
                    NavigationService.Navigate(new HighScoresPage());
                }
            }
            catch (Exception)
            {
            }
        }

        private void Answer_Click(object sender, RoutedEventArgs e)
        {
            _timer.Stop();
            Button clickedButton = sender as Button;
            SubmitAnswer(clickedButton.Content.ToString());
        }

        private void SubmitAnswer(string answerText)
        {
            try
            {
                ServerResponse response = NetworkClient.Send(NetworkClient.SUBMIT_ANSWER_REQ, new { answer = answerText });

                if (response.Data != null && response.Data.ContainsKey("correctAnswerId"))
                {
                    _correctAnswers++;
                    CorrectAnswersText.Text = $"Correct: {_correctAnswers}";
                }

                GetNextQuestion();
            }
            catch (Exception)
            {
            }
        }
    }
}