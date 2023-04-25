# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

void
Client::user_mode(void)
{
    if (_nick != _parsed_cmd[1])
    {
        _msg_buffer += ERR_USERSDONTMATCH(_nick);
        return;
    }

    std::string str_modes;
    for (std::map<std::string, std::string>::iterator it = _modes.begin(); it != _modes.end(); it++)
        str_modes += it->second;

    if (_parsed_cmd.size() == 2)
    {
        _msg_buffer += RPL_UMODEIS(_nick, str_modes);
        return;
    }
    else
    {
        std::string modestring = _parsed_cmd[2];
        if (modestring.size() < 2)
            return; //error
        if (modestring[0] != '+' && modestring[0] != '-')
            return;
        for (std::string::size_type i = 1; i < modestring.size(); i++)
        {
            if (modestring[i] == 'o')
            {
                if (modestring[0] == '-' && _is_server_oper == true)
                {
                    _is_server_oper = false;
                    _modes.erase("o");
                }
            }
            else if (modestring[i] == 'i')
            {
                if (modestring[0] == '-')
                    _modes.erase("i");
                else if (_modes.find("i") == _modes.end())
                    _modes.insert(std::pair<std::string, std::string>("i", "+i"));
            }
            else
                _msg_buffer += ERR_UMODEUNKNOWNFLAG(_nick, modestring[i]);
        }
    }

}

void
Client::channel_mode(void)
{
    std::map<std::string, Channel>::iterator channel = _server->_channels.find(_parsed_cmd[1]);
 
    if (_parsed_cmd.size() == 2)
    {
        _msg_buffer += RPL_CHANNELMODEIS(_nick, channel->first, "");
        return;
    }
    if (!client_is_chann_oper(_client_id, channel->second._id_operators))
    {
        _msg_buffer += ERR_CHANOPRIVSNEEDED(_nick, channel->first);
        return;
    }
    
    std::string modestring = _parsed_cmd[2];
    if (modestring.size() < 2)
        return; //error
    if (modestring[0] != '+' && modestring[0] != '-')
        return;
    for (std::string::size_type i = 1; i < modestring.size(); i++)
    {
        if (modestring[i] == 'o')
        {
            if (_parsed_cmd.size() < 3)
            {
                _msg_buffer += ERR_NEEDMOREPARAMS(_nick, "MODE");
                return;
            }
            int client_fd = search_for_client_by_nick(_parsed_cmd[3], _server->_clients);
            if (client_fd == -1) //si le client n'existe pas, renvoyer une erreur ?
            {
                _msg_buffer += ERR_NOSUCHNICK(_user, _parsed_cmd[3]);
                return;
            }
            if (channel->second._clients.find(_client_id) == channel->second._clients.end()) //si le client n'ai pas dans le channel, renvoyer une erreur ?
            {
            	_msg_buffer += ERR_NOTONCHANNEL(_nick, channel->first);
                return;
            }
            if (modestring[0] == '-') //TODO A MODIFIER quand le vecteur de channel operators aura été créé par Hugo
            {
                for (std::vector<int>::iterator it = channel->second._id_operators.begin(); it != channel->second._id_operators.end(); it++)
                {
                    if (*it == client_fd)
                    {
                        channel->second._id_operators.erase(it);
                        break;
                    }                    
                }
            }
            else //TODO A MODIFIER quand le vecteur de channel operators aura été créé par Hugo
                channel->second._id_operators.push_back(client_fd);
        }
        else
        {
            std::string s = std::string() + modestring[i];
            _msg_buffer += ERR_UNKNOWNMODE(_nick, s, channel->first);
        }
    }

}

void
Client::mode(void)
{
    if (_connected == false)
        return;
    if (_parsed_cmd.size() < 2)
    {
        _msg_buffer += ERR_NEEDMOREPARAMS(_nick, "MODE");
        return;
    }
    if (search_for_client_by_nick(_parsed_cmd[1], _server->_clients) == -1)
    {
        if (_server->_channels.find(_parsed_cmd[1]) != _server->_channels.end())
        {
            channel_mode();
            return;
        }
        else
        {
            _msg_buffer += ERR_NOSUCHNICK(_user, _parsed_cmd[1]);
            return;
        }
    }
    user_mode();
}