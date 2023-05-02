#ifndef FT_IRC_HPP
# define FT_IRC_HPP


# include <sys/socket.h>
# include <sys/types.h>
# include <netdb.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <poll.h>

# include <iostream>
# include <iterator>
# include <sstream>
# include <vector>
# include <map>
# include <iomanip>
# include <cstring>
# include <string>
# include <csignal>
# include <fstream>
# include <ctime>

# include "colors.hpp"
# include "messages.hpp"
# include "function_utils.hpp"

extern int int_sign; 

struct server_oper 
{
    std::string name;
    std::string pwd;
    std::string host;
};

#endif