# include "../../headers/client.hpp"

/*
NICKNAME =============================================================
nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
Nicknames are non-empty strings with the following restrictions:

They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
They MUST NOT start with a character listed as a channel type prefix.
They SHOULD NOT contain any dot character ('.', 0x2E).
*/



bool
Client::_forbiden_nick()
{
	if (_parsed_cmd[1].find('.') != std::string::npos ||_parsed_cmd[1].find(',') != std::string::npos
		|| _parsed_cmd[1].find('*') != std::string::npos || _parsed_cmd[1].find('?') != std::string::npos
		|| _parsed_cmd[1].find('!') != std::string::npos || _parsed_cmd[1].find('@') != std::string::npos
		||_parsed_cmd[1].find(' ') != std::string::npos || _parsed_cmd[1].size() >= 9)
		return (true);
	if (_parsed_cmd[1][0] == ':' || _parsed_cmd[1][0] == '$' || _parsed_cmd[1][0] == '#')
		return (true);
	return(false);
}

bool
Client::check_nick()
{
	std::string tmp;

	if(_forbiden_nick() == true)
	{
		if (_connected == true)
			_msg_buffer +=  ERR_ERRONEUSNICKNAME_CO(_nick, _parsed_cmd[1]);
		else
			_msg_buffer +=  ERR_ERRONEUSNICKNAME(_parsed_cmd[1]);
		return false;
	}

	std::map<int, Client>::iterator end = _server->_clients.end();
	for (std::map<int, Client>::iterator it = _server->_clients.begin(); it != end; it++)
	{
		if (it->second.getNick() == _parsed_cmd[1])
		{
			if (!_nick.empty())
				_msg_buffer += ERR_NICKNAMEINUSE_CO(_nick, _parsed_cmd[1]);
			else
				_msg_buffer += ERR_NICKNAMEINUSE(_parsed_cmd[1]);
			return (false);
		}
	}
	if (!_nick.empty())
		tmp = _nick;
	_nick = _parsed_cmd[1];
	_msg_buffer += RPL_NICK(_nick, USER_ID2(tmp, _user));
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
	if (!_pass.empty())
		check_nick();
}
