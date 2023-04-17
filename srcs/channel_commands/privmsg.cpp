# include "../../headers/channel_commands.hpp"
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
		if (it->first != _parsed_cmd[1])
			return (it);
	return (end);
}

// std::map<int, Client *>	_clients;

void
Client::privmsg()
{
	std::string mess;
	if (_parsed_cmd.size() == 3)
	{
		//TODO need the channels map
		if (_parsed_cmd[1].at(0) == '#')
		{
			// std::map<std::string, Channel>::iterator end = _server->_channels.end();
			std::map<std::string, Channel>::iterator it_chan = _channel_exist();
			if (it_chan == _server->_channels.end())
				send(_client_id, ERR_NOSUCHCHANNEL(_user, _parsed_cmd[1]).c_str(), strlen(ERR_NOSUCHCHANNEL(_user, _parsed_cmd[1]).c_str()), 0);
			else 
			{

				// send mess to all clients of a channel so need a for each loop ?
				std::map<int, Client *>	::iterator it_clients_chan = it_chan->second._clients.begin();
				for (;  it_clients_chan != it_chan->second._clients.end(); it_clients_chan++)
					send (_client_id, RPL_PRIVMSG(_nick, _user, it_clients_chan->second->getNick(), mess).c_str(), strlen(RPL_PRIVMSG(_nick, _user, it_clients_chan->second->getNick(), mess).c_str()), 0);
			}
		}
		else
		{
			mess = _parsed_cmd[2];
			std::map<int, Client>::iterator end = _server->_clients.end();
			std::map<int, Client>::iterator it = _nick_exist();
			if (it == end)
				send(_client_id, ERR_NOSUCHNICK(_user, _parsed_cmd[1]).c_str(), strlen(ERR_NOSUCHNICK(_user, _parsed_cmd[1]).c_str()), 0);
			else
				send (it->first, RPL_PRIVMSG(_nick, _user, it->second.getNick(), mess).c_str(), strlen(RPL_PRIVMSG(_nick, _user, it->second.getNick(), mess).c_str()), 0);
		}
	}
}


// for (std::map<int, Client>::iterator it = _server->_clients.begin();; it != end; it++)
// {
// 	std::cout << "nick = " << it->second.getNick() << std::endl;
// 	std::cout << "data_connexio[1] = " << _data_connexion[1] << std::endl;
// 	if (it->second.getNick() == _parsed_cmd[1])
// 	{
// 		send (it->first, RPL_PRIVMSG(_nick, _user, it->second.getNick(), mess).c_str(), strlen(RPL_PRIVMSG(_nick, _user, it->second.getNick(), mess).c_str()), 0);
// 		break ;
// 	}
// 	else
// 	{
// 		send(it->first, ERR_NOSUCHNICK(_user, _parsed_cmd[1]).c_str(), strlen(ERR_NOSUCHNICK(_user, _parsed_cmd[1]).c_str()), 0);
// 		break ;
// 	}
// }