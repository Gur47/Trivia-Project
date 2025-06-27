using System.Windows;

namespace WpfApp2
{
    public partial class MainMenuWindow : Window
    {
        public MainMenuWindow()
        {
            InitializeComponent();
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            LoginWindow loginWindow = new LoginWindow();
            loginWindow.Show();
            this.Close();
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            CreateRoomWindow createRoomWindow = new CreateRoomWindow();
            createRoomWindow.Show();
            this.Close();
        }

        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            JoinRoomWindow joinRoomWindow = new JoinRoomWindow();
            joinRoomWindow.Show();
            this.Close();
        }
    }
}
