#ifndef CHANNEL_COMMANDS_HPP
# define CHANNEL_COMMANDS_HPP

# include "ft_irc.hpp"

void
join(bool connected, std::vector<std::string> parsed_cmds, int client_id, std::string nickname);

#endif