# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

void
Client::user_mode(void)
{
    if (_nick != _parsed_cmd[1])
    {
        send(_client_id, ERR_USERSDONTMATCH(_nick).c_str(), ERR_USERSDONTMATCH(_nick).size(), 0);
        return;
    }

    std::string str_modes;
    for (std::map<std::string, std::string>::iterator it = _modes.begin(); it != _modes.end(); it++)
    {
        if (str_modes.empty())
            str_modes == it->first;
        else
            str_modes += " " + it->first;
    }

    if (_parsed_cmd.size() == 2)
    {
        send(_client_id, RPL_UMODEIS(_nick, str_modes).c_str(), RPL_UMODEIS(_nick, str_modes).size(), 0);
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
                send(_client_id, ERR_UMODEUNKNOWNFLAG(_nick, modestring[i]).c_str(), ERR_UMODEUNKNOWNFLAG(_nick, modestring[i]).size(), 0);
        }
    }

}

void
Client::channel_mode(void)
{
    std::map<std::string, Channel>::iterator channel = _server->_channels.find(_parsed_cmd[1]);
 
    if (_parsed_cmd.size() == 2)
    {
        send(_client_id, RPL_CHANNELMODEIS(_nick, channel->first, "").c_str(), RPL_CHANNELMODEIS(_nick, channel->first, "").size(), 0);
        return;
    }
    if (channel->second._id_operator != _client_id) //TODO A MODIFIER
    {
        send(_client_id, ERR_CHANOPRIVSNEEDED(_nick, channel->first).c_str(), ERR_CHANOPRIVSNEEDED(_nick, channel->first).size(), 0);
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
                send(_client_id, ERR_NEEDMOREPARAMS(_nick, "MODE").c_str(), ERR_NEEDMOREPARAMS(_nick, "MODE").size(), 0);
                return;
            }
            int client_fd = search_for_client_by_nick(_parsed_cmd[3], _server->_clients);
            if (client_fd == -1) //si le client n'existe pas, renvoyer une erreur ?
            {
                send(_client_id, ERR_NOSUCHNICK(_user, _parsed_cmd[3]).c_str(), ERR_NOSUCHNICK(_user, _parsed_cmd[3]).size(), 0);
                return;
            }
            if (channel->second._clients.find(_client_id) == channel->second._clients.end()) //si le client n'ai pas dans le channel, renvoyer une erreur ?
            {
            	send(_client_id, ERR_NOTONCHANNEL(_nick, chan_name).c_str(), ERR_NOTONCHANNEL(_nick, chan_name).size(), 0);
                return;
            }
            if (modestring[0] == '-') //TODO A MODIFIER quand le vecteur de channel operators aura été créé par Hugo
            {
                std::cout << "itérer dans le vecteur de channel operators du channel, et supprimer l'operateur si il s'y trouve\n";                
            }
            else //TODO A MODIFIER quand le vecteur de channel operators aura été créé par Hugo
            {
                std::cout << "Ajouter le client mentionner en tant qu'operateur dans le vecteur des channel operators du channel\n";
            }
        }
        else
        {
            std::string s = std::string() + modestring[i];
            send(_client_id, ERR_UNKNOWNMODE(s, channel->first).c_str(), ERR_UNKNOWNMODE(s, channel->first).size(), 0);
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
        send(_client_id, ERR_NEEDMOREPARAMS(_nick, "MODE").c_str(), ERR_NEEDMOREPARAMS(_nick, "MODE").size(), 0);
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
            send(_client_id, ERR_NOSUCHNICK(_user, _parsed_cmd[1]).c_str(), ERR_NOSUCHNICK(_user, _parsed_cmd[1]).size(), 0);
            return;
        }
    }
    user_mode();
}