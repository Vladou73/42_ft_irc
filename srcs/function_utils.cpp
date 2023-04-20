# include "../headers/function_utils.hpp"

bool
check_operator(std::vector<int> &oper, int client_id)
{
	std::vector<int>::iterator it = oper.begin();
	std::vector<int>::iterator end = oper.end();
	for(; it != end; it++)
	{
		if (*it == client_id)
			return true;
	}
	return false;
}

std::string
change_to_str(int a)
{
	std::stringstream ss;
	ss << a;
	std::string str = ss.str();
	return (str);
}

std::vector<std::string>
parse_commas(std::string buff)
{
    std::stringstream			strstream(buff);
	std::vector<std::string>	parsed_buff;
	std::string 				word;

	while(getline(strstream, word, ','))
	{
		parsed_buff.push_back(word);
	}
    return parsed_buff;
}

bool
check_channel_name(std::string name)
{
	if (name.size() < 1)
		return false;
	if (name[0] != '#' && name[0] != '&') //channel names must be identified by # or &
		return false;
	if (name.find("^G") != std::string::npos
		|| name.find(" ") != std::string::npos
		|| name.find(",") != std::string::npos)
		return false;
    return true;
}

bool
check_on_chan(std::vector<std::string> canals, std::string arg)
{
	std::vector<std::string>::iterator it = canals.begin();
	std::vector<std::string>::iterator end = canals.end();
	for(; it != end; it++)
	{
		if (*it == arg)
			return true;
	}
	return false;
}

int
search_for_client_by_nick(std::string nick, std::map<int, Client> clients)
{
    std::map<int, Client>::iterator it;
    for (it = clients.begin(); it != clients.end(); it++)
    {
        if (nick == it->second.getNick())
            return it->first;
    }
    return -1;
}