# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

/* * * * * * * * * * * *  Command: LIST * * * * * * * * * * * * * * * *

   Parameters: [ <channel> *( "," <channel> ) [ <target> ] ]

   The list command is used to list channels and their topics.  If the
   <channel> parameter is used, only the status of that channel is
   displayed.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
Client::list()
{
    if (_connected == true)
	{
        send(_client_id, SEPARATOR, strlen(SEPARATOR), 0);
	    if (_parsed_cmd.size() == 1)
		{
			std::map<std::string, Channel>::iterator it = _server->_channels.begin();
			std::map<std::string, Channel>::iterator end = _server->_channels.end();
			send(_client_id, RPL_LISTSTART(_nick).c_str(), RPL_LISTSTART(_nick).size(), 0);
			for(; it != end; it++)
			{
				std::stringstream ss;
				ss << it->second._clients.size();
				std::string nbr_client = ss.str();
				send(_client_id, RPL_LIST(_nick, it->first, nbr_client, it->second._topic).c_str(), RPL_LIST(_nick, it->first, nbr_client, it->second._topic).size(), 0);
			}
			send(_client_id, RPL_LISTEND(_nick).c_str(), RPL_LISTEND(_nick).size(), 0);
		}
		else if (_parsed_cmd.size() == 2)
		{
			send(_client_id, RPL_LISTSTART(_nick).c_str(), RPL_LISTSTART(_nick).size(), 0);
        	std::vector<std::string> chan_names = parse_commas(_parsed_cmd[1]);
        	for (std::vector<std::string>::iterator it = chan_names.begin(); it != chan_names.end(); it++)
			{
				if (check_channel_name(*it) == false)
				{
					send(_client_id, ERR_INVALIDCHANNAME(*it).c_str(), ERR_INVALIDCHANNAME(*it).size(), 0);
					continue;
				}
				if (_server->_channels.find(*it) == _server->_channels.end())
				{
					send(_client_id, ERR_NOSUCHCHANNEL(_nick, *it).c_str(), ERR_NOSUCHCHANNEL(_nick, *it).size(), 0);
					continue ;
				}
				std::map<std::string, Channel>::iterator chan = _server->_channels.find(*it);
				std::stringstream ss;
				ss << chan->second._clients.size();
				std::string nbr_client = ss.str();
				send(_client_id, RPL_LIST(_nick, chan->first, nbr_client, chan->second._topic).c_str(), RPL_LIST(_nick, chan->first, nbr_client, chan->second._topic).size(), 0);
			}
			send(_client_id, RPL_LISTEND(_nick).c_str(), RPL_LISTEND(_nick).size(), 0);
		}
        send(_client_id, SEPARATOR_END, strlen(SEPARATOR_END), 0);
	}
}
