# include "../headers/client.hpp"


//The PING command is sent by either clients or servers to check the other side of the connection is still connected and/or to check for connection latency, at the application layer.
void
Client::ping()
{
    if (_parsed_cmd.size() == 1)
        _msg_buffer += ERR_NEEDMOREPARAMS(_nick, "PING");
    else if (_connected == true)
        _msg_buffer += RPL_PONG(_parsed_cmd[1]);
}

void
Client::pass()
{
    if (_parsed_cmd.size() == 1)
	{
        _msg_buffer += ERR_NEEDMOREPARAMS(_nick, "PASS");
		return;
	}
    if (_connected == true)
    {
        _msg_buffer += ERR_ALREADYREGISTERED(_nick);
        return ;
    }
    if (_parsed_cmd[1] != _server->_pwd)
	{
		_data_connexion.clear();
        _msg_buffer += ERR_PASSWDMISMATCH;
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
	// 	_msg_buffer +=  ERR_ERRONEUSNICKNAME(_data_connexion[1],_data_connexion[1]).c_str(), strlen(ERR_ERRONEUSNICKNAME(_data_connexion[1],_data_connexion[1]).c_str()), 0);
	// 	return false;
	// }

	std::map<int, Client>::iterator end = _server->_clients.end();
	for (std::map<int, Client>::iterator it = _server->_clients.begin(); it != end; it++)
	{
		if (it->second.getNick() == _data_connexion[1])
		{
			_msg_buffer += ERR_NICKNAMEINUSE(_data_connexion[1]);
			_msg_buffer += USER_ID(_data_connexion[1], _user);
			return (false);
		}
	}
	if (_connected == false)
		_nick = _data_connexion[1];
	else
		_nick = _parsed_cmd[1];
	_msg_buffer += USER_ID(_data_connexion[1], _user);
	return true;
}

// 	https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.2
// 	NICK command is used to give user a nickname or change the existing
//    one.

// Numeric Replies:

//            ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
//            ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
//            ERR_UNAVAILRESOURCE             ERR_RESTRICTED

//    Examples:

//    NICK Wiz                ; Introducing new nick "Wiz" if session is
//                            still unregistered, or user changing his
//                            nickname to "Wiz"

//    :WiZ!jto@tolsun.oulu.fi NICK Kilroy
//                            ; Server telling that WiZ changed his
//                            nickname to Kilroy.


//TODO : un utilisateur peu changer de nickname si il est déjà connected
void
Client::nick()
{
	if (_parsed_cmd.size() == 1)
	{
		_msg_buffer += ERR_NONICKNAMEGIVEN;
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
	//TODO verifier quoi renvoyer en cas de probleme sur les users https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.3
	// ERR_NEEDMOREPARAMS
	// ERR_ALREADYREGISTRED
	// 4 informations
	// user  =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF ); any octet except NUL, CR, LF, " " and "@"
	if (_data_connexion.size() == 2)
	{
		if (_parsed_cmd.size() < 5)
		{
			_msg_buffer += ERR_NEEDMOREPARAMS(_data_connexion[1], "USER");
			return ;
		}

		std::string	append;
		for (size_t i = 1; i < _parsed_cmd.size(); i++)
			append += _parsed_cmd[i];
		_data_connexion.push_back(append);

		_user = _data_connexion[1];
		//TODO : vérifier ces informations
		// Parameters: <user> <mode> <unused> <realname> ==> le user c'est le 2ème argument donc _data_connexion[1]
		// https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.3
		//? Je pense que c'est le user est le dernier arg de user
		//? _user = _parsed_cmd[4];

		_msg_buffer += RPL_WELCOME(_client_id_str, _nick);
		_msg_buffer += RPL_YOURHOST(_nick);
		// TODO : Date time (devra etre decommente)
		// _msg_buffer += RPL_CREATED(_nick, datetime).c_str(), strlen(RPL_CREATED(_nick, datetime).c_str()), 0);
		// TODO : check user_mode/chan_mode/Chan_parm_mode (exemples in the messages file) (devra etre decommente)
		// _msg_buffer += RPL_MYINFO(_nick, user_modes, chan_modes, chan_param_modes).c_str(), strlen(RPL_MYINFO(_nick, user_modes, chan_modes, chan_param_modes).c_str()), 0);
		_msg_buffer += RPL_ISUPPORT(_nick);
		_msg_buffer += WELCOME_ART;

		_connected = true;
		_modes.insert(std::pair<std::string, std::string>("r", "+r"));
		_server->_count_clients++;
		std::cout << GREEN << "[server] clients connected = " << _server->_count_clients << std::endl;
	}
	else
		_data_connexion.clear(); //TODO :doit on clear egalement _nick et _pass ?
}

