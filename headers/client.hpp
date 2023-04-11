#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ft_irc.hpp"
# include "server.hpp"

class Server;

class Client
{
    friend class Channel;

	// =============================================================================
	// ATTRIBUTS ===================================================================
private:
	std::string					_nick;
	std::string					_user;
	std::string					_client_id_str;
	int							_client_id;
	std::vector<std::string>	_data_connexion;
	std::string					_buff;
	std::vector<std::string>	_parsed_cmd;
	bool						_connected;
	Server						*_server;					

public:
	// =============================================================================
	// CONSTRUCTORS ================================================================
    Client();
	Client(int client_id, Server *server);

	// =============================================================================
	// DESTRUCTORS =================================================================
    ~Client();

	// =============================================================================
	// GETTERS / SETTERS ===========================================================
	std::vector<std::string>	getDataConnexion();
	std::string 				getUser();
	std::string 				getNick();
	std::string 				getBuff();
	void		 				setBuff(std::string ss1);
	void						clearBuff();

    // =============================================================================
	// METHODS =====================================================================
	void	parse_connexion(std::string buff, std::string password, std::map<int, Client>  &clients, int &count_clients);
	void	parse_irssi(std::string big_buff, std::string password, std::map<int, Client> &clients, int &count_clients);
	bool	check_connexion(std::string password);
	bool	client_save(std::string password);
	void	parse_command(std::string buff);
	void	search_command();


    // =============================================================================
	// COMMANDS ====================================================================
	void 	ping();
	void 	pass();
	void 	nick();
	bool 	check_nick();
	void	user();
	void	privmsg();
	void 	join();



private:
	bool	_check_user();


};


// =============================================================================
// NON-MEMBER OVERLOADS ========================================================


#endif
