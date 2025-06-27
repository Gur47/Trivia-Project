using System;
using System.Windows;
using System.Windows.Controls;

namespace WpfApp2
{
    public partial class CreateRoomWindow : Window
    {
        private Serializer _serializer = new Serializer();
        private Communicator _communicator = App.Communicator;

        public CreateRoomWindow()
        {
            InitializeComponent();
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                string roomName = RoomNameBox.Text;
                uint maxPlayers = uint.Parse(MaxPlayersBox.Text);
                uint numQuestions = uint.Parse(NumQuestionsBox.Text);
                uint timeout = uint.Parse(TimeoutBox.Text);

                var request = new CreateRoomRequest
                {
                    roomName = roomName,
                    maxUsers = maxPlayers,
                    questionCount = numQuestions,
                    answerTimeout = timeout
                };

                string json = _serializer.Serialize(request);

                _communicator.SendRequest(5, json);
                string responseJson = _communicator.ReceiveResponse();
                var deserializer = new Deserializer();
                var response = deserializer.Deserialize<CreateRoomResponse>(responseJson);

                if (response.status == 1)
                {
                    var waitingWindow = new RoomWaitingWindow(response.roomId, isAdmin: true);
                    waitingWindow.Show();
                    this.Close();
                }

                else
                {
                    try
                    {
                        var error = deserializer.Deserialize<ErrorResponse>(responseJson);
                        if (!string.IsNullOrEmpty(error.message))
                        {
                            MessageBox.Show($"Error: {error.message}");
                        }
                        else
                        {
                            MessageBox.Show("Failed to create room.");
                        }
                    }
                    catch
                    {
                        MessageBox.Show("Failed to create room.");
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message);
            }
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            MainMenuWindow menu = new MainMenuWindow();
            menu.Show();
            this.Close();
        }
    }
}
