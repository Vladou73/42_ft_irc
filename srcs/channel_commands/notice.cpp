# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

void
Client::notice()
{
	std::string mess;
	if (_parsed_cmd.size() == 3)
	{
		if (_parsed_cmd[1].at(0) == '#' || _parsed_cmd[1].at(0) == '&')
		{
			std::map<std::string, Channel>::iterator it_chan = _channel_exist();
			if (it_chan == _server->_channels.end())
				return ;
			else if (search_for_client_by_nick_in_channel(_nick, it_chan->second._clients) == -1)
				return ;
			else 
			{
				mess = _parsed_cmd[2];
				std::map<int, Client *>	::iterator it_clients_chan = it_chan->second._clients.begin();
				for (; it_clients_chan != it_chan->second._clients.end(); it_clients_chan++)
					it_clients_chan->second->_msg_buffer += RPL_NOTICE(_nick, _user, _parsed_cmd[1], mess);
			}
		}
		else
		{
			mess = _parsed_cmd[2];
			std::map<int, Client>::iterator it = _nick_exist();
			if (it == _server->_clients.end())
				return ;
			else
				it->second._msg_buffer += RPL_NOTICE(_nick, _user, it->second.getNick(), mess);
		}
	}
}
