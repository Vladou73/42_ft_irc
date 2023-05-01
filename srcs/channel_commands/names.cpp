# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *  Commande: NAMES * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Paramètres: [<canal>{,<canal>}]

En utilisant la commande NAMES, un utilisateur peut obtenir la liste des pseudonymes visibles sur n'importe quel canal qu'il peut voir. 
Les noms de canaux qu'il peut voir sont ceux qui ne sont ni privés (+p), ni secrets (+s), ou ceux sur lesquels il est actuellement. 
Le paramètre <canal> spécifie quels sont les canaux dont l'information est voulue, s'ils sont valides. Il n'y a pas de message d'erreur pour les noms de canaux invalides.

Si le paramètre <canal> n'est pas donné, la liste de tous les canaux et de leurs occupants est renvoyée. 
A la fin de cette liste, sont listés les utilisateurs visibles, mais qui n'appartiennent à aucun canal visible. Ils sont listés comme appartenant au 'canal' "*".

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
Client::names()
{
   if (_connected == true)
	{
		std::string	clients_list;
        _msg_buffer += SEPARATOR;
	    if (_parsed_cmd.size() == 1)
		{
			std::map<std::string, Channel>::iterator chan = _server->_channels.begin();
			std::map<std::string, Channel>::iterator chan_end = _server->_channels.end();
			for(; chan != chan_end; chan++)
			{
				std::map<int, Client *>::iterator it = chan->second._clients.begin();
				std::map<int, Client *>::iterator end = chan->second._clients.end();
				for(; it != end; it++)
				{
					if (it->second->_modes.find("i") != it->second->_modes.end())
					{
						if (search_for_client_by_nick_in_channel(_nick, chan->second._clients) == -1)
							continue ;
					}
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
				}
				_msg_buffer += RPL_NAMREPLY(_nick, chan->first, clients_list);
				_msg_buffer += RPL_ENDOFNAMES(_nick, chan->first);
				clients_list.clear();
			}
			std::map<int, Client>::iterator it = _server->_clients.begin();
			std::map<int, Client>::iterator end = _server->_clients.end();
			for (; it != end; it++)
			{
				if (it->second._canals.size() == 0 && it->second._modes.find("i") == it->second._modes.end())
				{
					clients_list = clients_list + " " + it->second._nick;
					if (clients_list[0] == ' ')
						clients_list.erase(0, 1);
				}
			}
			std::string	temp = "*";
			_msg_buffer += RPL_NAMREPLY(_nick, temp, clients_list);
			_msg_buffer += RPL_ENDOFNAMES(_nick, temp);
			clients_list.clear();
		}
		else if (_parsed_cmd.size() == 2)
		{
        	std::vector<std::string> chan_names = parse_commas(_parsed_cmd[1]);
        	for (std::vector<std::string>::iterator it = chan_names.begin(); it != chan_names.end(); it++)
			{
				// if (check_channel_name(*it) == false)
				// 	continue;
				if (_server->_channels.find(*it) == _server->_channels.end())
				{
					_msg_buffer += RPL_ENDOFNAMES(_nick, *it);
					continue ;
				}
				std::map<std::string, Channel>::iterator chan = _server->_channels.find(*it);
				std::map<int, Client *>::iterator it_client = chan->second._clients.begin();
				std::map<int, Client *>::iterator end_client = chan->second._clients.end();
				for(; it_client != end_client; it_client++)
				{
					if (it_client->second->_modes.find("i") != it_client->second->_modes.end())
					{
						if (search_for_client_by_nick_in_channel(_nick, chan->second._clients) == -1)
							continue ;
					}
					if (client_is_chann_oper(it_client->second->_client_id, chan->second._id_operators) == true)
					{
						std::string temp = "@" + it_client->second->_nick;
						clients_list = clients_list + " " + temp;
					}
					else
						clients_list = clients_list + " " + it_client->second->_nick;
					if (clients_list[0] == ' ')
						clients_list.erase(0, 1);
					if (it_client->second->_client_id == _client_id)
						continue ;
				}
				_msg_buffer += RPL_NAMREPLY(_nick, chan->first, clients_list);
				_msg_buffer += RPL_ENDOFNAMES(_nick, chan->first);
				clients_list.clear();
			}
		}
        _msg_buffer += SEPARATOR_END;
	}
}