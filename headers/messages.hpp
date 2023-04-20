#ifndef MESSAGES_HPP
#define MESSAGES_HPP

// std::string nick_taken;
// nick_taken = ":localhost 433 l l :Nickname is already in use.\n
// :l!@localhost NICK l"

// =============================================================================
// USER ID =====================================================================

// prefix     =   ":" + servername / ( nickname [ [ "!" user ] "@" host ] )
# define USER_ID(nickname, username) (":" + nickname + "!" + username + "@localhost\r\n")
# define USER_ID2(nickname, username) (":" + nickname + "!" + username + "@localhost")

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
# define ERR_NOSUCHNICK(username, target) ("401 " + username + " " + target + " :No such nick\r\n")
# define ERR_NORECIPIENT(nickname) ("411 " + nickname + " :No recipient given PRIVMSG\r\n")
# define ERR_NOTEXTTOSEND(nickname) ("412 " + nickname + " :No text to send\r\n")
# define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + "@localhost PRIVMSG " + target + " :" + message + "\r\n")
// # define ERR_NOSUCHCHANNEL(username, channel) ("402 " + username + " " + channel + " :No such channel\r\n")

// =============================================================================
// QUIT MESSAGES ===============================================================
// The QUIT command is used to terminate a client’s connection to the server.
// The server acknowledges this by replying with an ERROR message and closing the connection to the client.
# define ERROR(reason) ("ERROR : " + reason + "\r\n")
# define RPL_QUIT(user_id, reason) (user_id + " QUIT :Quit: " + reason + "\r\n")


// =============================================================================
// JOIN MESSAGES ===============================================================

// FORMAT A RENVOYER POUR LE JOIN ":WiZ!jto@tolsun.oulu.fi JOIN #Twilight_zone" ; JOIN message from WiZ on channel #Twilight_zone
// # define JOIN_CHAN(nickname, username, chan_name) (":" + nickname + "!" + username + "@localhost JOIN " + chan_name + "\r\n")
# define JOIN_CHAN(nickname, chan_name) (":" + nickname + "!" + nickname + "@localhost JOIN " + chan_name + "\r\n") //TODO mauvais format

# define RPL_NAMREPLY(nickname, channel, client_list) (":localhost 353 " + nickname + " = " + channel + " :"+ client_list + "\r\n")
# define RPL_ENDOFNAMES(nickname, channel) (":localhost 366 " + nickname + " " + channel + " :End of NAMES list\r\n")
# define ERR_ALREADYINCHAN(nickname, chan) (":localhost " + nickname + " :You are already in the channel " + chan + "\r\n")


// ==============================================================================
// TOPIC MESSAGES ===============================================================
# define ERR_NOTONCHANNEL(nickname, chan_name) ("442 " + nickname + " " + chan_name + " :You're not on that channel\r\n")
# define RPL_NOTOPIC(nickname) ("331 " + nickname + " :No topic is set\r\n")
# define RPL_TOPIC(nickname, channel, topic) (":localhost 332 " + nickname + " " + channel + " :"+ topic + "\r\n")

// ==============================================================================
// LIST MESSAGES ================================================================
# define RPL_LISTSTART(nickname) (":localhost 321 " + nickname + " Channel :Users Name\r\n")
# define RPL_LIST(nickname, chan, number, topic) (":localhost 322 " + nickname + " Channel " + chan + " " + number + " :" + topic + "\r\n")
# define RPL_LISTEND(nickname) (":localhost 323 " + nickname + " :End of /LIST\r\n")

// =============================================================================
// PART MESSAGES ===============================================================
# define ERR_NOSUCHCHANNEL(nickname, chan_name) ("403 " + nickname + " " + chan_name + " :No such channel\r\n")
# define ERR_INVALIDCHANNAME(chan_name) (chan_name + ": this channel name is invalid \r\n")

// :WiZ!jto@tolsun.oulu.fi PART #playzone :I lost
// # define RPL_PART(nickname, username, chan_name) (":" + nickname + "!" + username + "@localhost PART " + chan_name + " :" + msg + "\r\n")
# define RPL_PART(chan_name, msg) (":successfuly left channel " + chan_name + " :" + msg + "\r\n") //TODO mauvais format de réponse
# define RPL_PART2(nickname, chan_name, msg) (":" + nickname + " left the channel " + chan_name + " :" + msg + "\r\n")

// =============================================================================
// NAMES MESSAGES ==============================================================

// =============================================================================
// SERVER OPERATOR MESSAGES ===========================================================
# define ERR_NOOPERHOST(nickname) ("491 " + nickname + " :No O-lines for your host\r\n")
# define RPL_YOUREOPER(nickname) ("381 " + nickname + " :You are now an IRC operator\r\n")
# define ERR_NOPRIVILEGES(nickname) ("481 " + nickname + " :Permission Denied- You're not an IRC operator\r\n")

// =============================================================================
// MODE MESSAGES ===========================================================
# define ERR_USERSDONTMATCH(nickname) ("502 " + nickname + " :Can't change or view mode for other users\r\n")
# define RPL_UMODEIS(nickname, modes) ("221 " + nickname + " :" + modes + "\r\n")
# define ERR_UMODEUNKNOWNFLAG(nickname, mode) ("501 " + nickname + " :Unknown MODE flag " + mode + "\r\n")
# define RPL_CHANNELMODEIS(nickname, channel, modes) ("324 " + nickname + " " + channel + " :" + modes + "\r\n")
# define ERR_CHANOPRIVSNEEDED(nickname, channel) ("482 " + nickname + " " + channel + " :You're not channel operator\r\n")
# define ERR_UNKNOWNMODE(modechar, channel) ("472 " + modechar + " :is unknown mode char for " + channel + "\r\n")

#endif
