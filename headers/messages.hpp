#ifndef MESSAGES_HPP
#define MESSAGES_HPP

// std::string nick_taken;
// nick_taken = ":localhost 433 l l :Nickname is already in use.\n
// :l!@localhost NICK l"

# define WELCOME_ART "******************************************************************\n               You are now connected to the server!\n******************************************************************\r\n" 
# define RPL_WELCOME(user_id, nickname) (":localhost 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
# define RPL_YOURHOST(nickname) (":localhost 002 " + nickname + " :Your host is ircserv (localhost), running version1.1\r\n")
/*
 RPL_CREATED (003)

  "<client> :This server was created <datetime>"
Part of the post-registration greeting, this numeric returns a human-readable date/time that the server was started or created. The text used in the last param of this message varies wildly.
*/
# define RPL_CREATED(nickname, datetime) (":localhost 003 " + nickname + " :This server was created " + datetime + "\r\n")

//RPL_MYINFO(it->second.getNickname(), "localhost", "1.1", "io", "kost", "k"));
# define RPL_MYINFO(nickname, user_modes, chan_modes, chan_param_modes) (":localhost 004 " + nickname + " ircserv 1.1 " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")

//RPL_ISUPPORT(it->second.getNickname(), "CHANNELLEN=32 NICKLEN=9 TOPICLEN=307"));
# define RPL_ISUPPORT(nickname) (":localhost 005 " + nickname + " CHANNELLEN=32 NICKLEN=9 TOPICLEN=307 :are supported by this server\r\n")



# define USER_ID(nickname) (":" + nickname + "!" + nickname + "@localhost\r\n")

# define ERR_NEEDMOREPARAMS(nickname, command) (":localhost 461 " + nickname + " " + command + " :Not enough parameters\r\n")

# define ERR_NICKNAMEINUSE(nickname) (":localhost 433 " + nickname + " " + nickname + " :Nickname is already in use.\r\n")

# define ERR_ERRONEUSNICKNAME(nickname) (":localhost 432 " + nickname + " " + nickname + " :Erroneus nickname\r\n")

# define RPL_PONG(token) (":localhost PONG ircserv " + token + "\r\n")

# define ERR_PASSWDMISMATCH ":localhost 464 :Password incorrect\r\n"

# define ERR_ALREADYREGISTERED(nickname) (":localhost 462 " + nickname + " :You may not reregister\r\n")

# define ERR_NONICKNAMEGIVEN ":localhost 431 :There is no nickname.\r\n"

#endif