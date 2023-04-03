#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "utils/ft_irc.hpp"

class Client
{
	// =============================================================================
	// ATTRIBUTS ===================================================================
private:

	std::string					_nick;
	std::string					_user;
	int							_client_id;
	std::vector<std::string>	_data_connexion;

public:
	// =============================================================================
	// CONSTRUCTORS ================================================================
    Client();
	Client(int client_id);

	// =============================================================================
	// DESTRUCTORS =================================================================
    ~Client();

	// =============================================================================
	// GETTERS / SETTERS ===========================================================
	std::vector<std::string>	getDataConnexion();


    // =============================================================================
	// METHODS =====================================================================
	void	parse_connexion(std::string buff, std::string password, std::map<int, Client>  &client);
	bool	check_connexion(std::string password);
	bool	client_save(std::string password);

private:
	bool	_check_nick( std::map<int, Client> &);
	bool	_check_user();


};


// =============================================================================
// NON-MEMBER OVERLOADS ========================================================


#endif
