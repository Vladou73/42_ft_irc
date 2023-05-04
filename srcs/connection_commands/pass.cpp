# include "../../headers/client.hpp"

void
Client::pass()
{
	if (_parsed_cmd.size() == 1)
	{
        _msg_buffer += ERR_NEEDMOREPARAMS(_nick, "PASS");
		return;
	}
    if (_connected == true)
    {
        _msg_buffer += ERR_ALREADYREGISTERED(_nick);
        return ;
    }
    if (_parsed_cmd[1] != _server->_pwd)
	{
        _msg_buffer += ERR_PASSWDMISMATCH;
		return ;
	}
	_pass = _parsed_cmd[1];
}
