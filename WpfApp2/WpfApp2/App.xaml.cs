using System.Windows;

namespace WpfApp2
{
    public partial class App : Application
    {
        public static Communicator Communicator { get; private set; }
        public static Serializer Serializer { get; private set; }
        public static Deserializer Deserializer { get; private set; }
        public static RoomStateResponse CurrentRoom { get; set; }

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);
            Communicator = Communicator.Instance;
            Serializer = new Serializer();
            Deserializer = new Deserializer();
        }
    }
}