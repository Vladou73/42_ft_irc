#ifndef MESSAGES_HPP
#define MESSAGES_HPP

// std::string nick_taken;
// nick_taken = ":localhost 433 l l :Nickname is already in use.\n
// :l!@localhost NICK l"

// =============================================================================
// USER ID =====================================================================
# define USER_ID(nickname) (":" + nickname + "!" + nickname + "@localhost\r\n")
# define USER_ID2(nickname) (":" + nickname + "!" + nickname + "@localhost")


// =============================================================================
// PASS MESSAGES ===============================================================
# define ERR_NEEDMOREPARAMS(nickname, command) (":localhost 461 " + nickname + " " + command + " :Not enough parameters\r\n")
# define ERR_ALREADYREGISTERED(nickname) (":localhost 462 " + nickname + " :You may not reregister\r\n")
# define ERR_PASSWDMISMATCH ":localhost 464 :Password incorrect\r\n"


// =============================================================================
// NICK MESSAGES ===============================================================
# define ERR_NONICKNAMEGIVEN ":localhost 431 :There is no nickname.\r\n"
# define ERR_ERRONEUSNICKNAME(nickname) (":localhost 432 " + nickname + " " + nickname + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(nickname) (":localhost 433 " + nickname + " " + nickname + " :Nickname is already in use.\r\n")


// =============================================================================
// WELCOME MESSAGES ============================================================
# define WELCOME_ART "******************************************************************\n               You are now connected to the server!\n******************************************************************\r\n" 
# define SEPARATOR "-------------------------------------------------\r\n"
# define SEPARATOR_END "-------------------------------------------------\r\n\n"
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

# define RPL_ISUPPORT(nickname) (":localhost 005 " + nickname + " CHANNELLEN=32 NICKLEN=9 TOPICLEN=307 :are supported by this server\r\n")


// =============================================================================
// PONG MESSAGES ===============================================================
# define RPL_PONG(token) (":localhost PONG ircserv " + token + "\r\n")


// =============================================================================
// PRIVATE MESSAGES ============================================================
# define ERR_NOSUCHNICK(nickname, target) ("401 " + nickname + " " + target + " :No such nick/channel\r\n")
# define ERR_NORECIPIENT(nickname) ("411 " + nickname + " :No recipient given PRIVMSG\r\n")
# define ERR_NOTEXTTOSEND(nickname) ("412 " + nickname + " :No text to send\r\n")
# define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + "@localhost PRIVMSG " + target + " " + message + "\r\n")



// =============================================================================
// QUIT MESSAGES ===============================================================
# define RPL_QUIT(user_id, reason) (user_id + " QUIT :Quit: " + reason + "\r\n")


// =============================================================================
// JOIN MESSAGES ===============================================================
# define JOIN_CHAN(nickname, chan_name) (nickname + "!" + nickname + "@localhost\nJOIN" + chan_name + "\r\n")
# define RPL_NAMREPLY(nickname, channel, client_list) (":localhost 353 " + nickname + " = " + channel + " :"+ client_list + "\r\n")
# define RPL_ENDOFNAMES(nickname, channel) (":localhost 366 " + nickname + " " + channel + " :End of NAMES list\r\n")

// ==============================================================================
// TOPIC MESSAGES ===============================================================
# define ERR_NOTONCHANNEL(nickname) ("442 " + nickname + " :You're not on that channel\r\n")
# define ERR_CHANNOTFOUND(nickname) (nickname + " :This channel does not exist\r\n")
# define RPL_NOTOPIC(nickname) ("331 " + nickname + " :No topic is set\r\n")
# define RPL_TOPIC(nickname, channel, topic) (":localhost 332 " + nickname + " " + channel + " :"+ topic + "\r\n")

// ==============================================================================
// LIST MESSAGES ================================================================
# define RPL_LISTSTART(nickname) (":localhost 321 " + nickname + " Channel :Users Name\r\n")
# define RPL_LIST(nickname, chan, number, topic) (":localhost 322 " + nickname + " Channel " + chan + " " + number + " :" + topic + "\r\n")
# define RPL_LISTEND(nickname) (":localhost 323 " + nickname + " :End of /LIST\r\n")

#endif