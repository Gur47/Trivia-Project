using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WpfApp2
{
    public class LoginRequest
    {
        public string username { get; set; }
        public string password { get; set; }
    }

    public class LoginResponse
    {
        public int status { get; set; }  // 1 = success, 0 = fail
    }

    public class SignupRequest
    {
        public string username { get; set; }
        public string password { get; set; }
        public string email { get; set; }
    }

    public class SignupResponse
    {
        public int status { get; set; }  // 1 = success, 0 = fail
    }
   
    public class CreateRoomRequest
    {
        public string roomName { get; set; }
        public uint maxUsers { get; set; }
        public uint questionCount { get; set; }
        public uint answerTimeout { get; set; }
    }

    public class JoinRoomRequest
    {
        public uint roomId { get; set; }
    }

    public class GetPlayersInRoomRequest
    {
        public uint roomId { get; set; }
    }

    public class CreateRoomResponse
    {
        public int status { get; set; }
        public uint roomId { get; set; }

    }

    public class JoinRoomResponse
    {
        public int status { get; set; }
    }

    public class GetPlayersInRoomResponse
    {
        public List<string> users { get; set; }
    }

    public class RoomData
    {
        public uint id { get; set; }
        public string name { get; set; }
        public uint maxPlayers { get; set; }
        public uint timePerQuestion { get; set; }
        public uint numOfQuestions { get; set; }
    }

    public class GetRoomsResponse
    {
        public int status { get; set; }
        public List<RoomData> rooms { get; set; } = new List<RoomData>();
    }

    public class LeaveRoomResponse
    {
        public uint status { get; set; }
    }

    public class CloseRoomResponse
    {
        public uint status { get; set; }
    }

    public class RoomStateResponse
    {
        public string name { get; set; }
        public int numOfQuestions { get; set; }
        public int timePerQuestion { get; set; }
        public int maxPlayers { get; set; }
        public int status { get; set; }
        public bool isActive { get; set; }
        public List<string> players { get; set; }

    }



}

