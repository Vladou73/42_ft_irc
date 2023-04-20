# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

int
check_on_chan_2(std::map<int, Client *>	clients, std::string chan_name, std::string client_name, int client_id)
{
	std::map<int, Client *>::iterator it = clients.begin();
	std::map<int, Client *>::iterator end = clients.end();
	for(; it != end; it++)
	{
		if (it->second->getNick() == client_name)
			return it->second->getClientID();;
	}
	send(client_id, ERR_NOTONCHANNEL(client_name, chan_name).c_str(), ERR_NOTONCHANNEL(client_name, chan_name).size(), 0);
	return (0);
}

void
Client::kick()
{
    if (_parsed_cmd.size() < 3)
    {
    	send(_client_id, ERR_NEEDMOREPARAMS(_nick, "KICK").c_str(), ERR_NEEDMOREPARAMS(_nick, "KICK").size(), 0);
        return ;
    }
    if (_connected == true)
	{	
		std::vector<std::string> chan_names = parse_commas(_parsed_cmd[1]);
		std::vector<std::string> client_names = parse_commas(_parsed_cmd[2]);
		for (std::vector<std::string>::iterator it = chan_names.begin(); it != chan_names.end(); it++)
		{
			std::map<std::string, Channel>::iterator chan = _server->_channels.find(*it);
			if (chan == _server->_channels.end())
			{
				send(_client_id, ERR_NOSUCHCHANNEL(_nick, *it).c_str(), ERR_NOSUCHCHANNEL(_nick, *it).size(), 0);
				continue ;
			}
			if (check_on_chan(_canals, *it) == false)
			{
				send(_client_id, ERR_NOTONCHANNEL(_nick, *it).c_str(), ERR_NOTONCHANNEL(_nick, *it).size(), 0);
				continue ;
			}
			if (check_operator(chan->second._id_operator, _client_id) == false)
			{
				send(_client_id, ERR_CHANOPRIVSNEEDED(*it).c_str(), ERR_CHANOPRIVSNEEDED(*it).size(), 0);
				return ;
			}
			for (std::vector<std::string>::iterator it_client = client_names.begin(); it_client != client_names.end(); it_client++)
			{
				int fd_client = check_on_chan_2(chan->second._clients, *it, *it_client, _client_id);
				if (fd_client == 0)
					continue ;
				std::map<int, Client *>::iterator iter = chan->second._clients.begin();
				std::map<int, Client *>::iterator client_kick = chan->second._clients.find(fd_client);
				if (_parsed_cmd.size() == 3)
				{
					for(; iter != chan->second._clients.end(); iter++)
					send(iter->second->_client_id, RPL_KICK(_nick, _user, _nick, _nick, _nick), )
				}
				chan->second._clients.erase(fd_client);
			}
		}
	}
}