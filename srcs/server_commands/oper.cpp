# include "../../headers/ft_irc.hpp"
# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"


int
check_oper_name(std::string name, std::vector<server_oper> oper_list)
{
    int i = 0;
    for (std::vector<struct server_oper>::iterator it = oper_list.begin(); it != oper_list.end(); it++)
    {
        if (name == it->name)
            return (i);
        i++;
    }
    return -1;
}

// bool
// check_oper_pwd(std::string pwd, std::vector<server_oper> oper_list)
// {
//     for (std::vector<struct server_oper>::iterator it = oper_list.begin(); it != oper_list.end(); it++)
//     {
//         if (pwd == it->pwd)
//             return (true);
//     }
//     return false;
// }

void
Client::oper(void)
{
    if (_connected == false)
        return;
    if (_is_server_oper == true)
        return;
    if (_parsed_cmd.size() < 3) // on a besoin de name et password
    {
        _msg_buffer += ERR_NEEDMOREPARAMS(_nick, "OPER");
        return;
    }
    int nameIndex = check_oper_name(_parsed_cmd[1], _server->_server_opers);
    if (nameIndex == -1)
    {
    	_msg_buffer += _client_id, ERR_NOOPERHOST(_nick);
        return;
    }
    if (_parsed_cmd[2] != _server->_server_opers[nameIndex].pwd)
    {
    	_msg_buffer += ERR_PASSWDMISMATCH;
        return ;      
    }
    //TODO : rajouter une verification du hostname ? ou pas besoin ?
    // If the client is not connecting from a valid host for the given name,
    // the server replies with an ERR_NOOPERHOST message and the request is not successful.

    _is_server_oper = true;
    _modes.insert(std::pair<std::string, std::string>("o", "+o"));
    _msg_buffer += RPL_YOUREOPER(_nick);

    //TODO : The user will also receive a MODE message indicating their new user modes, and other messages may be sent.

}