# include "utils/client_commands.hpp"


//The PING command is sent by either clients or servers to check the other side of the connection is still connected and/or to check for connection latency, at the application layer.
void
ping(bool connected, std::vector<std::string> parsed_cmds, int client_id, std::string nickname)
{
    if (parsed_cmds.size() == 1)
        send(client_id, ERR_NEEDMOREPARAMS(nickname, "PING").c_str(), ERR_NEEDMOREPARAMS(nickname, "PING").size(), 0);
    else if (connected == true)
        send(client_id, RPL_PONG(parsed_cmds[1]).c_str(), RPL_PONG(parsed_cmds[1]).size(), 0);        
}

void
pass(std::vector<std::string> &data_connexion, bool connected, std::string password, std::vector<std::string> parsed_cmds, int client_id, std::string nickname)
{
    if (connected == true)
    {
        send(client_id, ERR_ALREADYREGISTERED(nickname).c_str(), ERR_ALREADYREGISTERED(nickname).size(), 0);
        return ;
    }
    if (parsed_cmds[1] != password)
	{
		data_connexion.clear();
        send(client_id, ERR_PASSWDMISMATCH, strlen(ERR_PASSWDMISMATCH), 0);
		return ;
	}
    if (data_connexion.size() > 0)
        return ;
    // data_connexion.clear();
    data_connexion.push_back(parsed_cmds[1]);
}

bool	check_nick(std::map<int, Client> &clients, std::vector<std::string> &data_connexion, int client_id, std::string &nickname, std::string new_nickname, bool connected)
{
	//TODO verifier quoi checker sur les nicknames
	//TODO verifier quoi renvoyer en cas de probleme sur les nicknames
	// nickname   =  ( letter / special ) *8( letter / digit / special / "-" )

	//TODO verifier avec irssi le nombre max de caracteres
	// if(data_connexion[1].size() >= 9)
	// {
	// 	send (client_id, ERR_ERRONEUSNICKNAME(data_connexion[1],data_connexion[1]).c_str(), strlen(ERR_ERRONEUSNICKNAME(data_connexion[1],data_connexion[1]).c_str()), 0);
	// 	return false;
	// }

	std::cout << "check_nick()\n";

	std::map<int, Client>::iterator end = clients.end();
	for (std::map<int, Client>::iterator it = clients.begin(); it != end; it++)
	{
		if (it->second.getNick() == data_connexion[1])
		{
			send (client_id, ERR_NICKNAMEINUSE(data_connexion[1]).c_str(), strlen(ERR_NICKNAMEINUSE(data_connexion[1]).c_str()), 0);
			send (client_id, USER_ID(data_connexion[1]).c_str(), strlen(USER_ID(data_connexion[1]).c_str()), 0);
			return (false);
		}
	}
	std::cout << "data_connexion[1]= " << data_connexion[1] << std::endl;
	if (connected == false)
		nickname = data_connexion[1];
	else
		nickname = new_nickname;
	std::cout << "_nick="  << nickname << std::endl;
	send (client_id, USER_ID(data_connexion[1]).c_str(), strlen(USER_ID(data_connexion[1]).c_str()), 0);
	return true;
}

void
nick(std::vector<std::string> &data_connexion, bool connected, std::vector<std::string> parsed_cmds, int client_id, std::string &nickname, std::map<int, Client> &clients)
{
	std::string new_nickname;
	std::cout << "'"<< parsed_cmds[1] << "'" << std::endl;
	if (parsed_cmds.size() == 1)
	{
		send (client_id, ERR_NONICKNAMEGIVEN, strlen(ERR_NONICKNAMEGIVEN), 0);
		return;
	}
	if (connected == true)
		check_nick(clients, data_connexion, client_id, nickname, parsed_cmds[1], connected);
    else if (data_connexion.size() == 1)
    {
		std::cout << "COUCOU TEST!\n";

		std::cout << "'"<< data_connexion[0] << "'" << std::endl;
        data_connexion.push_back(parsed_cmds[1]);
		std::cout << "'"<< data_connexion[1] << "'" << std::endl;
        if (check_nick(clients, data_connexion, client_id, new_nickname, "", connected) == false)
    		data_connexion.pop_back();
    }
}