NAME            =       ircserv

CC                      =       c++

CFLAGS          =       -Wall -Werror -Wextra -std=c++98 -g

# directories
SRC_DIR         =       src/
OBJ_DIR         =       obj/
INC_DIR         =       includes/

# controll codes
RESET           =       \033[0m
GREEN           =       \033[32m
YELLOW          =       \033[33m
BLUE            =       \033[34m
RED                     =       \033[31m
UP                      =       \033[A
CUT                     =       \033[K

#source files
SRC_FILES       =       main.cpp Client.cpp Server.cpp

OBJ_FILES       =       $(SRC_FILES:.cpp=.o)

#paths
SRC                     =       $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ                     =       $(addprefix $(OBJ_DIR), $(OBJ_FILES))

#all rule
all: $(NAME)

#compile the executable
$(NAME): $(OBJ)
		@echo "$(YELLOW)Compiling $(NAME)...$(RESET)"
		@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
		 @echo "$(GREEN)Finished $(NAME)$(RESET)"

#compile objects
$(OBJ_DIR)%.o:$(SRC_DIR)%.cpp
		@mkdir -p $(OBJ_DIR)
		@echo "$(YELLOW)Compiling [$@]...$(RESET)"
		@$(CC) $(CFLAGS) -I $(INC_DIR) -o $@ -c $<
		@printf "$(UP)$(CUT)"
		@echo "$(GREEN)Finished [$@]$(RESET)"
		@printf "$(UP)$(CUT)"

#clean rule
clean:
		@if [ -d "$(OBJ_DIR)" ]; then \
		rm -rf $(OBJ_DIR); \
		echo "$(BLUE)Deleting all objects...$(RESET)"; else \
		echo "No objects to remove"; \
		fi;

#fclean rule
fclean: clean
		@if [ -f "$(NAME)" ]; then \
		rm -f $(NAME); \
		echo "$(BLUE)Deleting $(NAME)...$(RESET)"; else \
		echo "No Executable to remove."; \
		fi;

#re rule
re: fclean all

#phony
.PHONY: all clean fclean re