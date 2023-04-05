# include "client.hpp"

// =============================================================================
// CONSTRUCTORS ================================================================
Client::Client()
{}

Client::Client(int client_id) : _nick(), _user(),
		_client_id_str(change_to_str(client_id)),
		_client_id(client_id), _data_connexion(0), _buff()
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


// =============================================================================
// METHODS =====================================================================
bool	Client::_check_nick(std::map<int, Client> &client)
{
	//TODO verifier quoi checker sur les nicknames
	//TODO verifier quoi renvoyer en cas de probleme sur les nicknames
	// nickname   =  ( letter / special ) *8( letter / digit / special / "-" )

	//TODO verifier avec irssi le nombre max de caracteres
	// if(_data_connexion[1].size() >= 9)
	// {
	// 	send (_client_id, ERR_ERRONEUSNICKNAME(_data_connexion[1],_data_connexion[1]).c_str(), strlen(ERR_ERRONEUSNICKNAME(_data_connexion[1],_data_connexion[1]).c_str()), 0);
	// 	return false;
	// }
	std::map<int, Client>::iterator end = client.end();
	for (std::map<int, Client>::iterator it = client.begin(); it != end; it++)
	{
		if (it->second._nick == _data_connexion[1])
		{
			send (_client_id, ERR_NICKNAMEINUSE(_data_connexion[1],_data_connexion[1]).c_str(), strlen(ERR_NICKNAMEINUSE(_data_connexion[1],_data_connexion[1]).c_str()), 0);
			send (_client_id, USER_ID(_data_connexion[1], _data_connexion[1]).c_str(), strlen(USER_ID(_data_connexion[1], _data_connexion[1]).c_str()), 0);
			return (false);
		}
	}
	_nick = _data_connexion[1];
	std::cout << "_nick="  << _nick << std::endl;
	send (_client_id, USER_ID(_data_connexion[1], _data_connexion[1]).c_str(), strlen(USER_ID(_data_connexion[1], _data_connexion[1]).c_str()), 0);
	return true;
}

bool	Client::_check_user()
{
	//TODO verifier quoi checker sur les users
	//TODO verifier quoi renvoyer en cas de probleme sur les users
	// 4 informations
	// user  =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF ); any octet except NUL, CR, LF, " " and "@"
	std::string temp;
	std::stringstream X(_data_connexion[2]);
	int i = 0;

	while(getline(X, temp, ' '))
		if (!temp.empty())
			i++;
	if (i < 4)
	{
		send(_client_id, ERR_NEEDMOREPARAMS(_data_connexion[1], "USER").c_str(), ERR_NEEDMOREPARAMS(_data_connexion[1], "USER").size(), 0);

		return false;
	}
	return true;
}

bool
Client::check_connexion(std::string password)
{
	std::string	pwd = password;

	if (_data_connexion[0] != pwd)
	{
		_data_connexion.clear();
		return false;
	}
	else if (_check_user() == false)
	{
		_data_connexion.pop_back();
		return false;
	}
	_user = _data_connexion[2];
	std::cout << "_user=" << _user << std::endl;
	send (_client_id, RPL_WELCOME(_client_id_str, _nick).c_str(), strlen(RPL_WELCOME(_client_id_str, _nick).c_str()), 0);
	send (_client_id, WELCOME_ART, strlen(WELCOME_ART), 0);
	return true;
}

void
Client::parse_irssi(std::string big_buff, std::string password, std::map<int, Client> &client, int &count_clients)
{

	std::string buff;
	std::stringstream strstream(big_buff);


	while(getline(strstream, buff, '\n')){
		if (*(buff.end() - 1) == '\r')
			buff.erase(buff.end() - 1);
		// std::cout << buff << std::endl;
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
				if (check_connexion(password) == true) {
					count_clients++;
					std::cout << GREEN << "[server] clients connected = " << count_clients << std::endl;
				}
			}
			else
				_data_connexion.clear();
		}
		else
			_data_connexion.clear();
	}
}


void
Client::parse_connexion(std::string buff, std::string password, std::map<int, Client> &client, int &count_clients)
{
	// buff.erase(buff.end() - 1);

	parse_irssi(buff, password, client, count_clients);
}
