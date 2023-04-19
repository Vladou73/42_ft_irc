# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

bool
search_for_nick(std::string nick, std::map<int, Client> clients)
{
    std::map<int, Client>::iterator it;
    for (it = clients.begin(); it != clients.end(); it++)
    {
        if (nick == it->second.getNick())
            return true;
    }
    return false;
}

void
Client::kill(void)
{
    // std::cout << "***SERVER OPERS***" << std::endl;
    // for (std::vector<server_oper>::iterator it = _server->_server_opers.begin(); it != _server->_server_opers.end(); it++)
    // {
    //     std::cout << it->name << std::endl;
    //     std::cout << it->pwd << std::endl;
    //     std::cout << it->host << std::endl;
    //     std::cout << std::endl;
    // }

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
    if (search_for_nick(_parsed_cmd[1], _server->_clients) == false)
    {
        send(_client_id, ERR_NOSUCHNICK(_user, _parsed_cmd[1]).c_str(), ERR_NOSUCHNICK(_user, _parsed_cmd[1]).size(), 0);
        return;
    }

    std::cout << "attempt to kill " << _parsed_cmd[1] << std::endl;
 
    // quit()

    // send(_client_id, RPL_YOUREOPER(_nick).c_str(), RPL_YOUREOPER(_nick).size(), 0);


}