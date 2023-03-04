# Project compilation files and directories

NAME					=	ircserv

SRCS					=	main.cpp \
							Server.cpp \
							User.cpp \
							utils.cpp \
							Command.cpp \
							Channel.cpp \
							commands/Pass.cpp \
							commands/Nick.cpp \
							commands/Username.cpp \
							commands/Privmsg.cpp \
							commands/Ping.cpp \
							commands/Pong.cpp \
							commands/Mode.cpp \
							commands/Quit.cpp \
							commands/ChannelJoin.cpp \
							commands/ChannelPart.cpp \
							commands/ChannelKick.cpp \
							commands/ChannelInvite.cpp \
							commands/ChannelTopic.cpp \
							commands/ChannelNames.cpp \
							commands/ChannelList.cpp

SRCSFD					=	srcs/
OBJSFD					=	objs/
HDR_INC					=	-I./includes
# # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Compilation
CFLAGS					=	-Wall -Wextra -Werror -std=c++98
# # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Colors
RED						=	\033[0;31m
GREEN					=	\033[0;32m
NONE					=	\033[0m
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

all: project $(NAME)
	@echo "... project ready"

project:
	@echo "Checking project ..."

$(OBJSFD):
	@mkdir -p $@/commands
	@echo "\t[ $(GREEN)✔$(NONE) ] $@ directory"

$(NAME): $(OBJSFD) $(SRCS:%.cpp=$(OBJSFD)%.o)
	@clang++ $(CFLAGS) $(SRCS:%.cpp=$(OBJSFD)%.o) -o $@
	@echo "\t[ $(GREEN)✔$(NONE) ] $(NAME) executable"

$(OBJSFD)%.o: $(SRCSFD)%.cpp
	@clang++ $(CFLAGS) $(HDR_INC) -c $< -o $@
	@echo "\t[ $(GREEN)✔$(NONE) ] $@ object"

clean:
	@echo "Cleaning project ..."
	@/bin/rm -rf $(OBJSFD)
	@echo "\t[ $(RED)✗$(NONE) ] Objects directory"

fclean: clean
	@/bin/rm -f $(NAME)
	@echo "\t[ $(RED)✗$(NONE) ] $(NAME) executable deleted"

re: fclean all

.PHONY: project all clean fclean re
