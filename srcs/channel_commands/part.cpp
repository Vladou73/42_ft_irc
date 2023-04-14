# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

//https://github.com/Vladou73/42_ft_irc/wiki/5-Channel-operations#command-part

void
Client::part()
{
    if (_parsed_cmd.size() == 1)
    {
    	send(_client_id, ERR_NEEDMOREPARAMS(_nick, "PART").c_str(), ERR_NEEDMOREPARAMS(_nick, "PART").size(), 0);
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
            	send(_client_id, ERR_INVALIDCHANNAME(chan_name).c_str(), ERR_INVALIDCHANNAME(chan_name).size(), 0);
                continue;
            }

            std::map<std::string, Channel>::iterator end = _server->_channels.end();
            if (_server->_channels.find(chan_name) == end) //Si le channel n'existe pas, renvoyer une erreur
            {
            	send(_client_id, ERR_NOSUCHCHANNEL(_nick, chan_name).c_str(), ERR_NOSUCHCHANNEL(_nick, chan_name).size(), 0);
                continue;
            }

            std::map<std::string, Channel>::iterator chan = _server->_channels.find(chan_name);
            if (chan->second._clients.find(_client_id) == chan->second._clients.end())
            {
                //si le client n'ai pas dans le channel, renvoyer une erreur
            	send(_client_id, ERR_NOTONCHANNEL(_nick, chan_name).c_str(), ERR_NOTONCHANNEL(_nick, chan_name).size(), 0);
                continue;
            }
            else
            {
                chan->second._clients.erase(_client_id);
                send(_client_id, RPL_PART(chan_name, part_msg).c_str(), RPL_PART(chan_name, part_msg).size(), 0);

                //msg aux autres clients du channel
                for (std::map<int, Client*>::iterator client = _server->_channels.find(chan_name)->second._clients.begin();
                    client != _server->_channels.find(chan_name)->second._clients.end(); client++)
                	send(client->second->_client_id, RPL_PART2(_nick, chan_name, part_msg).c_str(), RPL_PART2(_nick, chan_name, part_msg).size(), 0);
            }
        }
    }
}
