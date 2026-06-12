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
        private int _timePerQuestion;

        public GameScreen(int timePerQuestion = 10)
        {
            InitializeComponent();
            _correctAnswers = 0;
            _timePerQuestion = timePerQuestion;
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
                SubmitAnswer(4);
            }
        }

        private void GetNextQuestion()
        {
            try
            {
                ServerResponse response = NetworkClient.Send(NetworkClient.GET_QUESTION_REQ, new { });

                if (response.Data != null && response.Data.ContainsKey("status") && response.Data["status"].GetInt32() == 0)
                {
                    MainWindow.Instance.NavigateToAfterGame();
                    return;
                }

                if (response.Data != null && response.Data.ContainsKey("question"))
                {
                    QuestionText.Text = response.Data["question"].GetString();

                    string[] answers = new string[4];
                    foreach (var prop in response.Data["answers"].EnumerateObject())
                    {
                        int index = int.Parse(prop.Name);
                        if (index >= 0 && index < 4)
                        {
                            answers[index] = prop.Value.GetString();
                        }
                    }

                    Answer1.Content = answers[0];
                    Answer2.Content = answers[1];
                    Answer3.Content = answers[2];
                    Answer4.Content = answers[3];

                    _timeLeft = _timePerQuestion;
                    TimeLeftText.Text = $"Time: {_timeLeft}s";
                    _timer.Start();
                }
            }
            catch (Exception)
            {
            }
        }

        private void Answer_Click(object sender, RoutedEventArgs e)
        {
            _timer.Stop();

            int answerId = 0;
            if (sender == Answer2)
            {
                answerId = 1;
            }
            else if (sender == Answer3)
            {
                answerId = 2;
            }
            else if (sender == Answer4)
            {
                answerId = 3;
            }

            SubmitAnswer(answerId);
        }

        private void SubmitAnswer(int answerId)
        {
            try
            {
                ServerResponse response = NetworkClient.Send(NetworkClient.SUBMIT_ANSWER_REQ, new { answerId = answerId });

                if (response.Data != null && response.Data.ContainsKey("correctAnswerId"))
                {
                    int correctId = response.Data["correctAnswerId"].GetInt32();
                    if (correctId == answerId)
                    {
                        _correctAnswers++;
                        CorrectAnswersText.Text = $"Correct: {_correctAnswers}";
                    }
                }

                GetNextQuestion();
            }
            catch (Exception)
            {
            }
        }
    }
}