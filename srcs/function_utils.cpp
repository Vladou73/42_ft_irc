# include "../headers/function_utils.hpp"

std::string
change_to_str(int a)
{
	std::stringstream ss;
	ss << a;
	std::string str = ss.str();
	return (str);
}