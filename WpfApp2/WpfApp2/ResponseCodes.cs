using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WpfApp2
{
    public enum ResponseCodes : byte
    {
        LOGIN_RESPONSE = 1,
        SIGNUP_RESPONSE = 2,
        ERROR_RESPONSE = 3,
        USERS_LIST_RESPONSE = 4,
        JOIN_ROOM_RESPONSE = 5,
        CREATE_ROOM_RESPONSE = 6,
        GET_ROOMS_RESPONSE = 7,
        GET_PLAYERS_IN_ROOM_RESPONSE = 8,
        LEAVE_ROOM_RESPONSE = 9,
        CLOSE_ROOM_RESPONSE = 10,
        GET_ROOM_STATE_RESPONSE = 11,
        START_GAME_RESPONSE = 12,
        GET_QUESTION_RESPONSE = 13,
        SUBMIT_ANSWER_RESPONSE = 14,
        GET_GAME_RESULTS_RESPONSE = 15,
        GET_PERSONAL_STATS_RESPONSE = 16,
        GET_GENERAL_STATS_RESPONSE = 17,
    }
}

