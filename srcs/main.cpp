#include "../headers/server.hpp"
#include "../headers/client.hpp"
#include "../headers/ft_irc.hpp"


int main (int ac, char **av)
{
    (void) ac;
    //TODO secure  Servers are uniquely identified by their name, which has a maximum length of sixty three (63) characters
    Server a(av);

    // a._get_listener_socket();

    return (0);
}

/*
nc -C 6667 123
CAP LS
PASS 123
NICK l
USER li li localhost :li
*/