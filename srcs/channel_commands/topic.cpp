# include "../../headers/channel_commands.hpp"
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


bool
check_on_chan(std::vector<std::string> canals, std::string arg)
{
	std::vector<std::string>::iterator it = canals.begin();
	std::vector<std::string>::iterator end = canals.end();
	for(; it != end; it++)
	{
		if (*it == arg)
			return true;
	}
	return false;
}

void
Client::topic()
{
    if (_parsed_cmd.size() == 1)
    {
    	send(_client_id, ERR_NEEDMOREPARAMS(_nick, "TOPIC").c_str(), ERR_NEEDMOREPARAMS(_nick, "TOPIC").size(), 0);
        return ;
    }
    if (_connected == true)
	{
		std::map<std::string, Channel>::iterator chan = _server->_channels.find(_parsed_cmd[1]);
		if (chan == _server->_channels.end())
		{
	    	send(_client_id, ERR_CHANNOTFOUND(_nick).c_str(), ERR_CHANNOTFOUND(_nick).size(), 0);
			return ;
		}
		if (check_on_chan(_canals, _parsed_cmd[1]) == false)
		{
	    	send(_client_id, ERR_NOTONCHANNEL(_nick).c_str(), ERR_NOTONCHANNEL(_nick).size(), 0);
			return ;
		}
		if (_operator == true)
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
			send(_client_id, RPL_NOTOPIC(_nick).c_str(), RPL_NOTOPIC(_nick).size(), 0);
			return ;
		}
        send(_client_id, SEPARATOR, strlen(SEPARATOR), 0);
		send(_client_id, RPL_TOPIC(_nick, chan->first, chan->second._topic).c_str(), RPL_TOPIC(_nick, chan->first, chan->second._topic).size(), 0);	
        send(_client_id, SEPARATOR_END, strlen(SEPARATOR_END), 0);
	}
}