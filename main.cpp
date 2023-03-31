#include "server.hpp"
#include "ft_irc.hpp"


int main (int ac, char **av)
{
    (void) ac;

    Server a(av[1]);

    // a._get_listener_socket();

    return (0);
}

/*
CAP LS
PASS 123
NICK l
USER li li localhost :li
*/