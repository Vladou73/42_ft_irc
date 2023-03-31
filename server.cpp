# include "server.hpp"

// =============================================================================
// CONSTRUCTORS ================================================================
Server::Server() : _server_port("6667")
{}

Server::Server(char *av) : _server_port(av)
{
    _get_listener_socket();
	_poll_loop();
}


// =============================================================================
// DESTRUCTORS =================================================================
Server::~Server()
{}

// =============================================================================
// MODIFIERS ===================================================================


// =============================================================================
// METHODS =====================================================================
void
Server::_get_listener_socket(void)
{
    int             yes = 1;
    int             rv;
    struct addrinfo hints, *ai, *temp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, _server_port, &hints, &ai)) != 0)
    {
        std::cerr << gai_strerror(rv);
        exit(1);
    }
    // Boucle pour trouver une adresse qui fonctionne avec l'ouverture du socket
    for(temp = ai; temp != NULL; temp = temp->ai_next)
    {
        // point d'ecoute dans le vide, pas encore connecter au Server
        _listener = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
        if (_listener < 0)
          continue;

        // En cas de crash, permet d'avoir un nouveau socket sur le meme port
        setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        // connexion entre le Server et lepoint d'ecoute (socket)
        if (bind(_listener, temp->ai_addr, temp->ai_addrlen) < 0) 
        {
            close(_listener);
            continue;
        }
        break;
    }
    freeaddrinfo(ai);
    // fd du socket et en mode ecoute de connexions rentrantes de clients
    if (temp == NULL || listen(_listener, SOMAXCONN) == -1)
    {
        std::cerr << "error getting listening socket\n";
        exit (1);
    }
}

void
Server::_add_new_client()
{
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t               addrlen;
    int						newfd; // Newly accept()ed socket descriptor

    // If listener is ready to read, handle new connection  
    addrlen = sizeof(remoteaddr);
    newfd = accept(_listener, (struct sockaddr *)&remoteaddr,&addrlen);
    if (newfd == -1)
        perror("accept");
    else
    {
        struct pollfd new_poll_fd;
        new_poll_fd.fd = newfd; // the socket descriptor
        new_poll_fd.events = POLLIN; 
        _pfds.push_back(new_poll_fd);
    }
}

void
Server::_parse_connexion(std::string buff)
{
	std::cout<<"buffer = "<< buff<<std::endl;
	// if (buff.compare(0,4, "PASS ") == 0)
	// {
		// _data_connexion.clear();
	_data_connexion.push_back(buff);
	// }
	// std::vector<std::string>::iterator it = _data_connexion.begin();
	// for (; it != _data_connexion.end(); it++)
	std::cout<<"PASS = "<<_data_connexion[0]<<std::endl;
}

void
Server::_handle_data(std::vector<struct pollfd>::iterator &it)
{
    //A single message is a string of characters with a maximum length of 512 characters. The end of the string is denoted by a CR-LF (Carriage Return - Line Feed) pair (i.e., “\r\n”). There is no null terminator. The 512 character limit includes this delimiter, meaning that a message only has space for 510 useful characters.
	char    buff[512];
	// std::stringstream ss;
    // If not the listener, we're just a regular client
    int nbytes = recv(it->fd, buff, sizeof(buff), 0);  
    int sender_fd = it->fd; 

    if (nbytes <= 0) 
    {
        // Got error or connection closed by client
        if (nbytes == 0) 
        {
            // Connection closed
            printf("pollServer: socket %d hung up\n", sender_fd);
        } else
            perror("recv"); 
        close(it->fd); // Bye!  
        _pfds.erase(it); 
    } 
    else 
    {
        std::cout << "[client msg] " << "fd = " << it->fd << " | ";
        std::cout << std::string(buff, 0, nbytes) << std::endl;
		// ss << buff;
		std::string ss1 = buff;
        _parse_connexion(ss1);
        // if (send(dest_fd, buff, nbytes, 0) == -1)
    }
}

void    
Server::_poll_loop(void)
{
    struct pollfd first;

    first.fd = _listener; // the socket descriptor
    first.events = POLLIN; // Report ready to read on incoming connection
    _pfds.push_back(first);

    for(;;) 
    {
		std::cout << "avant poll" << std::endl;
		std::cout << "socket fd listening"<< _listener << std::endl;
        int poll_count = poll((pollfd *)&_pfds[0], _pfds.size(), -1);
		
		std::cout << "apres poll" << std::endl;

        if (poll_count == -1) 
        {
            perror("poll");
            exit(1);
        }   
        std::vector<struct pollfd>::iterator end = _pfds.end();
        for (std::vector<struct pollfd>::iterator it = _pfds.begin(); it != end; it++)
        { 
            // Check if someone's ready to read
            if (it->revents & POLLIN)
            {
                if (it->fd == _listener) 
                    _add_new_client();
                else 
                    _handle_data(it);
            }
        }
    }
}
