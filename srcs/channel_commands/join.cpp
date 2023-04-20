# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

//https://github.com/Vladou73/42_ft_irc/wiki/5-Channel-operations#command-join

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *  Commande: JOIN * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Paramètres: <canal>{,<canal>} [<clé>{,<clé>}]

La commande JOIN est utilisée par un client pour commencer à écouter un canal spécifique. 
L'accès à un canal est autorisé ou non uniquement par le serveur auquel le client est connecté.
Une fois qu'un utilisateur a accès à un canal, il reçoit des notifications de toutes les commandes que son serveur reçoit et qui affectent le canal. 
Cela inclut MODE, KICK, PART, QUIT, et bien sûr PRIVMSG/NOTICE. 
Si un JOIN a lieu avec succès, on envoie à l'utilisateur le sujet du canal (en utilisant RPL_TOPIC) et la liste des utilisateurs du canal (en utilisant RPL_NAMREPLY), y compris lui-même.

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
Client::join()
{
    if (_parsed_cmd.size() == 1)
    {
    	send(_client_id, ERR_NEEDMOREPARAMS(_nick, "JOIN").c_str(), ERR_NEEDMOREPARAMS(_nick, "JOIN").size(), 0);
        return ;
    }
    if (_connected == true)
    {
        //parsing du 1er argument qui contient la liste des channels envoyés pour le JOIN
        send(_client_id, SEPARATOR, strlen(SEPARATOR), 0);
		std::string	clients_list;
        std::vector<std::string> chan_names = parse_commas(_parsed_cmd[1]);
        for (std::vector<std::string>::iterator it = chan_names.begin(); it != chan_names.end(); it++)
        {
            if (check_channel_name(*it) == false)
            {
            	send(_client_id, ERR_INVALIDCHANNAME(*it).c_str(), ERR_INVALIDCHANNAME(*it).size(), 0);
                continue;
            }

            //Si le channel n'existe pas encore, le crée
            std::map<std::string, Channel>::iterator end = _server->_channels.end();
            if (_server->_channels.find(*it) == end)
            {
                std::string	mess_op = "You are the Operator of the Channel : " + *it + "\r\n";
                Channel channel(*it);
				channel._id_operator.push_back(_client_id);
                _server->_channels.insert(std::pair<std::string, Channel>(*it, channel));
				send(_client_id, mess_op.c_str(), mess_op.size(), 0);
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
                        if (check_operator(chan->second._id_operator, it->second->_client_id) == true)
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
                            send(it->first, JOIN_CHAN(_nick, chan_names[i]).c_str(), JOIN_CHAN(_nick, chan_names[i]).size(), 0);
                    }
                    if (chan->second._first_connexion.find(_client_id)->second == true)
                    {
                        send(_client_id, RPL_TOPIC(_nick, chan->first, chan->second._topic).c_str(), RPL_TOPIC(_nick, chan->first, chan->second._topic).size(), 0);
                        send(_client_id, RPL_NAMREPLY(_nick, chan->first, clients_list).c_str(), RPL_NAMREPLY(_nick, chan->first, clients_list).size(), 0);
                        send(_client_id, RPL_ENDOFNAMES(_nick, chan->first).c_str(), RPL_ENDOFNAMES(_nick, chan->first).size(), 0);
                        chan->second._first_connexion.find(_client_id)->second = false;
                    }
                    else
                        send(_client_id, ERR_ALREADYINCHAN(_nick, chan->first).c_str(), ERR_ALREADYINCHAN(_nick, chan->first).size(), 0);
                    clients_list.clear();
				}
            }
        }
        send(_client_id, SEPARATOR_END, strlen(SEPARATOR_END), 0);
    }
}
