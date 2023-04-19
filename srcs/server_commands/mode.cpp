# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

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
        send(_client_id, ERR_NOSUCHNICK(_user, _parsed_cmd[1]).c_str(), ERR_NOSUCHNICK(_user, _parsed_cmd[1]).size(), 0);
        return;
    }
    if (_nick != _parsed_cmd[1])
    {
        send(_client_id, ERR_USERSDONTMATCH(_nick).c_str(), ERR_USERSDONTMATCH(_nick).size(), 0);
        return;
    }

    std::string str_modes;
    for (std::map<std::string, std::string>::iterator it = _modes.begin(); it != _modes.end(); it++)
    {
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