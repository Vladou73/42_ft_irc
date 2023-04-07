#ifndef MESSAGES_HPP
#define MESSAGES_HPP

// std::string nick_taken;
// nick_taken = ":localhost 433 l l :Nickname is already in use.\n
// :l!@localhost NICK l"

# define WELCOME_ART "***************************************************\n               You are now connected to the server!\n***************************************************\r\n" 


# define USER_ID(nickname) (":" + nickname + "!" + nickname + "@localhost\r\n")

# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")

# define ERR_NEEDMOREPARAMS(nickname, command) (":localhost 461 " + nickname + " " + command + " :Not enough parameters\r\n")

# define ERR_NICKNAMEINUSE(nickname) (":localhost 433 " + nickname + " " + nickname + " :Nickname is already in use.\r\n")

# define ERR_ERRONEUSNICKNAME(nickname) (":localhost 432 " + nickname + " " + nickname + " :Erroneus nickname\r\n")

# define RPL_PONG(token) (":localhost PONG ircserv " + token + "\r\n")

# define ERR_PASSWDMISMATCH ":localhost 464 :Password incorrect\r\n"

# define ERR_ALREADYREGISTERED(nickname) (":localhost 462 " + nickname + " :You may not reregister\r\n")

# define ERR_NONICKNAMEGIVEN ":localhost 431 :There is no nickname.\r\n"

#endif