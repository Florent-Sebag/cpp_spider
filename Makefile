##
## Makefile for makefile in /home/benamar/cpp_spider/Server
## 
## Made by benamar
## Login   <mahdi.benamar@epitech.eu>
## 
## Started on  Sun Oct  1 20:27:21 2017 benamar
## Last update Sun Oct  1 21:16:30 2017 benamar
##

NAME		=	server

CC		=	g++

SRCS		=	./Server/tcp_connection.cpp \
			./Server/server.cpp \
			./Server/DataManagement.cpp \
			./Server/Pswd.cpp \

OBJS		=	$(SRCS:.cpp=.o)

CPPFLAGS	+= 	-std=c++11 -lboost_system -L/usr/lib `mysql_config --cflags --libs` -lssl -lcrypto -W -Wall -Wextra -Werror  -I./include -g3

RM		=	rm -f

all:			$(NAME)

$(NAME):		$(OBJS)
			$(CC) -o $(NAME) $(OBJS) $(CPPFLAGS)

clean:
			$(RM) $(OBJS)

fclean:			clean
			$(RM) $(NAME)

re:			clean fclean all

.PHONY:			all clean fclean re
