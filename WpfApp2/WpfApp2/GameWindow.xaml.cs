using System;
using System.Windows;
using System.Windows.Threading;

namespace WpfApp2
{
    public partial class GameWindow : Window
    {
        private readonly Communicator _communicator = App.Communicator;
        private readonly Deserializer _deserializer = new Deserializer();
        private QuestionResponse _currentQuestion;
        private string _lastQuestionText = null;
        private bool _hasSubmittedAnswer = false;
        private DispatcherTimer _pollTimer;
        private DispatcherTimer _countdownTimer;
        private int _timeLeft = 0;

        public GameWindow()
        {
            InitializeComponent();
            LoadQuestion();
        }

        private void LoadQuestion()
        {
            try
            {
                _communicator.SendRequest((byte)RequestCodes.GET_QUESTION_REQUEST, "");
                string responseJson = _communicator.ReceiveResponse();

                if (string.IsNullOrEmpty(responseJson))
                {
                    CloseWithError("No response from server.");
                    return;
                }

                if (responseJson.Contains("Please wait for all players"))
                {
                    StatusTextBlock.Text = "Waiting for all players to be ready...";
                    ScheduleRetry();
                    return;
                }

                _currentQuestion = _deserializer.DeserializeQuestionResponse(responseJson);
                if (_currentQuestion.status != 1)
                {
                    CloseWithError("Failed to load question.");
                    return;
                }

                // הצגת שאלה
                QuestionTextBlock.Text = _currentQuestion.question;
                AnswersListBox.Items.Clear();
                foreach (var answer in _currentQuestion.answers)
                {
                    AnswersListBox.Items.Add(answer);
                }

                _lastQuestionText = _currentQuestion.question;
                SubmitAnswerButton.IsEnabled = true;
                StatusTextBlock.Text = "";
                _hasSubmittedAnswer = false;

                StartCountdownTimer();
            }
            catch (Exception ex)
            {
                CloseWithError("Error loading question: " + ex.Message);
            }
        }

        private void SubmitAnswer_Click(object sender, RoutedEventArgs e)
        {
            if (_hasSubmittedAnswer || AnswersListBox.SelectedItem == null)
                return;

            SubmitAnswer(AnswersListBox.SelectedItem.ToString());
        }

        private void SubmitAnswer(string answer)
        {
            var serializer = new Serializer();
            string json = serializer.Serialize(new SubmitAnswerRequest { answer = answer });

            try
            {
                _communicator.SendRequest((byte)RequestCodes.SUBMIT_ANSWER_REQUEST, json);
                string responseJson = _communicator.ReceiveResponse();

                // 💥 אם זו תשובה אוטומטית ריקה – אל תכתוב error
                if (responseJson.Contains("error"))
                {
                    if (!string.IsNullOrEmpty(answer))
                    {
                        StatusTextBlock.Text = "Failed to submit answer.";
                    }
                    return;
                }

                var response = _deserializer.DeserializeSubmitAnswerResponse(responseJson);

                _hasSubmittedAnswer = true;
                SubmitAnswerButton.IsEnabled = false;
                StatusTextBlock.Text = "Waiting for all players to answer the question...";
                _countdownTimer?.Stop();

                StartPollingForNextQuestion();
            }
            catch
            {
                // לא להציג שגיאה – אולי עוד לא התקדמנו
                if (!string.IsNullOrEmpty(answer))
                    StatusTextBlock.Text = "Temporary error. Waiting for next question...";
            }
        }



        private void StartPollingForNextQuestion()
        {
            _pollTimer = new DispatcherTimer();
            _pollTimer.Interval = TimeSpan.FromSeconds(2);
            _pollTimer.Tick += CheckNextQuestionAvailable;
            _pollTimer.Start();
        }

        private void CheckNextQuestionAvailable(object sender, EventArgs e)
        {
            try
            {
                _communicator.SendRequest((byte)RequestCodes.GET_QUESTION_REQUEST, "");
                string responseJson = _communicator.ReceiveResponse();

                if (string.IsNullOrEmpty(responseJson) || responseJson.Contains("Please wait for all players"))
                    return;

                var question = _deserializer.DeserializeQuestionResponse(responseJson);
                if (question.status != 1)
                {
                    _pollTimer.Stop();
                    ShowGameResults();
                    return;
                }

                // אותה שאלה? אל תרענן
                if (question.question == _lastQuestionText && _timeLeft > 0)
                    return;


                // קיבלנו שאלה חדשה
                _pollTimer.Stop();
                _currentQuestion = question;
                _lastQuestionText = question.question;

                QuestionTextBlock.Text = question.question;
                AnswersListBox.Items.Clear();
                foreach (var answer in question.answers)
                {
                    AnswersListBox.Items.Add(answer);
                }

                SubmitAnswerButton.IsEnabled = true;
                StatusTextBlock.Text = "";
                _hasSubmittedAnswer = false;

                StartCountdownTimer();
            }
            catch
            {
                // המתן לניסיון הבא
            }
        }

        private void StartCountdownTimer()
        {
            _countdownTimer?.Stop();
            _timeLeft = App.CurrentRoom?.answerTimeout ?? 45; // או ברירת מחדל

            TimerTextBlock.Text = $"Time left: {_timeLeft}";

            _countdownTimer = new DispatcherTimer();
            _countdownTimer.Interval = TimeSpan.FromSeconds(1);
            _countdownTimer.Tick += (s, e) =>
            {
                _timeLeft--;
                TimerTextBlock.Text = $"Time left: {_timeLeft}";

                if (_timeLeft <= 0)
                {
                    _countdownTimer.Stop();

                    if (!_hasSubmittedAnswer)
                    {
                        SubmitAnswer(""); // שליחת תשובה ריקה תיחשב לשגויה
                    }
                }
            };

            _countdownTimer.Start();
        }

        private void ScheduleRetry()
        {
            _pollTimer = new DispatcherTimer();
            _pollTimer.Interval = TimeSpan.FromSeconds(2);
            _pollTimer.Tick += (s, e) =>
            {
                _pollTimer.Stop();
                LoadQuestion();
            };
            _pollTimer.Start();
        }

        private void ShowGameResults()
        {
            try
            {
                _communicator.SendRequest((byte)RequestCodes.GET_GAME_RESULTS_REQUEST, "{}");
                string responseJson = _communicator.ReceiveResponse();
                var results = _deserializer.DeserializeGameResultsResponse(responseJson);

                if (results.status == 1 && results.results != null)
                {
                    new GameResultsWindow(results.results).Show();
                    this.Close();
                }
                else
                {
                    CloseWithError("Failed to retrieve game results.");
                }
            }
            catch (Exception ex)
            {
                CloseWithError("Error loading game results: " + ex.Message);
            }
        }

        private void CloseWithError(string msg)
        {
            MessageBox.Show(msg);
            this.Close();
        }

        protected override void OnClosed(EventArgs e)
        {
            _pollTimer?.Stop();
            _countdownTimer?.Stop();
            base.OnClosed(e);
        }
    }
}
