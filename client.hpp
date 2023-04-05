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
	std::string					_client_id_str;
	int							_client_id;
	std::vector<std::string>	_data_connexion;
	std::string					_buff;

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
	std::string 				getUser();
	std::string 				getBuff();
	void		 				setBuff(std::string ss1);
	void						clearBuff();

    // =============================================================================
	// METHODS =====================================================================
	void	parse_connexion(std::string buff, std::string password, std::map<int, Client>  &client, int &count_clients);
	void	parse_irssi(std::string big_buff, std::string password, std::map<int, Client> &client, int &count_clients);
	bool	check_connexion(std::string password);
	bool	client_save(std::string password);

private:
	bool	_check_nick( std::map<int, Client> &);
	bool	_check_user();


};


// =============================================================================
// NON-MEMBER OVERLOADS ========================================================


#endif
