using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Threading;

namespace WpfApp2
{
    public partial class RoomWaitingWindow : Window
    {
        private readonly Communicator _communicator = App.Communicator;
        private readonly Serializer _serializer = new Serializer();
        private readonly Deserializer _deserializer = new Deserializer();
        private DispatcherTimer _refreshTimer;
        private readonly uint _roomId;
        private readonly bool _isAdmin;

        public RoomWaitingWindow(uint roomId, bool isAdmin)
        {
            InitializeComponent();
            _roomId = roomId;
            _isAdmin = isAdmin;

            Console.WriteLine($"[Client] Entered RoomWaitingWindow. RoomId: {_roomId}, IsAdmin: {_isAdmin}");

            if (_isAdmin)
            {
                StartButton.Visibility = Visibility.Visible;
                CloseRoomButton.Visibility = Visibility.Visible;
                LeaveRoomButton.Visibility = Visibility.Collapsed;
            }
            else
            {
                StartButton.Visibility = Visibility.Collapsed;
                CloseRoomButton.Visibility = Visibility.Collapsed;
                LeaveRoomButton.Visibility = Visibility.Visible;
            }

            LoadRoomState();
            LoadPlayersAndCheckState();

            _refreshTimer = new DispatcherTimer();
            _refreshTimer.Interval = TimeSpan.FromSeconds(3);
            _refreshTimer.Tick += (s, e) => LoadPlayersAndCheckState();
            _refreshTimer.Start();

            Console.WriteLine("[Client] Started polling every 3 seconds for room state...");
        }

        private void LoadRoomState()
        {
            try
            {
                Console.WriteLine("[Client] Requesting initial room state...");
                _communicator.SendRequest((byte)RequestCodes.GET_ROOM_STATE_REQUEST, "");
                string responseJson = _communicator.ReceiveResponse();
                Console.WriteLine("[Client] Received room state response JSON: " + responseJson);

                var response = _deserializer.Deserialize<RoomStateResponse>(responseJson);
                RoomInfoTextBlock.Text = $"Room - Max {response.maxPlayers}, Time/Question: {response.answerTimeout}, Questions: {response.questionCount}";
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to load room state: " + ex.Message);
                Console.WriteLine("[Client][Error] LoadRoomState failed: " + ex.Message);
            }
        }

        private void LoadPlayersAndCheckState()
        {
            try
            {
                Console.WriteLine("[Client] Polling room state...");
                _communicator.SendRequest((byte)RequestCodes.GET_ROOM_STATE_REQUEST, "");
                string responseJson = _communicator.ReceiveResponse();

                if (responseJson.Contains("Room is not joinable") || responseJson.Contains("Game already started") || responseJson.Contains("error"))
                {
                    MessageBox.Show("You cannot stay in this room because the game already started.");
                    var menu = new MainMenuWindow();
                    menu.Show();
                    this.Close();
                    return;
                }

                Console.WriteLine("[Client] Room state JSON: " + responseJson);

                var response = _deserializer.Deserialize<RoomStateResponse>(responseJson);
                App.CurrentRoom = response;


                if (response?.players == null)
                {
                    Console.WriteLine("[Client] Invalid room state response. Skipping.");
                    return;
                }

                PlayersListBox.Items.Clear();
                foreach (var username in response.players)
                {
                    PlayersListBox.Items.Add(username);
                }

                Console.WriteLine($"[Client] Players in room: {string.Join(", ", response.players)}");
                Console.WriteLine($"[Client] isActive={response.isActive}, isGameStarted={response.isGameStarted}");

                if (!response.isActive)
                {
                    Console.WriteLine("[Client] Room was closed. Returning to menu.");
                    MessageBox.Show("Room has been closed by admin.");
                    var menu = new MainMenuWindow();
                    menu.Show();
                    this.Close();
                }
                else if (response.isGameStarted)
                {
                    Console.WriteLine("[Client] Detected game start via isGameStarted=true");

                    // שליחה אחת בלבד – מחליף handler וגם מקבל שאלה
                    _communicator.SendRequest((byte)RequestCodes.GET_QUESTION_REQUEST, "");
                    string _response = _communicator.ReceiveResponse();
                    Console.WriteLine("[Client] GET_QUESTION_REQUEST response: " + _response);

                    if (_response.Contains("error") || string.IsNullOrEmpty(_response))
                    {
                        Console.WriteLine("[Client][Error] GET_QUESTION_REQUEST failed. Will try again next poll.");
                        return;
                    }

                    _refreshTimer.Stop(); // 🛑 מפסיק את הפולינג — זהו, אנחנו נכנסים למשחק

                    var gameWindow = new GameWindow();
                    gameWindow.Show();
                    this.Close();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error while updating room state: " + ex.Message);
                Console.WriteLine("[Client][Error] LoadPlayersAndCheckState failed: " + ex.Message);
            }
        }


        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            Console.WriteLine("[Client] RoomWaitingWindow closed. Stopping refresh timer.");
            _refreshTimer?.Stop();
        }

        private void StartButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Console.WriteLine("[Client] Admin clicked Start Game. Sending START_GAME_REQUEST...");
                App.Communicator.SendRequest((byte)RequestCodes.START_GAME_REQUEST, "");
                string json = App.Communicator.ReceiveResponse();
                Console.WriteLine("[Client] Received START_GAME_RESPONSE: " + json);

                var gameWindow = new GameWindow();
                gameWindow.Show();
                this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to start game: " + ex.Message);
                Console.WriteLine("[Client][Error] StartButton_Click failed: " + ex.Message);
            }
        }

        private void CloseRoom_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Console.WriteLine("[Client] Admin clicked Close Room.");
                App.Communicator.SendRequest((byte)RequestCodes.CLOSE_ROOM_REQUEST, "");
                string responseJson = App.Communicator.ReceiveResponse();
                var response = App.Deserializer.Deserialize<CloseRoomResponse>(responseJson);

                Console.WriteLine("[Client] Close room response: " + responseJson);

                if (response.status == 1)
                {
                    Console.WriteLine("[Client] Room closed successfully.");
                    var menu = new MainMenuWindow();
                    menu.Show();
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Failed to close room.");
                    Console.WriteLine("[Client] Close room failed (status != 1)");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error while closing room: " + ex.Message);
                Console.WriteLine("[Client][Error] CloseRoom_Click failed: " + ex.Message);
            }
        }

        private void LeaveRoom_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Console.WriteLine("[Client] Player clicked Leave Room.");
                _communicator.SendRequest((byte)RequestCodes.LEAVE_ROOM_REQUEST, "");
                string responseJson = _communicator.ReceiveResponse();
                var response = _deserializer.Deserialize<LeaveRoomResponse>(responseJson);

                Console.WriteLine("[Client] Leave room response: " + responseJson);

                if (response.status == 1)
                {
                    Console.WriteLine("[Client] Left room successfully.");
                    MainMenuWindow menu = new MainMenuWindow();
                    menu.Show();
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Failed to leave room.");
                    Console.WriteLine("[Client] Leave room failed (status != 1)");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error while leaving room: " + ex.Message);
                Console.WriteLine("[Client][Error] LeaveRoom_Click failed: " + ex.Message);
            }
        }
    }
}
