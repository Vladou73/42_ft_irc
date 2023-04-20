#ifndef FUNCTION_UTILS_HPP
#define FUNCTION_UTILS_HPP

# include <string>
# include <iostream>
# include <sstream>
# include "ft_irc.hpp"
# include "client.hpp"

// class Server;
// class Channel;
class Client;

std::string                 change_to_str(int a);
std::vector<std::string>    parse_commas(std::string buff);
bool						check_channel_name(std::string name);
bool                        check_on_chan(std::vector<std::string> canals, std::string arg);
int                         search_for_client_by_nick(std::string nick, std::map<int, Client> clients);
bool                        client_is_chann_oper(int client_id, std::vector<int> channel_operators);

#endif