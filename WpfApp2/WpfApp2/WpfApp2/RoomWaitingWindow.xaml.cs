using System.Windows;
using System.Collections.Generic;
using System.Windows.Threading;
using System;

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

            LoadRoomState(); // טען פעם אחת נתונים כלליים
            LoadPlayersAndCheckState(); // טען שחקנים ובדוק האם החדר נסגר

            _refreshTimer = new DispatcherTimer();
            _refreshTimer.Interval = TimeSpan.FromSeconds(3);
            _refreshTimer.Tick += (s, e) => LoadPlayersAndCheckState();
            _refreshTimer.Start();
        }

        private void LoadRoomState()
        {
            try
            {
                _communicator.SendRequest((byte)RequestCodes.GET_ROOM_STATE_REQUEST, "");
                string responseJson = _communicator.ReceiveResponse();
                var response = _deserializer.Deserialize<RoomStateResponse>(responseJson);

                RoomInfoTextBlock.Text = $"Room '{response.name}' - Max {response.maxPlayers}, Time/Question: {response.timePerQuestion}, Questions: {response.numOfQuestions}";
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to load room state: " + ex.Message);
            }
        }

        private void LoadPlayersAndCheckState()
        {
            try
            {
                _communicator.SendRequest((byte)RequestCodes.GET_ROOM_STATE_REQUEST, "");
                string responseJson = _communicator.ReceiveResponse();
                var response = _deserializer.Deserialize<RoomStateResponse>(responseJson);

                PlayersListBox.Items.Clear();
                foreach (var username in response.players)
                {
                    PlayersListBox.Items.Add(username);
                }

                if (!response.isActive)
                {
                    MessageBox.Show("Room has been closed by admin.");
                    var menu = new MainMenuWindow();
                    menu.Show();
                    this.Close();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error while updating room state: " + ex.Message);
            }
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            if (_refreshTimer != null)
            {
                _refreshTimer.Stop();
            }
        }

        private void StartButton_Click(object sender, RoutedEventArgs e)
        {
            // יישום בעתיד: שלח START_GAME
        }

        private void CloseRoom_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                App.Communicator.SendRequest((byte)RequestCodes.CLOSE_ROOM_REQUEST, "");
                string responseJson = App.Communicator.ReceiveResponse();
                var response = App.Deserializer.Deserialize<CloseRoomResponse>(responseJson);

                if (response.status == 1)
                {
                    var menu = new MainMenuWindow();
                    menu.Show();
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Failed to close room.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error while closing room: " + ex.Message);
            }
        }

        private void LeaveRoom_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                _communicator.SendRequest((byte)RequestCodes.LEAVE_ROOM_REQUEST, "");
                string responseJson = _communicator.ReceiveResponse();
                var response = _deserializer.Deserialize<LeaveRoomResponse>(responseJson);

                if (response.status == 1)
                {
                    MainMenuWindow menu = new MainMenuWindow();
                    menu.Show();
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Failed to leave room.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error while leaving room: " + ex.Message);
            }
        }
    }
}
