using System.Windows;
using System.Collections.Generic;
using System;

namespace WpfApp2
{
    public partial class JoinRoomWindow : Window
    {
        private Communicator _communicator = App.Communicator;
        private Serializer _serializer = new Serializer();
        private Deserializer _deserializer = new Deserializer();

        public JoinRoomWindow()
        {
            InitializeComponent();
            LoadRooms();
        }

        private void LoadRooms()
        {
            try
            {
                _communicator.SendRequest(6, ""); // 6 = GET_ROOMS_REQUEST
                string responseJson = _communicator.ReceiveResponse();

                if (string.IsNullOrEmpty(responseJson))
                {
                    MessageBox.Show("No rooms available.");
                    return;
                }

                Console.WriteLine("[CLIENT] Received response for GET_ROOMS:");
                Console.WriteLine(responseJson);

                var response = _deserializer.Deserialize<GetRoomsResponse>(responseJson);

                if (response.status == 1 && response.rooms != null && response.rooms.Count > 0)
                {
                    RoomsListBox.Items.Clear();
                    foreach (var room in response.rooms)
                    {
                        RoomsListBox.Items.Add($"[{room.id}] {room.name} (Max {room.maxPlayers})");
                    }
                }
                else
                {
                    MessageBox.Show("No rooms available.");
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show("Error in LoadRooms: " + ex.Message);
            }
        }


        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            if (RoomsListBox.SelectedItem == null)
            {
                MessageBox.Show("Please select a room first.");
                return;
            }

            string selected = RoomsListBox.SelectedItem.ToString();
            int roomId = int.Parse(selected.Split(']')[0].TrimStart('['));

            var request = new JoinRoomRequest { roomId = (uint)roomId };
            string json = _serializer.Serialize(request);
            _communicator.SendRequest(4, json); // 4 = JOIN_ROOM_REQUEST

            string responseJson = _communicator.ReceiveResponse();
            var response = _deserializer.Deserialize<JoinRoomResponse>(responseJson);

            if (response.status == 1)
            {
                var waitingWindow = new RoomWaitingWindow(request.roomId, isAdmin: false);
                waitingWindow.Show();
                this.Close();
            }
            else
            {
                MessageBox.Show("Failed to join room.");
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
