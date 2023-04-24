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
# define RPL_CREATED(nickname, datetime) (":localhost 003 " + nickname + " :This server was created " + datetime + "\r\n")
# define RPL_MYINFO(nickname, user_modes, chan_modes) (":localhost 004 " + nickname + " ircserv 1.1 " + "USERMODES=" + user_modes + " " + chan_modes + "\r\n")
# define RPL_ISUPPORT(nickname) (":localhost 005 " + nickname + " CHANNELLEN=64 NICKLEN=9 TOPICLEN=307 :are supported by this server\r\n")


// =============================================================================
// PONG MESSAGES ===============================================================
# define RPL_PONG(token) (":localhost PONG ircserv " + token + "\r\n")


// =============================================================================
// PRIVATE MESSAGES ============================================================
# define ERR_NOSUCHNICK(username, target) (":localhost 401 " + username + " " + target + " :No such nick\r\n")
# define ERR_NORECIPIENT(nickname) (":localhost 411 " + nickname + " :No recipient given PRIVMSG\r\n")
# define ERR_NOTEXTTOSEND(nickname) (":localhost 412 " + nickname + " :No text to send\r\n")
# define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + "@localhost PRIVMSG " + target + " :" + message + "\r\n")
# define RPL_NOTICE(nick, username, target, message) (":" + nick + "!" + username + "@localhost NOTICE " + target + " :" + message + "\r\n")



// =============================================================================
// QUIT MESSAGES ===============================================================
// The QUIT command is used to terminate a client’s connection to the server.
// The server acknowledges this by replying with an ERROR message and closing the connection to the client.
# define ERROR(reason) ("ERROR : " + reason + "\r\n")
# define RPL_QUIT(user_id, reason) (user_id + " QUIT :Quit: " + reason + "\r\n")


// =============================================================================
// JOIN MESSAGES ===============================================================

// FORMAT A RENVOYER POUR LE JOIN ":WiZ!jto@tolsun.oulu.fi JOIN #Twilight_zone" ; JOIN message from WiZ on channel #Twilight_zone
# define JOIN_CHAN(user_id, chan_name) (user_id + " JOIN " + chan_name + "\r\n")
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
# define RPL_PART(user_id, chan_name, msg) (user_id + " PART " + chan_name + " :" + msg + "\r\n")
// # define RPL_PART2(user_id, chan_name, msg) (user_id + " PART " + chan_name + " :" + msg + "\r\n")

// =============================================================================
// INVITE MESSAGES ==============================================================
# define ERR_USERONCHANNEL(nick, chan) (":localhost 443 " + nick + " " + chan + " :is already on channel\r\n")
# define RPL_INVITING(nick, chan) ("INVITE " + nick + " " + chan + "\r\n")
# define INVITATION(nick, user, client_nick, chan) (":" + nick + "!" + user +"@localhost INVITE " + client_nick + " " + chan + "\r\n")
// =============================================================================
// KICK MESSAGES ==============================================================
# define RPL_KICK(nick, user, chan, client_kick, reason) (":" + nick + "!" + user + "@localhost KICK " + chan + " " + client_kick + " :" + reason + "\r\n")

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
