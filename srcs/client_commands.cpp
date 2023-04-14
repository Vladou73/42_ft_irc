# include "../headers/client_commands.hpp"
# include "../headers/client.hpp"


//The PING command is sent by either clients or servers to check the other side of the connection is still connected and/or to check for connection latency, at the application layer.
void
Client::ping()
{
    if (_parsed_cmd.size() == 1)
        send(_client_id, ERR_NEEDMOREPARAMS(_nick, "PING").c_str(), ERR_NEEDMOREPARAMS(_nick, "PING").size(), 0);
    else if (_connected == true)
        send(_client_id, RPL_PONG(_parsed_cmd[1]).c_str(), RPL_PONG(_parsed_cmd[1]).size(), 0);
}

void
Client::pass()
{
    if (_parsed_cmd.size() == 1)
	{
        send(_client_id, ERR_NEEDMOREPARAMS(_nick, "PASS").c_str(), ERR_NEEDMOREPARAMS(_nick, "PASS").size(), 0);
		return;
	}
    if (_connected == true)
    {
        send(_client_id, ERR_ALREADYREGISTERED(_nick).c_str(), ERR_ALREADYREGISTERED(_nick).size(), 0);
        return ;
    }
    if (_parsed_cmd[1] != _server->_pwd)
	{
		_data_connexion.clear();
        send(_client_id, ERR_PASSWDMISMATCH, strlen(ERR_PASSWDMISMATCH), 0);
		return ;
	}
    if (_data_connexion.size() > 0)
        return ;
    // _data_connexion.clear();
    _data_connexion.push_back(_parsed_cmd[1]);
}

bool
Client::check_nick()
{
	//TODO verifier quoi checker sur les nicknames
	//TODO verifier quoi renvoyer en cas de probleme sur les nicknames
	// nickname   =  ( letter / special ) *8( letter / digit / special / "-" )

	//TODO verifier avec irssi le nombre max de caracteres
	// if(_data_connexion[1].size() >= 9)
	// {
	// 	send (client_id, ERR_ERRONEUSNICKNAME(_data_connexion[1],_data_connexion[1]).c_str(), strlen(ERR_ERRONEUSNICKNAME(_data_connexion[1],_data_connexion[1]).c_str()), 0);
	// 	return false;
	// }

	std::map<int, Client>::iterator end = _server->_clients.end();
	for (std::map<int, Client>::iterator it = _server->_clients.begin(); it != end; it++)
	{
		if (it->second.getNick() == _data_connexion[1])
		{
			send (_client_id, ERR_NICKNAMEINUSE(_data_connexion[1]).c_str(), strlen(ERR_NICKNAMEINUSE(_data_connexion[1]).c_str()), 0);
			send (_client_id, USER_ID(_data_connexion[1]).c_str(), strlen(USER_ID(_data_connexion[1]).c_str()), 0);
			return (false);
		}
	}
	if (_connected == false)
		_nick = _data_connexion[1];
	else
		_nick = _parsed_cmd[1];
	send (_client_id, USER_ID(_data_connexion[1]).c_str(), strlen(USER_ID(_data_connexion[1]).c_str()), 0);
	return true;
}

void
Client::nick()
{
	if (_parsed_cmd.size() == 1)
	{
		send (_client_id, ERR_NONICKNAMEGIVEN, strlen(ERR_NONICKNAMEGIVEN), 0);
		return;
	}
	if (_connected == true)
		check_nick();
    else if (_data_connexion.size() == 1)
    {
        _data_connexion.push_back(_parsed_cmd[1]);
        if (check_nick() == false)
    		_data_connexion.pop_back();
    }
}

void
Client::user()
{
	if (_connected == true)
		return;

	//TODO verifier quoi checker sur les users
	//TODO verifier quoi renvoyer en cas de probleme sur les users
	// 4 informations
	// user  =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF ); any octet except NUL, CR, LF, " " and "@"
	if (_data_connexion.size() == 2)
	{
		if (_parsed_cmd.size() < 5)
		{
			send(_client_id, ERR_NEEDMOREPARAMS(_data_connexion[1], "USER").c_str(), ERR_NEEDMOREPARAMS(_data_connexion[1], "USER").size(), 0);
			return ;
		}

		std::string	append;
		for (size_t i = 1; i < _parsed_cmd.size(); i++)
			append += _parsed_cmd[i];
		_data_connexion.push_back(append);

		_user = _data_connexion[2];
		//? Je pense que c'est le user est le dernier arg de user
		//? _user = _parsed_cmd[4];

		send (_client_id, RPL_WELCOME(_client_id_str, _nick).c_str(), strlen(RPL_WELCOME(_client_id_str, _nick).c_str()), 0);
		send(_client_id, RPL_YOURHOST(_nick).c_str(), strlen(RPL_YOURHOST(_nick).c_str()), 0);
		// TODO : Date time (devra etre decommente)
		// send(_client_id, RPL_CREATED(_nick, datetime).c_str(), strlen(RPL_CREATED(_nick, datetime).c_str()), 0);
		// TODO : check user_mode/chan_mode/Chan_parm_mode (exemples in the messages file) (devra etre decommente)
		// send(_client_id, RPL_MYINFO(_nick, user_modes, chan_modes, chan_param_modes).c_str(), strlen(RPL_MYINFO(_nick, user_modes, chan_modes, chan_param_modes).c_str()), 0);
		send(_client_id, RPL_ISUPPORT(_nick).c_str(), strlen(RPL_ISUPPORT(_nick).c_str()), 0);
		send (_client_id, WELCOME_ART, strlen(WELCOME_ART), 0);

		_connected = true;
		_server->_count_clients++;
		std::cout << GREEN << "[server] clients connected = " << _server->_count_clients << std::endl;
	}
	else
		_data_connexion.clear(); //TODO :doit on clear egalement _nick et _pass ?
}

void
Client::privmsg()
{
	std::string mess;
	if (_parsed_cmd.size() == 3)
	{
		//TODO need the channels map
		if (_parsed_cmd[1].at(0) == '#')
		{
			std::cout <<  "CHANEL MESSAGES \n";
		}
		mess = _parsed_cmd[2];
		std::map<int, Client>::iterator end = _server->_clients.end();
		for (std::map<int, Client>::iterator it = _server->_clients.begin(); it != end; it++)
		{
			std::cout << "nick = " << it->second.getNick() << std::endl;
			std::cout << "data_connexion[1] = " << _data_connexion[1] << std::endl;
			if (it->second.getNick() != _parsed_cmd[1])
				send(it->first, ERR_NOSUCHNICK(_user, _parsed_cmd[1]).c_str(), strlen(ERR_NOSUCHNICK(_user, _parsed_cmd[1]).c_str()), 0);
			send (it->first, RPL_PRIVMSG(_nick, _user, it->second.getNick(), mess).c_str(), strlen(RPL_PRIVMSG(_nick, _user, it->second.getNick(), mess).c_str()), 0);
		}
	}
}

void
Client::quit()
{
	//TODO : ajouter la suppression du client dans les channels
	std::string mess;
	if (_parsed_cmd.size() == 2)
		mess = _parsed_cmd[1];
	else
		mess = " ";
	send(_client_id, RPL_QUIT(USER_ID2(_nick), mess).c_str(), strlen(RPL_QUIT(USER_ID2(_nick), mess).c_str()), 0);
	if (!_server->_clients[_client_id]._user.empty())
        _server->_count_clients--;

	_socket_connected = false;
	close(_client_id);
}
