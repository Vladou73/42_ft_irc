# include "channel.hpp"


// =============================================================================
// CONSTRUCTORS ================================================================
Channel::Channel() : _name(), _fd_users() 
{}

Channel::Channel(std::string name) : _name(name), _fd_users() 
{}


// =============================================================================
// DESTRUCTORS =================================================================
Channel::~Channel()
{}

// =============================================================================
// GETTERS / SETTERS ===========================================================
void
Channel::setChan(int fd)
{
    _fd_users.push_back(fd);
}

void
Channel::delUser(int fd)
{
    std::vector<int>::iterator it = _fd_users.begin();
    std::vector<int>::iterator end = _fd_users.end();

    for(; it != end; it++)
    {
        if (*(it) == fd)
        {
            _fd_users.erase(it);
            break ;
        }
    }
}
