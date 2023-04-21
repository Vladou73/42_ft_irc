NAME =	ircserv

SRCS =	srcs/main.cpp\
		srcs/function_utils.cpp\
		srcs/server.cpp\
		srcs/client.cpp\
		srcs/client_commands.cpp\
		srcs/channel.cpp\
		srcs/channel_commands/join.cpp\
    	srcs/channel_commands/privmsg.cpp\
		srcs/channel_commands/part.cpp\
		srcs/channel_commands/topic.cpp\
		srcs/channel_commands/list.cpp\
		srcs/channel_commands/names.cpp\
		srcs/channel_commands/kick.cpp\
		srcs/connection_commands/quit.cpp\
		srcs/server_commands/oper.cpp\
		srcs/server_commands/kill.cpp\
		srcs/server_commands/mode.cpp\
# srcs/channel_commands.cpp\

CC = c++

OBJS_PATH = objects/
OBJS = ${addprefix ${OBJS_PATH}, ${SRCS:.cpp=.o}}

CFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address -g3

$(NAME_FT): $(OBJS_FT)
	$(CXX) $(CXXFLAGS) -o $(NAME_FT) $(OBJS_FT)

$(NAME_STD): $(OBJS_STD)
	$(CXX) $(CXXFLAGS) -o $(NAME_STD) $(OBJS_STD)


${OBJS_PATH}%.o: ${SRCS_PATH}%.cpp
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $< -o $@ ${INCLUDES}

all:        $(NAME)

$(NAME):    $(OBJS)
	${CC} ${CFLAGS} $(OBJS) -o $(NAME)

clean:
	${RM} ${OBJS}

fclean:			clean
	${RM} ${NAME}

re:				fclean all

.PHONY:			all clean fclean re
