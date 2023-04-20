#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"
# include "client.hpp"
# include "server.hpp"

class Client;
class Server;

class Channel
{
    friend class Client;

    // =============================================================================
	// ATTRIBUTS ===================================================================
private:
    std::string				_name;
	std::string				_topic;
	std::map<int, Client *>	_clients;
	std::map<int, bool>		_first_connexion;
	std::vector<int>		_id_operator;


public:
	// =============================================================================
	// CONSTRUCTORS ================================================================
    Channel();
    Channel(std::string name);

    // =============================================================================
	// DESTRUCTORS =================================================================
    ~Channel();


    // =============================================================================
	// GETTERS / SETTERS ===========================================================
	// void	setChan(int fd);
	std::map<int, Client *> getClients();

    // =============================================================================
	// METHODS =====================================================================
	// void	delUser(int fd);

};

#endif