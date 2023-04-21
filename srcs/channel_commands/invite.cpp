# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

void
Client::invite()
{
    if (_parsed_cmd.size() != 3)
    {
        _msg_buffer += ERR_NEEDMOREPARAMS(_nick, "INVITE");
        return ;
    }

	if (_connected == true)
	{
		std::map<std::string, Channel>::iterator chan = _server->_channels.find(_parsed_cmd[2]);
		if (chan == _server->_channels.end())
		{
			_msg_buffer += ERR_NOSUCHCHANNEL(_nick, _parsed_cmd[2]);
			return ;
		}
		if (check_on_chan(_canals, _parsed_cmd[2]) == false)
		{
			_msg_buffer += ERR_NOTONCHANNEL(_nick, _parsed_cmd[2]);
			return ;
		}
		if (search_for_client_by_nick_in_channel(_parsed_cmd[1], chan->second._clients) != -1)
		{
			_msg_buffer += ERR_USERONCHANNEL(_parsed_cmd[1], _parsed_cmd[2]);
			return ;
		}
		std::map<int, Client>::iterator iter = _server->_clients.begin();
		for(; iter != _server->_clients.end(); iter++)
		{
			if (iter->second._nick == _parsed_cmd[1])
			{
				iter->second._msg_buffer += INVITATION(_nick, _user, _parsed_cmd[1], _parsed_cmd[2]);
				break ;
			}
		}
		_msg_buffer += RPL_INVITING(_parsed_cmd[1], _parsed_cmd[2]);
	}
}