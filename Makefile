##
## EPITECH PROJECT, 2018
## makefile
## File description:
## makefile
##

SERVER	=	server

CLIENT	=	myftp

CC	=	gcc

RM	=	rm -f

SRCS_SERVER	=	./srcs/server/server.c			\
			./srcs/server/client.c			\
			./srcs/server/authent.c			\
			./srcs/server/login.c			\
			./srcs/server/socket.c			\
			./srcs/server/upload_download.c		\
			./srcs/server/actif_passif.c		\
			./srcs/server/move.c			\
			./srcs/server/help.c			\
			./srcs/server/cmds.c			\
			./srcs/server/random.c			\
			./srcs/server/session.c			\
			./srcs/server/norme.c			\
			./srcs/server/list.c			\
			./srcs/server/my_strncmp.c

SRCS_CLIENT	=	./srcs/client/client.c			\
			./srcs/client/link.c			\
			./srcs/client/system_cmds.c		\
			./srcs/client/action_before_send.c	\
			./srcs/client/socket.c			\
			./srcs/client/read_listen.c		\
			./srcs/client/ip.c			\
			./srcs/client/list.c			\
			./srcs/client/norme.c			\
			./srcs/client/stor.c			\
			./srcs/client/my_strncmp.c

OBJS_SERVER	=	$(SRCS_SERVER:.c=.o)

OBJS_CLIENT	=	$(SRCS_CLIENT:.c=.o)

CFLAGS	=	-I include

CFLAGS	+=	-W -Wall -Wextra

LDFLAGS =

.c.o:
		@$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@ && \
		printf "[\033[1;32mcompiled\033[0m] % 34s\n" $< | tr ' ' '.' || \
		printf "[\033[1;31mfailed\033[0m] % 36s\n" $< | tr ' ' '.'

all: 		$(SERVER)

$(SERVER):	$(OBJS_SERVER) $(OBJS_CLIENT)
		@$(CC) $(OBJS_SERVER) -o $(SERVER) $(LDFLAGS) && \
		(printf "[\033[1;36mbuilt\033[0m] % 37s\n" $(SERVER) | tr ' ' '.' || \
		printf "[\033[1;31mfailed\033[0m] % 36s\n" $(SERVER) | tr ' ' '.') && \
		$(CC) $(OBJS_CLIENT) -o $(CLIENT) $(LDFLAGS) && \
		printf "[\033[1;36mbuilt\033[0m] % 37s\n" $(CLIENT) | tr ' ' '.' || \
		printf "[\033[1;31mfailed\033[0m] % 36s\n" $(CLIENT) | tr ' ' '.'


clean:
		@$(RM) $(OBJS_CLIENT) && \
		(printf "[\033[1;31mdeleted\033[0m] % 35s\n" $(OBJS_CLIENT) | tr ' ' '.' || \
		printf "[\033[1;31mdeleted\033[0m] % 35s\n" $(OBJS_CLIENT) | tr ' ' '.') && \
		$(RM) $(OBJS_SERVER) && \
		(printf "[\033[1;31mdeleted\033[0m] % 35s\n" $(OBJS_SERVER) | tr ' ' '.' || \
		printf "[\033[1;31mdeleted\033[0m] % 35s\n" $(OBJS_SERVER) | tr ' ' '.')

fclean: 	clean
		@$(RM) $(SERVER) && \
		(printf "[\033[1;31mdeleted\033[0m] % 35s\n" $(SERVER) | tr ' ' '.' || \
		printf "[\033[1;31mdeleted\033[0m] % 35s\n" $(SERVER) | tr ' ' '.') && \
		$(RM) $(CLIENT) && \
		(printf "[\033[1;31mdeleted\033[0m] % 35s\n" $(CLIENT) | tr ' ' '.' || \
		printf "[\033[1;31mdeleted\033[0m] % 35s\n" $(CLIENT) | tr ' ' '.')

re:		fclean all

.PHONY: 	all clean fclean re
