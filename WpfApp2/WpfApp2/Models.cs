using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.Json.Serialization;


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

    //public class RoomStateResponse
    //{
    //    public string name { get; set; }
    //    public int numOfQuestions { get; set; }
    //    public int timePerQuestion { get; set; }
    //    public int maxPlayers { get; set; }
    //    public int status { get; set; }
    //    public bool isActive { get; set; }
    //    public List<string> players { get; set; }
    //    public bool isGameStarted { get; set; }

    //}

    public class RoomStateResponse
    {
        public int status { get; set; }
        public List<string> players { get; set; }
        public int questionCount { get; set; }
        public int answerTimeout { get; set; }
        public int maxPlayers { get; set; }
        public bool isActive { get; set; }
        public bool isGameStarted { get; set; }
    }


    public class QuestionResponse
    {
        public uint status { get; set; }
        public string question { get; set; }
        public List<string> answers { get; set; }
    }

    public class ErrorResponse
    {
        public string message { get; set; }
    }

    public class SubmitAnswerRequest
    {
        public string answer { get; set; }
    }

    public class SubmitAnswerResponse
    {
        public uint status { get; set; }
        public bool correct { get; set; }
        public bool hasMoreQuestions { get; set; }
    }

    public class PlayerResult
    {
        [JsonPropertyName("username")]
        public string username { get; set; }

        [JsonPropertyName("correctAnswers")]
        public int correctAnswers { get; set; }

        [JsonPropertyName("totalTime")]
        public float totalTime { get; set; }

        [JsonIgnore]
        public string Rank { get; set; }
    }

    public class GameResultsResponse
    {
        public uint status { get; set; }
        public List<PlayerResult> results { get; set; } = new List<PlayerResult>();
    }

    //public class GeneralStatsResponse
    //{
    //    public uint status { get; set; }
    //    public List<PlayerResult> topCorrectAnswers { get; set; }
    //    public List<PlayerResult> topWins { get; set; }
    //    public List<PlayerResult> topAccuracy { get; set; }
    //}
    public class UserPersonalStats
    {
        public string username { get; set; }
        public int gamesPlayed { get; set; }
        public int correctAnswers { get; set; }
        public int totalAnswers { get; set; }
        public float averageTime { get; set; }
        public int wins { get; set; }
    }

    public class SimpleStat
    {
        public string username { get; set; }
        public float value { get; set; }
    }

    public class GeneralStatsResponse
    {
        public byte status { get; set; }
        public List<SimpleStat> topCorrectAnswers { get; set; }
        public List<SimpleStat> topWins { get; set; }
        public List<SimpleStat> topAccuracy { get; set; }
    }

}

