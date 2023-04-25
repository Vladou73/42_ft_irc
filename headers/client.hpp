#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ft_irc.hpp"
# include "server.hpp"

class Server;
class Channel;

class Client
{
    friend class Channel;

	// =============================================================================
	// ATTRIBUTS ===================================================================
private:
	std::string							_nick;
	std::string							_user;
	std::string							_user_infos;
	std::string							_client_id_str;
	int									_client_id;
	std::vector<std::string>			_data_connexion;
	std::string							_buff;
	std::vector<std::string>			_parsed_cmd;
	bool								_connected;
	Server								*_server;
	bool								_socket_connected;
	bool								_operator;
	std::vector<std::string>			_canals;
	std::string							_quit_msg;
	bool								_is_server_oper;
	std::map<std::string, std::string>	_modes;
	std::string							_msg_buffer;

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
	bool						getSocketConnexion();
	void						socketDisconnect();
	void						setQuitMsg(std::string);
	std::string					getQuitMsg();
	std::string					getMsgBuffer();
	void						clearMsgBuffer();
	void						setMsgBuffer(std::string);

    // =============================================================================
	// METHODS =====================================================================
	void	parse_connexion(std::string buff, std::string password, std::map<int, Client>  &clients, int &count_clients);
	void	parse_irssi(std::string big_buff, std::string password, std::map<int, Client> &clients, int &count_clients);
	bool	check_connexion(std::string password);
	bool	client_save(std::string password);
	void	parse_command(std::string buff);
	void	parse_lines(std::string buff);
	void	search_command();
	void	clear_client();
	void	delete_client();
	void	delete_client_from_chans(std::string);
	void	channel_mode();
	void	user_mode();


    // =============================================================================
	// COMMANDS ====================================================================
	void 	ping();
	void 	pass();
	void 	nick();
	bool 	check_nick();
	void	user();
	void	privmsg();
	void	quit(Client &client, std::string msg = "");
	void 	join();
	void	part();
	void 	topic();
	void	list();
	void	names();
	void	oper();
	void	kill();
	void	mode();
	void	kick();
	void	invite();
	void	notice();
	void	msg();
	

private:
	void										_check_topic_len();
	bool										_forbiden_nick();
	bool										_check_user();
	std::map<int, Client>::iterator				_nick_exist();
	std::map<std::string, Channel>::iterator	_channel_exist();


};


// =============================================================================
// NON-MEMBER OVERLOADS ========================================================


#endif
