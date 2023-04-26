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

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
Client::_check_topic_len()
{
	if (_parsed_cmd[2].size() >= 307)
		_parsed_cmd[2].resize(307);
}

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
			//TODO j'ai change pour ce message d'erreur car INVALIDCHANNANE n'existe pas
            _msg_buffer += ERR_NOSUCHCHANNEL(_nick, _parsed_cmd[1]);
			// _msg_buffer += ERR_INVALIDCHANNAME(_parsed_cmd[1]);
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
		if (client_is_chann_oper(_client_id, chan->second._id_operators) == true)
		{
			if (_parsed_cmd.size() > 2)
			{
				// for(size_t i = 2; i < _parsed_cmd.size(); i++)
				_check_topic_len();
				chan->second._topic = _parsed_cmd[2];
				// chan->second._topic = chan->second._topic + " " + _parsed_cmd[i];
				// chan->second._topic.erase(0, 1);
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
