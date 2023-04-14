# include "../headers/server.hpp"

// =============================================================================
// CONSTRUCTORS ================================================================
Server::Server() : _server_port("6667")
{}

Server::Server(char **av) : _pwd(av[2]), _listener(0),
    _server_port(av[1]), _pfds(0),
    _clients(), _count_clients(0),
    _channels()
{
    _get_listener_socket();
	_poll_loop();
}


// =============================================================================
// DESTRUCTORS =================================================================
Server::~Server()
{

}

// =============================================================================
// MODIFIERS ===================================================================


// =============================================================================
// METHODS =====================================================================
void
Server::_handle_data(std::vector<struct pollfd>::iterator &it)
{
    //A single message is a string of characters with a maximum length of 512 characters. The end of the string is denoted by a CR-LF (Carriage Return - Line Feed) pair (i.e., “\r\n”). There is no null terminator. The 512 character limit includes this delimiter, meaning that a message only has space for 510 useful characters.
	char    buff[100000]; //TODO CHECK LE BUFFER SIZE

	memset(&buff, 0, sizeof(buff));
    // If not the listener, we're just a regular client
    int nbytes = recv(it->fd, buff, sizeof(buff), 0);
    int sender_fd = it->fd;

    if (nbytes <= 0)
    {
        if (nbytes == 0) // Got connection closed by client. Ici gestion du ctrl+C
        {
            std::cout << "pollServer: socket #" << sender_fd << " hung up" << std::endl; // Connection closed
            if (!_clients[sender_fd].getUser().empty())
                _count_clients--;
           std::cout << GREEN << "[server] " << "clients connected = " << _count_clients << std::endl;
        } else //Got error //TODO : verifier si on doit egalement close le fd et supprimer le client. verifer erno (EWOULDBLOCK)
            perror("recv");
        _clients[sender_fd].setQuitMsg("abrupt client aborting");
        send(sender_fd, ERROR(_clients[sender_fd].getQuitMsg()).c_str(), ERROR(_clients[sender_fd].getQuitMsg()).size(), 0);
        _clients[sender_fd].delete_client();
        close(it->fd);
    }
    else
    {
        //DEBUG : visualiser les channels existants
        std::cout << std::endl << "*****CHANNELS***** " << std::endl;
        for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
        {
            std::cout << std::endl << "channel " << it->first << std::endl;
            std::map<int, Client *> clients = it->second.getClients();
            for (std::map<int, Client *>::iterator client = clients.begin(); client != clients.end(); client++)
            {
                std::cout << "client " << client->second->getNick() << std::endl;
            }
        }

        std::cout   << PURPLE << "[client] " << "fd = " << it->fd << " | "
                    << std::string(buff, 0, nbytes) << RESET << std::endl ;
		std::string ss1 = buff;
        _clients[sender_fd].setBuff(ss1);
        if (_clients[sender_fd].getBuff() == "\n")
        {
            _clients[sender_fd].clearBuff();
            return ;
        }
        if (*(_clients[sender_fd].getBuff().end() - 1) == '\n') //condition pour ctrl+D
        {
            _clients[sender_fd].parse_lines(_clients[sender_fd].getBuff());
            if (_clients[sender_fd].getSocketConnexion() == true)
                _clients[sender_fd].clearBuff();
        }
    }
}

void
Server::_add_new_client(std::vector<struct pollfd> &new_pollfds)
{
    int             reuseaddr = 1;
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
        // En cas de crash, permet d'avoir un nouveau socket sur le meme port
        setsockopt(newfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int));
        struct pollfd new_poll_fd;
        new_poll_fd.fd = newfd; // the socket descriptor
        new_poll_fd.events = POLLIN;
        new_pollfds.push_back(new_poll_fd);

        Client client(newfd, this);
		_clients.insert(std::pair<int,Client>(newfd, client));
    }
}

void
Server::_poll_loop(void)
{
    struct pollfd first;

    first.fd = _listener; // the socket descriptor
    first.events = POLLIN; // Report ready to read on incoming connection
    _pfds.push_back(first);

    // Redirect standard input to the socket file descriptor
    if (dup2(_listener, STDIN_FILENO) == -1) {
        std::cout<<"Dup2 fail!\n";
        // Handle error
    }
    for(;;)
    {
        std::vector<pollfd> new_pollfds; // tmp struct hosting potential newly-created fds

		std::cout << GREEN <<  "[server] is listening in fd = "<< _listener << RESET << std::endl;
        int poll_count = poll((pollfd *)&_pfds[0], _pfds.size(), -1);
        if (poll_count == -1)
        {
            perror("poll");
            exit(1);
        }
        std::vector<struct pollfd>::iterator end = _pfds.end();
        for (std::vector<struct pollfd>::iterator it = _pfds.begin(); it != end; it++)
        {
            std::string input;
            std::getline(std::cin, input);

            if (it->revents & POLLIN) // Check if someone's ready to read
            {
                if (it->fd == _listener)
                    _add_new_client(new_pollfds);
                else
                    _handle_data(it);
            }
            else if (it->revents & POLLHUP) // POLLHUP :Déconnexion (uniquement en sortie).
            {
                std::cout << "Client at socket #" << it->fd << " disconnected." << std::endl;
                _clients[it->fd].socketDisconnect();
                _clients[it->fd].delete_client();
            }
            // POLLERR :Condition d'erreur (uniquement en sortie).
            // POLLNVAL :Requête invalide : fd nest pas ouvert (uniquement en sortie)
            else if (it->revents & POLLERR || it->revents & POLLNVAL)
            {
                std::cout << "Invalid event on socket #" << it->fd << "." << std::endl;
                _clients[it->fd].socketDisconnect();
                _clients[it->fd].delete_client();
            }
        }
        _pfds.insert(_pfds.end(), new_pollfds.begin(), new_pollfds.end()); // Add the range of NEW_pollfds in poll_fds (helps recalculating poll_fds.end() in the for loop)
    }
}


void
Server::_get_listener_socket(void)
{
    int             reuseaddr = 1;
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
        setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int));

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
