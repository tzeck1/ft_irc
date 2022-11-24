# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tzeck <@student.42heilbronn.de>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/13 12:21:11 by btenzlin          #+#    #+#              #
#    Updated: 2022/11/24 10:10:48 by tzeck            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

VPATH = src .

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address
# COLORS
Y = "\033[33m"
R = "\033[31m"
G = "\033[32m"
B = "\033[34m"
X = "\033[0m"
UP = "\033[A"
CUT = "\033[K"
# EXECUTABLE
NAME = ./ircserv
# PATHS
OBJ_PATH = ./.obj/
# SOURCES
SRC =	main.cpp init_server.cpp utils.cpp loop_requests.cpp handle_cmds.cpp \
		user.cpp debug.cpp commands.cpp

# OBJECTS
OBJ :=$(addprefix $(OBJ_PATH), $(SRC:.cpp=.o))
# INCLUDES
INC = -I ./include

# RULES
all: $(NAME)
	$(NAME) 420 test

$(NAME): $(OBJ)
	@echo $(Y)Compiling [$(OBJ)]
	@printf $(UP)$(CUT)
	@echo into [$(NAME)]...$(X)
	@printf $(UP)$(CUT)
	@$(CXX) $(CXXFLAGS) $^ $(INC) -o $(NAME)
	@echo $(G)Finished [$(NAME)]$(X)

$(OBJ_PATH)%.o: %.cpp
	@echo $(Y)Compiling [$@]...$(X)
	@mkdir -p .obj
	@printf $(UP)$(CUT)
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< $(INC) -o $@
	@echo $(G)Finished [$@]$(X)
	@printf $(UP)$(CUT)

clean:
	@if [ -d "$(OBJ_PATH)" ]; then \
			rm -f -r $(OBJ_PATH); \
			echo $(R)Cleaning" "[$(OBJ) $(OBJ_PATH)]...$(X); else \
			echo "No objects to remove."; \
	fi;

fclean: clean
	@if [ -f "$(NAME)" ]; then \
			rm -f $(NAME); \
			echo $(R)Cleaning" "[$(NAME)]...$(X);else \
			echo "No executable to remove."; \
	fi;

re: fclean all

# ADDITIONAL RULES
.PHONY: all clean fclean re

-include $(OBJ:.o=.d)