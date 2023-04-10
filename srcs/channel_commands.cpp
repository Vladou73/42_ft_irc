# include "../headers/channel_commands.hpp"
# include "../headers/client.hpp"
# include "../headers/channel.hpp"
# include "../headers/ft_irc.hpp"

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
        std::map<std::string, Channel *>::iterator end = _server->_channels.end();
        if (_server->_channels.find(_parsed_cmd[1]) == end)
        {
            Channel *channel = new Channel(_parsed_cmd[1]);
            _server->_channels.insert(std::pair<std::string, Channel *>(_parsed_cmd[1], channel));
        }
        std::map<std::string, Channel *>::iterator it = _server->_channels.find(_parsed_cmd[1]);
        it->second->_client_in_chan.push_back(this);
        for(size_t i = 0; i < _server->_channels.find(_parsed_cmd[1])->second->_client_in_chan.size(); i++)
        {
            send(_server->_channels.find(_parsed_cmd[1])->second->_client_in_chan[i]->_client_id, JOIN_CHAN(_nick, _parsed_cmd[1]).c_str(), JOIN_CHAN(_nick, _parsed_cmd[1]).size(), 0);
        }
    }
}