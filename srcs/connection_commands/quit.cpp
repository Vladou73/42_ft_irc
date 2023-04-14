# include "../../headers/client.hpp"

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
