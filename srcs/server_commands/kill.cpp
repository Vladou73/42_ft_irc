# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

void
Client::kill(void)
{
    if (_connected == false)
        return;
    if (_is_server_oper == false)
    {
    	_msg_buffer += ERR_NOPRIVILEGES(_nick);
        return;
    }
    if (_parsed_cmd.size() < 3) // on a besoin de nick et comment
    {
        _msg_buffer += ERR_NEEDMOREPARAMS(_nick, "KILL");
        return;
    }
    int client_id = search_for_client_by_nick(_parsed_cmd[1], _server->_clients);
    if (client_id == -1)
    {
        _msg_buffer += ERR_NOSUCHNICK(_user, _parsed_cmd[1]);
        return;
    }

    std::string msg = _nick + " killed " + _parsed_cmd[1] + " :" + _parsed_cmd[2];
    
    quit(_server->_clients[client_id], msg);
}