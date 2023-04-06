# include "utils/client_commands.hpp"


//The PING command is sent by either clients or servers to check the other side of the connection is still connected and/or to check for connection latency, at the application layer.
void
ping(bool connected, std::vector<std::string> parsed_cmds, int client_id, std::string nickname)
{
    if (parsed_cmds.size() == 1)
        send(client_id, ERR_NEEDMOREPARAMS(nickname, "PING").c_str(), ERR_NEEDMOREPARAMS(nickname, "PING").size(), 0);
    else if (connected == true)
        send(client_id, RPL_PONG(parsed_cmds[1]).c_str(), RPL_PONG(parsed_cmds[1]).size(), 0);        
}
