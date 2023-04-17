#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.hpp"
# include "client.hpp"
# include "channel.hpp"

class Client;
class Channel;

class Server
{
public:
    friend class Client;
    friend class Channel;
	// =============================================================================
	// ATTRIBUTS ===================================================================
private:
    const char *				_pwd;
	int							_listener;
	const char *				_server_port;
    std::vector<struct pollfd>	_pfds;
    
protected:
	std::map<int, Client>	                _clients;
    int                                     _count_clients;
    std::map<std::string, Channel>          _channels;
    static Server*                          _instance;

public:
	// =============================================================================
	// CONSTRUCTORS ================================================================
    Server();
    Server(char **av);


	// =============================================================================
	// DESTRUCTORS =================================================================
    ~Server();

	// =============================================================================
	// MODIFIERS ===================================================================

    static void     signalHandler(int signum);
    static void     ctrl_C();

    // =============================================================================
	// METHODS =====================================================================
    // Return a listening socket
private:
    void    _get_listener_socket(void);
    void    _poll_loop(void);
    void    _add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size);
    void    _del_from_pfds(struct pollfd pfds[], int i, int *fd_count);
    void    _add_new_client(std::vector<struct pollfd> &);
    void    _handle_data(std::vector<struct pollfd>::iterator &it);
    void    _rm_deconnected_clients();

};


// =============================================================================
// NON-MEMBER OVERLOADS ========================================================


#endif


/*
int listener

struct addrinfo hints :

ai_family = AF_UNSPEC
ai_family: This parameter specifies the desired address family. It can be set to AF_INET for IPv4, AF_INET6 for IPv6, or AF_UNSPEC for either IPv4 or IPv6.

ai_socktype = SOCK_STREAM
ai_socktype: This parameter specifies the desired socket type. It can be set to SOCK_STREAM for a TCP socket or SOCK_DGRAM for a UDP socket.

ai_flags = AI_PASSIVE
ai_flags: This parameter specifies additional flags that control the behavior of getaddrinfo(). One common flag is AI_PASSIVE, which indicates that the resulting address structure should be suitable for use in a call to bind() to bind the socket to a local address.

*/
