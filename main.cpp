#include "server.hpp"
#include "ft_irc.hpp"


int main (int ac, char **av)
{
    (void) ac;

    server a(av[1]);

    // a._get_listener_socket();

    return (0);
}