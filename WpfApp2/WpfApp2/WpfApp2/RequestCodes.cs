namespace WpfApp2
{
    public enum RequestCodes : byte
    {
        LOGIN_REQUEST = 1,
        SIGNUP_REQUEST = 2,
        USERS_LIST_REQUEST = 3,
        JOIN_ROOM_REQUEST = 4,
        CREATE_ROOM_REQUEST = 5,
        GET_ROOMS_REQUEST = 6,
        LEAVE_ROOM_REQUEST = 7,
        CLOSE_ROOM_REQUEST = 8,
        START_GAME_REQUEST = 9,
        GET_PLAYERS_IN_ROOM = 10,
        GET_ROOM_STATE_REQUEST = 11    
    }
}
