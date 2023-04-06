#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "utils/ft_irc.hpp"

class Channel
{

private:

    std::string			_name;
	std::vector<int>	_fd_users;


public:

    Channel();
    Channel(std::string name);
    ~Channel();

	void	setChan(int fd);
	void	delUser(int fd);

};

#endif