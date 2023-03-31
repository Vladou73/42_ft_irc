# include "client.hpp"

// =============================================================================
// CONSTRUCTORS ================================================================
Client::Client()
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
void
Client::parse_connexion(std::string buff)
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
			_data_connexion.push_back(buff.substr(5, std::string::npos));
		else
			_data_connexion.clear();
	}
	else if (buff.compare(0, 5, "USER ") == 0)
	{
		if (_data_connexion.size() == 2)
			_data_connexion.push_back(buff.substr(5, std::string::npos));
		else
			_data_connexion.clear();
	}
	else
		_data_connexion.clear();
	std::cout << "size= "<< _data_connexion.size() << std::endl;
	std::vector<std::string>::iterator it = _data_connexion.begin();
	std::vector<std::string>::iterator end = _data_connexion.end();
	for (; it != end; it++)
		std::cout << *it <<std::endl;
}

bool	Client::_check_nick()
{
	//TODO verifier quoi checker sur les nicknames
	//TODO verifier quoi renvoyer en cas de probleme sur les nicknames
	return true;
}

bool	Client::_check_user()
{
	//TODO verifier quoi checker sur les users
	//TODO verifier quoi renvoyer en cas de probleme sur les users
	return true;
}

bool
Client::check_connexion(std::string password)
{
	//check password
	std::string	pwd = password;
	if (_data_connexion[0] != pwd)
	{
		_data_connexion.clear();
		return false;
	}
	else if (_check_nick() == false)
	{
		return false;
	}
	else if (_check_user() == false)
	{
		return false;
	}
	return true;
}
