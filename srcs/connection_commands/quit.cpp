# include "../../headers/client.hpp"

void
Client::quit(Client &client, std::string msg) //quit est aussi appelé par KILL donc on a besoin de lui passer le client qui doit quit ou celui qui est kill
{
	if (client._parsed_cmd.size() > 1 && msg.empty())
		client._quit_msg = client._parsed_cmd[1];
	else
		client._quit_msg = msg;

	client._msg_buffer += ERROR(client._quit_msg); //d'apres la documentation c'est ce message qu'il faut envoyer au client qui quitte
	if (!_server->_clients[client._client_id]._user.empty())
        _server->_count_clients--;

	client._socket_connected = false;
	close(client._client_id);
}
