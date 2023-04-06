#ifndef CLIENT_COMMANDS_HPP
# define CLIENT_COMMANDS_HPP

# include "ft_irc.hpp"

void
ping(bool connected, std::vector<std::string> parsed_cmds, int client_id, std::string nickname);

#endif