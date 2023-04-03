# include "client.hpp"

// =============================================================================
// CONSTRUCTORS ================================================================
Client::Client()
{}

Client::Client(int client_id) : _client_id(client_id)
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


// =============================================================================
// METHODS =====================================================================

bool	Client::_check_nick(std::map<int, Client> &client)
{
	//TODO verifier quoi checker sur les nicknames
	//TODO verifier quoi renvoyer en cas de probleme sur les nicknames
	// Nick is max len of 9char or truncated, if client already exist it add an _
	// nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
	// std::cout << "check nick\n";
	std::string mess = ":" + _data_connexion[1] + "!@localhost NICK " + _data_connexion[1] + "\n";
	std::map<int, Client>::iterator end = client.end();
	for (std::map<int, Client>::iterator it = client.begin(); it != end; it++)
	{

		std::cout << "it->second = "<< it->second._nick << std::endl;
		if (it->second._nick == _data_connexion[1])
		{
			std::cout << "client fd = " << _client_id << std::endl;
			// std::string nick_taken = ":localhost 433 " + _data_connexion[1] + " " + _data_connexion[1] + " :Nickname is already in use.\n";
			// send (_client_id, nick_taken.c_str(), nick_taken.size(), 0);
			// send (_client_id, mess.c_str(), mess.size(), 0);
			return (false);
		}
	}
	send (_client_id, mess.c_str(), mess.size(), 0);
	return true;
}

bool	Client::_check_user()
{
	//TODO verifier quoi checker sur les users
	//TODO verifier quoi renvoyer en cas de probleme sur les users
	// 4 informations
	// user  =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF ); any octet except NUL, CR, LF, " " and "@"
	return true;
}

bool
Client::check_connexion(std::string password, std::map<int, Client> &client)
{
	(void)client;
	// std::cout << "check connexion\n";
	//check password
	std::string	pwd = password;
	if (_data_connexion[0] != pwd)
	{
		_data_connexion.clear();
		return false;
	}
	// else if (_check_nick(client) == false)
	// {
	// 	return false;
	// }
	else if (_check_user() == false)
	{
		return false;
	}
	return true;
}

bool
Client::client_save(std::string password, std::map<int, Client> &client)
{
	// std::cout << "client save\n";
	if (check_connexion(password, client) == true)
	{
		_nick = _data_connexion[1];
		_user = _data_connexion[2];

		return (true);
	}
	return (false);
}


void
Client::parse_connexion(std::string buff, std::string password, std::map<int, Client> &client)
{
	buff.erase(buff.end() - 1);
	if (buff.size() < 6)
		_data_connexion.clear();
	else if (buff.compare(0, 5, "PASS ") == 0)
	{
		_data_connexion.clear();
		_data_connexion.push_back(buff.substr(5, std::string::npos));
	}
	else if (buff.compare(0, 5, "NICK ") == 0)
	{
		if (_data_connexion.size() == 1)
		{
			_data_connexion.push_back(buff.substr(5, std::string::npos));
			if ( _check_nick(client) == false)
				_data_connexion.pop_back();	
		}
		else
			_data_connexion.clear();
	}
	else if (buff.compare(0, 5, "USER ") == 0)
	{
		if (_data_connexion.size() == 2)
		{
			_data_connexion.push_back(buff.substr(5, std::string::npos));
			client_save(password, client);

		}
		else
			_data_connexion.clear();
	}
	else
		_data_connexion.clear();
}
