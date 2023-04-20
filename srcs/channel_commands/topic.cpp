# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"


/* * * * * * * * * * * * * Command: TOPIC * * * * * * * * * * * * * * *

   Parameters: <channel> [ <topic> ]

   The TOPIC command is used to change or view the topic of a channel.
   The topic for channel <channel> is returned if there is no <topic>
   given.  If the <topic> parameter is present, the topic for that
   channel will be changed, if this action is allowed for the user
   requesting it.  If the <topic> parameter is an empty string, the
   topic for that channel will be removed.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
Client::topic()
{
    if (_parsed_cmd.size() == 1)
    {
    	_msg_buffer += ERR_NEEDMOREPARAMS(_nick, "TOPIC").c_str();
        return ;
    }
    if (_connected == true)
	{
		std::map<std::string, Channel>::iterator chan = _server->_channels.find(_parsed_cmd[1]);
		std::string chan_name = chan->first;

		if (check_channel_name(_parsed_cmd[1]) == false)
		{
			_msg_buffer += ERR_INVALIDCHANNAME(_parsed_cmd[1]);
			return ;
		}
		if (chan == _server->_channels.end())
		{
	    	_msg_buffer += ERR_NOSUCHCHANNEL(_nick, chan_name);
			return ;
		}
		if (check_on_chan(_canals, _parsed_cmd[1]) == false)
		{
	    	_msg_buffer += ERR_NOTONCHANNEL(_nick, chan_name);
			return ;
		}
		if (chan->second._id_operator == _client_id)
		{
			if (_parsed_cmd.size() > 2)
			{
				for(size_t i = 2; i < _parsed_cmd.size(); i++)
					chan->second._topic = chan->second._topic + " " + _parsed_cmd[i];
				chan->second._topic.erase(0, 1);
			}
		}
		if (chan->second._topic.size() == 0)
		{
			_msg_buffer += RPL_NOTOPIC(_nick);
			return ;
		}
        _msg_buffer += SEPARATOR;
		_msg_buffer += RPL_TOPIC(_nick, chan->first, chan->second._topic);
        _msg_buffer += SEPARATOR_END;
	}
}
