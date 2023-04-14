# include "../headers/client.hpp"

// =============================================================================
// CONSTRUCTORS ================================================================
Client::Client()
{}

Client::Client(int client_id, Server *server) : _nick(), _user(),
		_client_id_str(change_to_str(client_id)),
		_client_id(client_id), _data_connexion(0),
		_buff(), _parsed_cmd(), _connected(false),
		_server(server), _socket_connected(true), _canals()
{}


// =============================================================================
// DESTRUCTORS =================================================================
Client::~Client()
{}


// =============================================================================
// GETTERS / SETTERS ===========================================================
std::vector<std::string>
Client::getDataConnexion()
{
    return _data_connexion;
}

std::string
Client::getUser()
{
    return _user;
}

std::string
Client::getNick()
{
    return _nick;
}

std::string
Client::getBuff()
{
    return _buff;
}

void
Client::setBuff(std::string ss1)
{
	_buff = _buff + ss1;
}

void
Client::clearBuff()
{
	_buff.clear();
}

void
Client::socketDisconnect()
{
    _socket_connected = false;
}

bool
Client::getSocketConnexion()
{
    return _socket_connected;
}

// =============================================================================
// METHODS =====================================================================

void
Client::search_command()
{
	// for (std::vector<std::string>::iterator it = _parsed_cmd.begin(); it != _parsed_cmd.end(); it++)
	// 	std::cout << *it << std::endl;

	if (_parsed_cmd[0] == "PASS")
		pass();
	else if (_parsed_cmd[0] == "NICK")
		nick();
	else if (_parsed_cmd[0] == "USER")
		user();
	else if (_parsed_cmd[0] == "PING")
		ping();
	else if (_parsed_cmd[0]== "JOIN")
		join();
	else if (_parsed_cmd[0]== "PRIVMSG")
		privmsg();
	else if (_parsed_cmd[0] == "QUIT")
		quit();
	else if (_parsed_cmd[0]== "PART")
	  	part();
	else if (_parsed_cmd[0]== "TOPIC")
	  topic();
	else if (_parsed_cmd[0]== "LIST")
	  list();
	else
		std::cout << "default\n";
}

void
Client::parse_lines(std::string buff)
{
	std::stringstream			strstream(buff);
	std::vector<std::string>	buff_lines;
	std::string 				line;

	while(getline(strstream, line, '\n'))
	{
		if (*(line.end() - 1) == '\n')
			line.erase(line.end() - 1);
		if (*(line.end() - 1) == '\r')
			line.erase(line.end() - 1);
		buff_lines.push_back(line);
	}

	for (std::vector<std::string>::iterator it = buff_lines.begin(); it != buff_lines.end(); it++)
	{
		// std::cout << "'" << *it << "'" << std::endl;
		parse_command(*it);
	}
}

void
Client::parse_command(std::string command)
{
	std::stringstream	strstream(command);
	std::string 		word;
	std::string			msg;
	bool				last_arg = false;

	if (!_parsed_cmd.empty())
		_parsed_cmd.clear();

	while(getline(strstream, word, ' '))
	{
		if (word[0] == ':')
		{
			last_arg = true;
			word.erase(word.begin());
		}
		if (last_arg == false) {
			if (word.length() > 0)
				_parsed_cmd.push_back(word);
		}
		else
			msg += word + " ";
	}
	if (!msg.empty() && msg.length() > 0)
		_parsed_cmd.push_back(msg);

	search_command();
}


// void
// Client::clear_client()
// {
// 	_nick.clear();
// 	_user.clear();
// 	_client_id_str.clear();
// 	_data_connexion.clear();
// 	// _buff.clear();
// 	_parsed_cmd.clear();
// 	_server = NULL;
// 	// delete this;
// }

void
Client::delete_client()
{
	std::cout << "delete client func\n";
	std::vector<struct pollfd>::iterator it = _server->_pfds.begin();
	std::vector<struct pollfd>::iterator end = _server->_pfds.end();
	for (; it != end; it++)
	{
		if (it->fd == _client_id)
		{
			std::cout << "it->fd" << it->fd << std::endl;
			_server->_pfds.erase(it);
		}
	}
	_server->_clients.erase(_client_id);
	// clear_client();
}
