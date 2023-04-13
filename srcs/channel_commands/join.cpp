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
        std::vector<std::string> chan_names = parse_commas(_parsed_cmd[1]);
        for (std::vector<std::string>::iterator it = chan_names.begin(); it != chan_names.end(); it++)
        {

            //Si le channel n'existe pas encore, le crée
            std::map<std::string, Channel>::iterator end = _server->_channels.end();
            if (_server->_channels.find(*it) == end)
            {
                Channel channel(*it);
                // _server->_channels.insert(std::pair<std::string, Channel>("test", channel));
                _server->_channels.insert(std::pair<std::string, Channel>(*it, channel));
            }

            //si le client n'ai pas déjà dans le channel, l'ajoute
            std::map<std::string, Channel>::iterator chan = _server->_channels.find(*it);
            if (chan->second._clients.find(_client_id) == chan->second._clients.end())
                chan->second._clients.insert(std::pair<int, Client *>(_client_id, this));
        }


        
        // for(size_t i = 0; i < _server->_channels.find(_parsed_cmd[1])->second->_client_in_chan.size(); i++)
        // {
        //     send(_server->_channels.find(_parsed_cmd[1])->second->_client_in_chan[i]->_client_id, JOIN_CHAN(_nick, _parsed_cmd[1]).c_str(), JOIN_CHAN(_nick, _parsed_cmd[1]).size(), 0);
        // }
    }
}