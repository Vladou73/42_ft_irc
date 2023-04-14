# include "../headers/function_utils.hpp"

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
