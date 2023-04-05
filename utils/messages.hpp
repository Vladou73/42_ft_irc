#ifndef MESSAGES_HPP
#define MESSAGES_HPP

// std::string nick_taken;
// nick_taken = ":localhost 433 l l :Nickname is already in use.\n
// :l!@localhost NICK l"

# define WELCOME_ART "***************************************************\n               You are now connected to the server!\n***************************************************\r\n" 


# define USER_ID(nickname, username) (":" + nickname + "!" + username + "@localhost\r\n")

# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")

# define ERR_NEEDMOREPARAMS(client, command) (":localhost 461 " + client + " " + command + " :Not enough parameters\r\n")

# define ERR_NICKNAMEINUSE(client, nickname) (":localhost 433 " + client + " " + nickname + " :Nickname is already in use.\r\n")

# define ERR_ERRONEUSNICKNAME(client, nickname) (":localhost 432 " + client + " " + nickname + " :Erroneus nickname\r\n")

#endif