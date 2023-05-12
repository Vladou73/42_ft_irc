# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

//https://github.com/Vladou73/42_ft_irc/wiki/5-Channel-operations#command-part

void
Client::part()
{
    if (_parsed_cmd.size() == 1)
    {
    	_msg_buffer += ERR_NEEDMOREPARAMS(_nick, "PART");
        return ;
    }
    if (_connected == true)
    {
        std::string part_msg = "";
        if (_parsed_cmd.size() > 2) //recuperation du message dans la commande
            part_msg = _parsed_cmd[2];

        //parsing du 1er argument qui contient la liste des channels envoyés pour le PART
        std::vector<std::string> chan_names = parse_commas(_parsed_cmd[1]);
        for (std::vector<std::string>::iterator it = chan_names.begin(); it != chan_names.end(); it++)
        {
            std::string chan_name = *it;
            if (check_channel_name(chan_name) == false)
            {
                _msg_buffer += ERR_NOSUCHCHANNEL(_nick, chan_name);
                continue;
            }

            std::map<std::string, Channel>::iterator end = _server->_channels.end();
            if (_server->_channels.find(chan_name) == end) //Si le channel n'existe pas, renvoyer une erreur
            {
            	_msg_buffer += ERR_NOSUCHCHANNEL(_nick, chan_name);
                continue;
            }

            std::map<std::string, Channel>::iterator chan = _server->_channels.find(chan_name);
            if (chan->second._clients.find(_client_id) == chan->second._clients.end())
            {
                //si le client n'ai pas dans le channel, renvoyer une erreur
            	_msg_buffer += ERR_NOTONCHANNEL(_nick, chan_name);
                continue;
            }
            else
            {
                chan->second._clients.erase(_client_id); //Dans le channel, suppression du client
                for (std::vector<std::string>::iterator it = _canals.begin(); it != _canals.end(); it++) // Dans le client, suppression du nom du canal du vector _canals
                {
                    if (*it == chan_name)
                    {
                        _canals.erase(it);
                        break;
                    }
                }
                chan->second._first_connexion.erase(chan->second._first_connexion.find(_client_id));
                erase_from_chan_opers(chan->second._id_operators, _client_id);
                _msg_buffer += RPL_PART(USER_ID2(_nick, _user), chan_name, part_msg);

                //msg aux autres clients du channel
                for (std::map<int, Client*>::iterator client = _server->_channels.find(chan_name)->second._clients.begin();
                    client != _server->_channels.find(chan_name)->second._clients.end(); client++)
                	client->second->_msg_buffer += RPL_PART(USER_ID2(_nick, _user), chan_name, part_msg);
            }
        }
    }
}
