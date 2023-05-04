# include "../../headers/client.hpp"

/*
PING =================================================================
The PING command is sent by either clients or servers to check the other side of the connection is still connected and/or to check for connection latency, at the application layer.
*/

void
Client::ping()
{
    if (_parsed_cmd.size() == 1)
        _msg_buffer += ERR_NEEDMOREPARAMS(_nick, "PING");
    else if (_connected == true)
        _msg_buffer += RPL_PONG(_nick, _user, _parsed_cmd[1]);
}
