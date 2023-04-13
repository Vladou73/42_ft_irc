# include "../headers/channel.hpp"


// =============================================================================
// CONSTRUCTORS ================================================================
Channel::Channel() : _name(), _topic(), _clients() 
{}

Channel::Channel(std::string name) : _name(name), _topic(), _clients() 
{}


// =============================================================================
// DESTRUCTORS =================================================================
Channel::~Channel()
{}

// =============================================================================
// GETTERS / SETTERS ===========================================================
// void
// Channel::setChan(int fd)
// {
//     _fd_users.push_back(fd);
// }
std::map<int, Client *>
Channel::getClients()
{
    return _clients;
}

// // =============================================================================
// // METHODS =====================================================================
// void
// Channel::delUser(int fd)
// {
//     std::vector<int>::iterator it = _fd_users.begin();
//     std::vector<int>::iterator end = _fd_users.end();

//     for(; it != end; it++)
//     {
//         if (*(it) == fd)
//         {
//             _fd_users.erase(it);
//             break ;
//         }
//     }
// }
