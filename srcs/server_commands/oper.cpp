# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

#include <unistd.h>

void
Client::oper(void)
{
    if (_connected == false)
        return;
    if (_parsed_cmd.size() != 3) // on a besoin de name et password
        return;
    if (_parsed_cmd[1] != _server->_oper_name)
        return;
    if (_parsed_cmd[2] != _server->_oper_password)
    {
    	send(_client_id, ERR_PASSWDMISMATCH, strlen(ERR_PASSWDMISMATCH), 0);
        return ;      
    }
    
    std::cout << _user << std::endl;
    std::cout << _nick << std::endl;


}