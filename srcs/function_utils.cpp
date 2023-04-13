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