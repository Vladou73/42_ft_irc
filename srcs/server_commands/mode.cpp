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
        if (modestring.size() != 2 || (modestring[0] != '+' && modestring[0] != '-'))
        {
            _msg_buffer += ERR_UMODEUNKNOWNFLAG(_nick);
            return;
        }
        // if (modestring[0] != '+' && modestring[0] != '-')
        //     return;

        if (modestring[1] == 'o')
        {
            if (modestring[0] == '-' && _is_server_oper == true)
            {
                _is_server_oper = false;
                _modes.erase("o");
            }
        }
        else if (modestring[1] == 'i')
        {
            if (modestring[0] == '-')
                _modes.erase("i");
            else if (_modes.find("i") == _modes.end())
                _modes.insert(std::pair<std::string, std::string>("i", "+i"));
        }
        else
        {
            _msg_buffer += ERR_UMODEUNKNOWNFLAG(_nick);
            return ;
        }
        str_modes.clear();
        for (std::map<std::string, std::string>::iterator it = _modes.begin(); it != _modes.end(); it++)
            str_modes += it->second;
        _msg_buffer += RPL_UMODEIS(_nick, str_modes);
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
    std::cout << modestring.size() << std::endl;

    if (modestring.size() != 2 || (modestring[0] != '+' && modestring[0] != '-'))
    // if (modestring.size() != 2)
    {
        _msg_buffer += ERR_UNKNOWNMODE(_nick, modestring);
        return ;
    }
    if (modestring[1] == 'o')
    {
        if (_parsed_cmd.size() < 4)
        {
            _msg_buffer += ERR_NEEDMOREPARAMS(_nick, "MODE");
            return;
        }
        int target_client_fd = search_for_client_by_nick(_parsed_cmd[3], _server->_clients);
        if (target_client_fd == -1) //si le client n'existe pas, renvoyer une erreur ?
        {
            _msg_buffer += ERR_NOSUCHNICK(_nick, _parsed_cmd[3]);
            return;
        }
        if (channel->second._clients.find(target_client_fd) == channel->second._clients.end()) //si le client n'ai pas dans le channel, renvoyer une erreur ?
        {
            _msg_buffer += ERR_USERNOTINCHANNEL(_nick, _parsed_cmd[3], channel->first);
            return;
        }
        if (modestring[0] == '-')
        {
            for (std::vector<int>::iterator it = channel->second._id_operators.begin(); it != channel->second._id_operators.end(); it++)
            {
                if (*it == target_client_fd)
                {
                    channel->second._id_operators.erase(it);
                    break;
                }                    
            }
        }
        else
            channel->second._id_operators.push_back(target_client_fd);
    }
    else
        _msg_buffer += ERR_UNKNOWNMODE(_nick, modestring);
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
        // std::cout << "_parsed_cmd[1] = " << _parsed_cmd[1] << std::endl;
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