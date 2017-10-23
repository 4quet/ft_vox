# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/07/21 11:32:23 by tpierron          #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2017/10/23 11:31:09 by tpierron         ###   ########.fr        #
=======
#    Updated: 2017/10/23 10:54:59 by lfourque         ###   ########.fr        #
>>>>>>> d0d60e9639c28bad85931ab4a6c5918959296cc9
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re

NAME = ft_vox

CC = clang++
SRC =	src/Sdl_gl_win.cpp \
<<<<<<< HEAD
		src/Skybox.cpp \
=======
		src/Block.cpp \
>>>>>>> d0d60e9639c28bad85931ab4a6c5918959296cc9
		main.cpp \


CFLAGS = -Wall -Wextra -Werror
OBJ_PATH = ./obj/
OBJ_NAME = $(SRC:.cpp=.o)

SDL = -L/Users/$(LOGNAME)/.brew/lib -lSDL2
SDL_PATH = -I/Users/$(LOGNAME)/.brew/include/SDL2

OPENGL = -framework OpenGl -framework AppKit

OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))

all: $(NAME)
$(OBJ_PATH)%.o: %.cpp
	@mkdir $(OBJ_PATH) 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/src 2> /dev/null || echo "" > /dev/null
	$(CC) $(CFLAGS) -o $@ -c -Isrc $(SDL_PATH) $< -std=c++11

$(NAME): $(OBJ)
	$(CC) $(SDL) $(OPENGL) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all
