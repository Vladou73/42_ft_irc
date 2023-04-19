# include "../../headers/ft_irc.hpp"
# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"


bool
check_oper_name(std::string name, std::vector<server_oper> oper_list)
{
    for (std::vector<struct server_oper>::iterator it = oper_list.begin(); it != oper_list.end(); it++)
    {
        if (name == it->name)
            return (true);
    }
    return false;
}

bool
check_oper_pwd(std::string pwd, std::vector<server_oper> oper_list)
{
    for (std::vector<struct server_oper>::iterator it = oper_list.begin(); it != oper_list.end(); it++)
    {
        if (pwd == it->pwd)
            return (true);
    }
    return false;
}

void
Client::oper(void)
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
    if (_parsed_cmd.size() < 3) // on a besoin de name et password
    {
        send(_client_id, ERR_NEEDMOREPARAMS(_nick, "OPER").c_str(), ERR_NEEDMOREPARAMS(_nick, "OPER").size(), 0);
        return;
    }
    if (check_oper_name(_parsed_cmd[1], _server->_server_opers) == false)
    {
    	send(_client_id, ERR_NOOPERHOST(_nick).c_str(), ERR_NOOPERHOST(_nick).size(), 0);
        return;
    }
    if (check_oper_pwd(_parsed_cmd[2], _server->_server_opers) == false)
    {
    	send(_client_id, ERR_PASSWDMISMATCH, strlen(ERR_PASSWDMISMATCH), 0);
        return ;      
    }
    //TODO : rajouter une verification du hostname ? ou pas besoin ?
    // If the client is not connecting from a valid host for the given name,
    // the server replies with an ERR_NOOPERHOST message and the request is not successful.

    _is_server_oper = true;
    send(_client_id, RPL_YOUREOPER(_nick).c_str(), RPL_YOUREOPER(_nick).size(), 0);

    //TODO : The user will also receive a MODE message indicating their new user modes, and other messages may be sent.

}