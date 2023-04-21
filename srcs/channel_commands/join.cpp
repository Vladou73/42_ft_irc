# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

//https://github.com/Vladou73/42_ft_irc/wiki/5-Channel-operations#command-join

void
Client::join()
{
    if (_parsed_cmd.size() == 1)
    {
    	_msg_buffer += ERR_NEEDMOREPARAMS(_nick, "JOIN");
        return ;
    }
    if (_connected == true)
    {
        //parsing du 1er argument qui contient la liste des channels envoyés pour le JOIN
        _msg_buffer += SEPARATOR;
		std::string	clients_list;
        std::vector<std::string> chan_names = parse_commas(_parsed_cmd[1]);
        for (std::vector<std::string>::iterator it = chan_names.begin(); it != chan_names.end(); it++)
        {
            if (check_channel_name(*it) == false)
            {
            	_msg_buffer += ERR_INVALIDCHANNAME(*it);
                continue;
            }

            //Si le channel n'existe pas encore, le crée
            std::map<std::string, Channel>::iterator end = _server->_channels.end();
            if (_server->_channels.find(*it) == end)
            {
                std::string	mess_op = "You are the Operator of the Channel : " + *it + "\r\n";
                Channel channel(*it);
                channel._id_operators.push_back(_client_id);
                _server->_channels.insert(std::pair<std::string, Channel>(*it, channel));
				_msg_buffer += mess_op.c_str();
            }
			_canals.push_back(*it);

            //si le client n'ai pas déjà dans le channel, l'ajoute
            std::map<std::string, Channel>::iterator chan = _server->_channels.find(*it);
            if (chan->second._clients.find(_client_id) == chan->second._clients.end())
            {
                chan->second._clients.insert(std::pair<int, Client *>(_client_id, this));
                chan->second._first_connexion.insert(std::pair<int, bool>(_client_id, true));
            }
        }
        std::map<std::string, Channel>::iterator chan = _server->_channels.begin();
        std::map<std::string, Channel>::iterator chan_end = _server->_channels.end();
        for(; chan != chan_end; chan++)
        {
            for(size_t i = 0; i < chan_names.size(); i++)
            {
				if (chan->first == chan_names[i])
				{
					std::map<int, Client *>::iterator it = chan->second._clients.begin();
					std::map<int, Client *>::iterator end = chan->second._clients.end();
					for(; it != end; it++)
                    {
                        if (client_is_chann_oper(it->second->_client_id, chan->second._id_operators) == true)
                        {
                            std::string temp = "@" + it->second->_nick;
                            clients_list = clients_list + " " + temp;
                        }
                        else
                            clients_list = clients_list + " " + it->second->_nick;
                        if (clients_list[0] == ' ')
                            clients_list.erase(0, 1);
                        if (it->second->_client_id == _client_id)
                            continue ;
                        if (chan->second._first_connexion.find(_client_id)->second == true)
                            it->second->_msg_buffer += JOIN_CHAN(USER_ID2(_nick, _user), chan_names[i]);
                    }
                    if (chan->second._first_connexion.find(_client_id)->second == true)
                    {
                        _msg_buffer += JOIN_CHAN(USER_ID2(_nick, _user), chan_names[i]);
                        _msg_buffer += RPL_TOPIC(_nick, chan->first, chan->second._topic);
                        _msg_buffer += RPL_NAMREPLY(_nick, chan->first, clients_list);
                        _msg_buffer += RPL_ENDOFNAMES(_nick, chan->first);
                        chan->second._first_connexion.find(_client_id)->second = false;
                    }
                    else
                        _msg_buffer += ERR_ALREADYINCHAN(_nick, chan->first).c_str();
                    clients_list.clear();
				}
            }
        }
        _msg_buffer += SEPARATOR_END;
    }
}
