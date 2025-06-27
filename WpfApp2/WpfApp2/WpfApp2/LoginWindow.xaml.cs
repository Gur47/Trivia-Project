using System.Windows;

namespace WpfApp2
{
    public partial class LoginWindow : Window
    {
        public LoginWindow()
        {
            InitializeComponent();
        }

        private void Login_Click(object sender, RoutedEventArgs e)
        {
            string username = UsernameBox.Text;
            string password = PasswordBox.Password; // גישה ישירה לעצם PasswordBox

            var loginRequest = new LoginRequest
            {
                username = username,
                password = password
            };

            try
            {
                string json = App.Serializer.Serialize(loginRequest);
                App.Communicator.SendRequest((byte)RequestCodes.LOGIN_REQUEST, json);

                string respJson = App.Communicator.ReceiveResponse();
                var loginResponse = App.Deserializer.Deserialize<LoginResponse>(respJson);

                if (loginResponse.status == 1)
                {
                    MainMenuWindow mainMenu = new MainMenuWindow();
                    mainMenu.Show();
                    this.Close();

                }
                else
                {
                    MessageBox.Show("Login failed. Please check your credentials.", "Error");
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show($"Error communicating with server: {ex.Message}", "Error");
            }
        }

        private void GoToSignup_Click(object sender, RoutedEventArgs e)
        {
            SignupWindow signupWindow = new SignupWindow();
            signupWindow.Show();
            this.Close();
        }

    }
}