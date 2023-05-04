# include "../../headers/client.hpp"

void
Client::user()
{
	if (_connected == true)
	{
		_msg_buffer += ERR_ALREADYREGISTERED(_nick);
		return;
	}
	if (!_pass.empty() && !_nick.empty())
	{
		if (_parsed_cmd.size() < 5)
		{
			_msg_buffer += ERR_NEEDMOREPARAMS(_nick, "USER");
			return ;
		}

		if (_parsed_cmd[1].find('@') == std::string::npos)
			_user = _parsed_cmd[1];
		else
			return ;
		for (size_t i = 1; i < _parsed_cmd.size(); i++)
			_user_infos += _parsed_cmd[i];

		_msg_buffer += RPL_WELCOME(_nick, USER_ID2(_nick, _user));
		_msg_buffer += RPL_YOURHOST(_nick);
		_msg_buffer += RPL_CREATED(_nick, _server->_launch_date);
		_msg_buffer += RPL_MYINFO(_nick, "+r+o+i", "");
		_msg_buffer += RPL_ISUPPORT(_nick);
		_msg_buffer += WELCOME_ART;

		_connected = true;
		_modes.insert(std::pair<std::string, std::string>("r", "+r"));
		_server->_count_clients++;
		std::cout << GREEN << "[server] clients connected = " << _server->_count_clients << std::endl;
	}
}
