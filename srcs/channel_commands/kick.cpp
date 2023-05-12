# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

void
Client::kick()
{
    if (_parsed_cmd.size() < 3)
    {
        _msg_buffer += ERR_NEEDMOREPARAMS(_nick, "KICK");
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
                _msg_buffer += ERR_NOSUCHCHANNEL(_nick, *it);
				continue ;
			}
			if (check_on_chan(_canals, *it) == false)
			{
                _msg_buffer += ERR_NOTONCHANNEL(_nick, *it);
				continue ;
			}
			if (client_is_chann_oper(_client_id, chan->second._id_operators) == false)
			{
                _msg_buffer += ERR_CHANOPRIVSNEEDED(_nick, *it);
				return ;
			}
            std::vector<int>    fd_erase;
			for (std::vector<std::string>::iterator it_client = client_names.begin(); it_client != client_names.end(); it_client++)
			{
				int fd_client = search_for_client_by_nick_in_channel(*it_client, chan->second._clients);
				if (fd_client == -1)
                {
                    _msg_buffer += ERR_USERNOTINCHANNEL(_nick ,*it_client, *it);
					continue ;
                }
				std::map<int, Client *>::iterator iter = chan->second._clients.begin();
				std::string	reason;
				// for(size_t i = 3; i < _parsed_cmd.size(); i++)
				if (_parsed_cmd.size() > 3)
					reason = _parsed_cmd[3];
				else
					reason = "";
				// reason = reason + " " + _parsed_cmd[i];
				// reason.erase(0, 1);
				for(; iter != chan->second._clients.end(); iter++)
					iter->second->_msg_buffer += RPL_KICK(_nick, _user, *it, *it_client, reason);
                fd_erase.push_back(fd_client);
			}
            std::vector<int>::iterator first = fd_erase.begin();
            std::vector<int>::iterator last = fd_erase.end();
            for (; first!= last; first++)
            {
                chan->second._clients.erase(*first);
                chan->second._first_connexion.erase(*first);
				erase_from_chan_opers(chan->second._id_operators, *first);
				for (std::vector<std::string>::iterator it = _server->_clients[*first]._canals.begin(); it != _server->_clients[*first]._canals.end(); it++) // Dans le client, suppression du nom du canal du vector _canals
                {
                    if (*it == chan->first)
                    {
                        _canals.erase(it);
                        break;
                    }
                }
            }
		}
	}
}
