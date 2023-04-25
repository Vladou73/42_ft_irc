# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

std::map<int, Client>::iterator
Client::_nick_exist()
{
	std::map<int, Client>::iterator end = _server->_clients.end();
	for (std::map<int, Client>::iterator it = _server->_clients.begin(); it != end; it++)
		if (it->second.getNick() == _parsed_cmd[1])
			return (it);
	return (end);
}

std::map<std::string, Channel>::iterator
Client::_channel_exist()
{
	std::map<std::string, Channel>::iterator end = _server->_channels.end();
	for (std::map<std::string, Channel>::iterator it = _server->_channels.begin(); it != end; it++)
		if (it->first == _parsed_cmd[1])
			return (it);
	return (end);
}

//TODO si le mess est resize, faut il ajouter /r/n ?
std::string
Client::_mess_trunc()
{
	std::string mess;
	if (_parsed_cmd[2].size() >= 510)
		_parsed_cmd[2].resize(510);
	mess = _parsed_cmd[2];
	return (mess);
}

void
Client::privmsg()
{
	std::string mess;
	if (_parsed_cmd.size() == 2)
		_msg_buffer += ERR_NOTEXTTOSEND(_nick);
	if (_parsed_cmd.size() == 3)
	{
		if (_parsed_cmd[1].at(0) == '#' || _parsed_cmd[1].at(0) == '&')
		{
			std::map<std::string, Channel>::iterator it_chan = _channel_exist();
			if (it_chan == _server->_channels.end())
				_msg_buffer += ERR_NOSUCHCHANNEL(_nick, _parsed_cmd[1]);
			else 
			{
				mess = _mess_trunc();
				std::map<int, Client *>	::iterator it_clients_chan = it_chan->second._clients.begin();
				for (; it_clients_chan != it_chan->second._clients.end(); it_clients_chan++)
					it_clients_chan->second->_msg_buffer += RPL_PRIVMSG(_nick, _user, _parsed_cmd[1], mess);
			}
		}
		else
		{
			mess = _mess_trunc();
			std::map<int, Client>::iterator it = _nick_exist();
			if (it == _server->_clients.end())
				_msg_buffer += ERR_NOSUCHNICK(_nick, _parsed_cmd[1]);
			else
				it->second._msg_buffer += RPL_PRIVMSG(_nick, _user, it->second.getNick(), mess);
		}
	}
}


void
Client::msg()
{
	std::string mess;
	if (_parsed_cmd.size() == 2)
		_msg_buffer += ERR_NOTEXTTOSEND(_nick);
	if (_parsed_cmd.size() == 3)
	{
		if (_parsed_cmd[1].at(0) == '#' || _parsed_cmd[1].at(0) == '&')
		{
			std::map<std::string, Channel>::iterator it_chan = _channel_exist();
			if (it_chan == _server->_channels.end())
				_msg_buffer += ERR_NOSUCHCHANNEL(_nick, _parsed_cmd[1]);
			else 
			{
				mess = _mess_trunc();
				std::map<int, Client *>	::iterator it_clients_chan = it_chan->second._clients.begin();
				for (; it_clients_chan != it_chan->second._clients.end(); it_clients_chan++)
					it_clients_chan->second->_msg_buffer += RPL_MSG(_nick, _user, _parsed_cmd[1], mess);
			}
		}
		else
		{
			mess = _mess_trunc();
			std::map<int, Client>::iterator it = _nick_exist();
			if (it == _server->_clients.end())
				_msg_buffer += ERR_NOSUCHNICK(_nick, _parsed_cmd[1]);
			else
				it->second._msg_buffer += RPL_MSG(_nick, _user, it->second.getNick(), mess);
		}
	}
}
