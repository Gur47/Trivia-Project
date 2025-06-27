using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace WpfApp2
{
    public partial class SignupWindow : Window
    {
        public SignupWindow()
        {
            InitializeComponent();
        }

        private void Signup_Click(object sender, RoutedEventArgs e)
        {
            string username = UsernameBox.Text;
            string password = PasswordBox.Password;
            string email = EmailBox.Text;

            var signupRequest = new SignupRequest
            {
                username = username,
                password = password,
                email = email
            };

            try
            {
                string json = App.Serializer.Serialize(signupRequest);
                App.Communicator.SendRequest((byte)RequestCodes.SIGNUP_REQUEST, json);

                string respJson = App.Communicator.ReceiveResponse();
                var signupResponse = App.Deserializer.Deserialize<SignupResponse>(respJson);

                if (signupResponse.status == 1)
                {
                    MessageBox.Show("Signup successful! Please login.", "Success");
                    LoginWindow loginWindow = new LoginWindow();
                    loginWindow.Show();
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Signup failed. Username or email may already be taken.", "Error");
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show($"Error communicating with server: {ex.Message}", "Error");
            }
        }

        private void BackToLogin_Click(object sender, RoutedEventArgs e)
        {
            LoginWindow loginWindow = new LoginWindow();
            loginWindow.Show();
            this.Close();
        }
    }
}