# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/07/21 11:32:23 by tpierron          #+#    #+#              #
#    Updated: 2017/10/25 10:35:49 by lfourque         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re

NAME = ft_vox

CC = clang++
SRC =	src/Sdl_gl_win.cpp \
		src/Skybox.cpp \
		src/Block.cpp \
		src/Chunk.cpp \
		src/ChunkManager.cpp \
		src/Skybox.cpp \
		src/Shader.class.cpp \
		src/FastNoise.cpp \
		main.cpp \


CFLAGS = -Wall -Wextra -Werror -Wno-unused-function
OBJ_PATH = ./obj/
OBJ_NAME = $(SRC:.cpp=.o)

SDL = -L/Users/$(LOGNAME)/.brew/lib -lSDL2
SDL_PATH = -I/Users/$(LOGNAME)/.brew/include/SDL2

GLM_PATH = -I/Users/$(LOGNAME)/.brew/include/glm

OPENGL = -framework OpenGl -framework AppKit

OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))

all: $(NAME)
$(OBJ_PATH)%.o: %.cpp
	@mkdir $(OBJ_PATH) 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/src 2> /dev/null || echo "" > /dev/null
	$(CC) $(CFLAGS) -o $@ -c -Isrc $(SDL_PATH) $(GLM_PATH) $< -std=c++11

$(NAME): $(OBJ)
	$(CC) $(SDL) $(GLM) $(OPENGL) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all
