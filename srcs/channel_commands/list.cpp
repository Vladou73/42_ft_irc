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
        _msg_buffer += SEPARATOR;
	    if (_parsed_cmd.size() == 1)
		{
			std::map<std::string, Channel>::iterator it = _server->_channels.begin();
			std::map<std::string, Channel>::iterator end = _server->_channels.end();
			_msg_buffer += RPL_LISTSTART(_nick);
			for(; it != end; it++)
			{
				std::stringstream ss;
				ss << it->second._clients.size();
				std::string nbr_client = ss.str();
				_msg_buffer += RPL_LIST(_nick, it->first, nbr_client, it->second._topic);
			}
			_msg_buffer += RPL_LISTEND(_nick);
		}
		else if (_parsed_cmd.size() == 2)
		{
			_msg_buffer += RPL_LISTSTART(_nick);
        	std::vector<std::string> chan_names = parse_commas(_parsed_cmd[1]);
        	for (std::vector<std::string>::iterator it = chan_names.begin(); it != chan_names.end(); it++)
			{
				if (check_channel_name(*it) == false)
				{
					 //TODO j'ai change pour ce message d'erreur car INVALIDCHANNANE n'existe pas
                	_msg_buffer += ERR_NOSUCHCHANNEL(_nick, *it);
					// _msg_buffer += ERR_INVALIDCHANNAME(*it);
					continue;
				}
				if (_server->_channels.find(*it) == _server->_channels.end())
				{
					_msg_buffer += ERR_NOSUCHCHANNEL(_nick, *it);
					continue ;
				}
				std::map<std::string, Channel>::iterator chan = _server->_channels.find(*it);
				std::stringstream ss;
				ss << chan->second._clients.size();
				std::string nbr_client = ss.str();
				_msg_buffer += RPL_LIST(_nick, chan->first, nbr_client, chan->second._topic);
			}
			_msg_buffer += RPL_LISTEND(_nick);
		}
        _msg_buffer += SEPARATOR_END;
	}
}
