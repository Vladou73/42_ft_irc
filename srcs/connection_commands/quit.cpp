# include "../../headers/client.hpp"

void
Client::quit()
{
	if (_parsed_cmd.size() > 1)
		_quit_msg = _parsed_cmd[1];

	send(_client_id, ERROR(_quit_msg).c_str(), ERROR(_quit_msg).size(), 0); //d'apres la documentation c'est ce message qu'il faut envoyer au client qui quitte
	if (!_server->_clients[_client_id]._user.empty())
        _server->_count_clients--;

	_socket_connected = false;
	close(_client_id);
}
