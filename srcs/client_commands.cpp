# include "../headers/client.hpp"

/*
NICKNAME =============================================================
nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
Nicknames are non-empty strings with the following restrictions:

They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
They MUST NOT start with a character listed as a channel type prefix.
They SHOULD NOT contain any dot character ('.', 0x2E).
*/

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
		_data_connexion.clear();
        _msg_buffer += ERR_PASSWDMISMATCH;
		return ;
	}
    if (_data_connexion.size() > 0)
        return ;
    // _data_connexion.clear();
    _data_connexion.push_back(_parsed_cmd[1]);
}


//TODO we take off space so the nick doesnt have any, is it a problem ?
bool
Client::_forbiden_nick()
{
	if (_parsed_cmd[1].find('.') != std::string::npos ||_parsed_cmd[1].find(',') != std::string::npos || _parsed_cmd[1].find('*') != std::string::npos || _parsed_cmd[1].find('?') != std::string::npos || _parsed_cmd[1].find('!') != std::string::npos || _parsed_cmd[1].find('@') != std::string::npos ||_parsed_cmd[1].find(' ') != std::string::npos || _parsed_cmd[1].size() >= 9)
		return (true);
	if (_parsed_cmd[1][0] == '$' || _parsed_cmd[1][0] == '#')
		return (true);
	return(false);
}

bool
Client::check_nick()
{
	std::string tmp;

	if(_forbiden_nick() == true)
	{
		_msg_buffer +=  ERR_ERRONEUSNICKNAME(_parsed_cmd[1]);
		return false;
	}

	std::map<int, Client>::iterator end = _server->_clients.end();
	for (std::map<int, Client>::iterator it = _server->_clients.begin(); it != end; it++)
	{
		if (it->second.getNick() == _parsed_cmd[1])
		{
			_msg_buffer += ERR_NICKNAMEINUSE(_parsed_cmd[1]);
			return (false);
		}
	}
	tmp = _data_connexion[1]; 
	if (_connected == false)
		_nick = _data_connexion[1];
	else
	{
		_nick = _parsed_cmd[1];
		_data_connexion[1] = _parsed_cmd[1];
	}
	_msg_buffer += USER_ID(tmp, _user);
	return true;
}

void
Client::nick()
{
	if (_parsed_cmd.size() == 1)
	{
		_msg_buffer += ERR_NONICKNAMEGIVEN;
		return;
	}
	if (_connected == true)
		check_nick();
    else if (_data_connexion.size() == 1)
    {
        _data_connexion.push_back(_parsed_cmd[1]);
        if (check_nick() == false)
    		_data_connexion.pop_back();
    }
}

void
Client::user()
{
	if (_connected == true)
	{
		_msg_buffer += ERR_ALREADYREGISTERED(_nick);
		return;
	}
	if (_data_connexion.size() == 2)
	{
		if (_parsed_cmd.size() < 5)
		{
			_msg_buffer += ERR_NEEDMOREPARAMS(_data_connexion[1], "USER");
			return ;
		}

		if (_parsed_cmd[1].find('@') == std::string::npos)
			_user = _parsed_cmd[1];
		else
			return ;

		std::string	append;
		for (size_t i = 1; i < _parsed_cmd.size(); i++)
			append += _parsed_cmd[i];
		_data_connexion.push_back(append);
		_user_infos = _data_connexion[2];

		_msg_buffer += RPL_WELCOME(_nick, USER_ID2(_nick, _user));
		_msg_buffer += RPL_YOURHOST(_nick);
		_msg_buffer += RPL_CREATED(_nick, _server->_launch_date);
		_msg_buffer += RPL_MYINFO(_nick, "+r+o+i", "");
		_msg_buffer += RPL_ISUPPORT(_nick);
		_msg_buffer += WELCOME_ART;

		_connected = true;
		_modes.insert(std::pair<std::string, std::string>("r", "+r"));
		_server->_count_clients++;
		std::cout << GREEN << "[server] clients connected = " << _server->_count_clients << std::endl;
	}
	else
		_data_connexion.clear(); //TODO :doit on clear egalement _nick et _pass ?
}

