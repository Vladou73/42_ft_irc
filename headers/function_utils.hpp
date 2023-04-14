#ifndef FUNCTION_UTILS_HPP
#define FUNCTION_UTILS_HPP

# include <string>
# include <iostream>
# include <sstream>
# include "ft_irc.hpp"

std::string                 change_to_str(int a);
std::vector<std::string>    parse_commas(std::string buff);
bool						check_channel_name(std::string name);

#endif
