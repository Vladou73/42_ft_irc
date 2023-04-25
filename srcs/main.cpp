#include "../headers/server.hpp"
#include "../headers/client.hpp"
#include "../headers/ft_irc.hpp"


int main (int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "Please run the executable as follow : /ircserv <port> <password>" << std::endl;
        return (2);
    }

    std::string port(av[1]);
    std::string pwd(av[2]);
    
    for (size_t i = 0; i < port.size(); i++)
    {
        if (!std::isdigit(port[i]))
        {
            std::cout << "Please use a valid port number : /ircserv <port> <password>" << std::endl;
            return (2);
        }
    }
    Server a(av);

    return (0);
}

/*
nc -C 6667 123
CAP LS
PASS 123
NICK l
USER li li localhost :li
*/