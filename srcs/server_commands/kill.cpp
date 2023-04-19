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
    	send(_client_id, ERR_NOPRIVILEGES(_nick).c_str(), ERR_NOPRIVILEGES(_nick).size(), 0);
        return;
    }
    if (_parsed_cmd.size() < 3) // on a besoin de nick et comment
    {
        send(_client_id, ERR_NEEDMOREPARAMS(_nick, "KILL").c_str(), ERR_NEEDMOREPARAMS(_nick, "KILL").size(), 0);
        return;
    }
    int client_id = search_for_client_by_nick(_parsed_cmd[1], _server->_clients);
    if (client_id == -1)
    {
        send(_client_id, ERR_NOSUCHNICK(_user, _parsed_cmd[1]).c_str(), ERR_NOSUCHNICK(_user, _parsed_cmd[1]).size(), 0);
        return;
    }

    std::string msg = _nick + " killed " + _parsed_cmd[1] + " :" + _parsed_cmd[2];
    
    quit(_server->_clients[client_id], msg);

}