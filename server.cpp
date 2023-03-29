# include "server.hpp"

// =============================================================================
// CONSTRUCTORS ================================================================
server::server() : _server_port("6667")
{}

server::server(char *av) : _server_port(av)
{
    _get_listener_socket();
	_poll_loop();
}


// =============================================================================
// DESTRUCTORS =================================================================
server::~server()
{}

// =============================================================================
// MODIFIERS ===================================================================


// =============================================================================
// METHODS =====================================================================
void
server::_get_listener_socket(void)
{
    int             yes = 1;
    int             rv;
    struct addrinfo hints, *ai, *p;

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
    for(p = ai; p != NULL; p = p->ai_next)
    {
        _listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (_listener < 0)
          continue;

        // En cas de crash, permet d'avoir un nouveau socket sur le meme port
        setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(_listener, p->ai_addr, p->ai_addrlen) < 0) 
        {
            close(_listener);
            continue;
        }
        break;
    }
    freeaddrinfo(ai);

    if (p == NULL || listen(_listener, SOMAXCONN) == -1)
    {
        std::cerr << "error getting listening socket\n";
        exit (1);
    }
}


// Add a new file descriptor to the set
void 
server::_add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
{
	// If we don't have room, add more space in the pfds array
	if (*fd_count == *fd_size) {
		*fd_size *= 2; // Double it
		// *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_size));
		// *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_size));
	}
	(*pfds)[*fd_count].fd = newfd;
	(*pfds)[*fd_count].events = POLLIN; // Check ready-to-read
	(*fd_count)++;
}

// Remove an index from the set
void 
server::_del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
{
	// Copy the one from the end over this one
	pfds[i] = pfds[*fd_count-1];

	(*fd_count)--;
}



/*
struct pollfd {
int fd; // the socket descriptor
short events; // bitmap of events we're interested in
short revents; // when poll() returns, bitmap of events that occurred
};
*/
void    
server::_poll_loop(void)
{
	//TODO find buff size
	char					buff[256];
    int						fd_count = 1;
    int						fd_size = 10;
    int						newfd; // Newly accept()ed socket descriptor
    socklen_t               addrlen;
    struct pollfd           *pfds = new pollfd[fd_size];
    struct sockaddr_storage remoteaddr; // Client address

    // Add the listener to set
    pfds[0].fd = _listener; // the socket descriptor
    pfds[0].events = POLLIN; // Report ready to read on incoming connection
    fd_count = 1; // For the liste  / Main loop

    for(;;) 
    {
		std::cout << "avant poll" << std::endl;
		std::cout << "socket fd listening"<< _listener << std::endl;
        // poll_count = nb sockets ouverts
        int poll_count = poll(pfds, fd_count, -1);
		
		std::cout << "apres poll" << std::endl;

        if (poll_count == -1) 
        {
            perror("poll");
            exit(1);
        }   
        // Run through the existing connections looking for data to read
        for(int i = 0; i < fd_count; i++) 
        { 
            // Check if someone's ready to read
            if (pfds[i].revents && POLLIN) 
            {   // We got one!! 
                if (pfds[i].fd == _listener) 
                {
                    // If listener is ready to read, handle new connection  
                    addrlen = sizeof(remoteaddr);
                    newfd = accept(_listener, (struct sockaddr *)&remoteaddr,&addrlen);  
					std::cout << newfd << std::endl;
                    if (newfd == -1)
                        perror("accept");
					else
                    {
						//TODO create function add to pdfs
                        _add_to_pfds(&pfds, newfd, &fd_count, &fd_size); 
                        std::cout << "pollserver: new connection from \n"; 
                    }
                } 
                else 
                {
                    // If not the listener, we're just a regular client
                    int nbytes = recv(pfds[i].fd, buff, sizeof(buff), 0);  
                    int sender_fd = pfds[i].fd; 
                    if (nbytes <= 0) 
                    {
                            // Got error or connection closed by client
                            if (nbytes == 0) 
                            {
                                // Connection closed
                                printf("pollserver: socket %d hung up\n", sender_fd);
                            } else
                                perror("recv"); 
                            close(pfds[i].fd); // Bye!  
							//TODO code del function
                            _del_from_pfds(pfds, i, &fd_count);  
                    } 
                    else 
                    {
						std::cout << std::string(buff, 0, nbytes) << std::endl;
                        // We got some good data from a client  
                        // for(int j = 0; j < fd_count; j++) 
                        // {
                        //     // Send to everyone!
                        //     int dest_fd = pfds[j].fd;
                        //     // Except the listener and ourselves
                        //     if (dest_fd != _listener && dest_fd != sender_fd) 
                        //     {
                        //         if (send(dest_fd, buff, nbytes, 0) == -1)
                        //             perror("send");
                        //     }
                        // }
                    }
                } // END handle data from client
            } // END got ready-to-read from poll()
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
}
