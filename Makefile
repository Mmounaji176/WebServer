# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mdarify <mdarify@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/30 18:34:33 by mdarify           #+#    #+#              #
#    Updated: 2023/07/23 09:53:49 by mdarify          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Server
CPPFLAGS = -Wall -Wextra -Werror -std=c++98   #-fsanitize=address -g3
SRC = 	Serv/webserv.cpp Client/Client.cpp Serv/Server.cpp	configuration/config.cpp main.cpp\
		Serv/location.cpp Serv/Checklocation.cpp configuration/checkconfig.cpp
SRCS = ${SRC:.cpp=.o}

all: $(NAME)

$(NAME): $(SRCS)
	$(CXX) $(CPPFLAGS) $(SRCS) -o $(NAME)

clean:
	rm -f $(SRCS)
fclean: clean
	rm -f $(NAME)

re: fclean all
