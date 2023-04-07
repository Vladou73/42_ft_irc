#ifndef CLIENT_COMMANDS_HPP
# define CLIENT_COMMANDS_HPP

# include "ft_irc.hpp"
# include "../client.hpp"
# include "../server.hpp"


void
ping(bool connected, std::vector<std::string> , int client_id, std::string nickname);

void
pass(std::vector<std::string> &data_connexion, bool connected, std::string password, std::vector<std::string> parsed_cmds, int client_id, std::string nickname);

void
nick(std::vector<std::string> &data_connexion, bool connected, std::vector<std::string> parsed_cmds, int client_id, std::string &nickname, std::map<int, Client> &clients);

bool	check_nick(std::map<int, Client> &clients, int client_id, std::string &nickname, std::string new_nickname);

#endif