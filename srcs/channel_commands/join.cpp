# include "../../headers/channel_commands.hpp"
# include "../../headers/client.hpp"
# include "../../headers/channel.hpp"
# include "../../headers/ft_irc.hpp"

//https://github.com/Vladou73/42_ft_irc/wiki/5-Channel-operations#command-join

std::vector<std::string>
parse_commas(std::string buff)
{
    std::stringstream			strstream(buff);
	std::vector<std::string>	parsed_buff;
	std::string 				word;

	while(getline(strstream, word, ','))
	{
		parsed_buff.push_back(word);
	}
    return parsed_buff;
}

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
		std::string	mess_op("You are the Operator of this Channel !\n");
		std::string	clients_list;
        std::vector<std::string> chan_names = parse_commas(_parsed_cmd[1]);
        for (std::vector<std::string>::iterator it = chan_names.begin(); it != chan_names.end(); it++)
        {

            //Si le channel n'existe pas encore, le crée
            std::map<std::string, Channel>::iterator end = _server->_channels.end();
            if (_server->_channels.find(*it) == end)
            {
                Channel channel(*it);
                // _server->_channels.insert(std::pair<std::string, Channel>("test", channel));
				channel._id_operator =_client_id;
                _server->_channels.insert(std::pair<std::string, Channel>(*it, channel));
				_operator = true;
				send(_client_id, mess_op.c_str(), mess_op.size(), 0);
            }
			_canals.push_back(*it);

            //si le client n'ai pas déjà dans le channel, l'ajoute
            std::map<std::string, Channel>::iterator chan = _server->_channels.find(*it);
            if (chan->second._clients.find(_client_id) == chan->second._clients.end())
                chan->second._clients.insert(std::pair<int, Client *>(_client_id, this));
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
						send(it->first, JOIN_CHAN(_nick, chan_names[i]).c_str(), JOIN_CHAN(_nick, chan_names[i]).size(), 0);
                        if (it->second->_operator == true)
                        {
                            std::string temp = "@" + it->second->_nick;
                            clients_list = clients_list + " " + temp;
                        }
                        else
                            clients_list = clients_list + " " + it->second->_nick;
                        if (clients_list[0] == ' ')
                            clients_list.erase(0, 1);
                    }
                    send(_client_id, RPL_TOPIC(_nick, chan->first, chan->second._topic).c_str(), RPL_TOPIC(_nick, chan->first, chan->second._topic).size(), 0);
                    send(_client_id, RPL_NAMREPLY(_nick, chan->first, clients_list).c_str(), RPL_NAMREPLY(_nick, chan->first, clients_list).size(), 0);
                    send(_client_id, RPL_ENDOFNAMES(_nick, chan->first).c_str(), RPL_ENDOFNAMES(_nick, chan->first).size(), 0);
				}
            }
        }
        send(_client_id, SEPARATOR_END, strlen(SEPARATOR_END), 0);
    }
}