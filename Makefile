NAME		= webserv
CC			= c++
FLAGS		= -Wall -Wextra -std=c++98 -fsanitize=address -g #-Werror
SRC			= main.cpp Config.cpp Location.cpp ConfigParser.cpp Listen.cpp Socket.cpp Print.cpp HttpRequest.cpp
OBJ			= $(SRC:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ)
		$(CC) $(FLAGS) $(OBJ) -o $@ 

%.o : %.cpp Config.hpp ConfigParser.hpp Location.hpp Listen.hpp Socket.hpp Print.hpp HttpRequest.hpp
		$(CC) $(FLAGS) -c $< -o $@


clean :
		rm -f $(OBJ)

fclean : clean
		rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re