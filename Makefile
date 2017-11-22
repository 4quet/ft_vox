# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/07/21 11:32:23 by tpierron          #+#    #+#              #
#    Updated: 2017/11/22 11:36:07 by tpierron         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re

NAME = ft_vox

CC = clang++
SRC =	src/Sdl_gl_win.cpp \
		src/Skybox.cpp \
		src/Chunk.cpp \
		src/ChunkManager.cpp \
		src/BiomeManager.cpp \
		src/Skybox.cpp \
		src/Camera.cpp \
		src/GLString.cpp \
		src/Shader.class.cpp \
		libs/FastNoise.cpp \
		src/Frustum.cpp \
		main.cpp \


CFLAGS = -Wall -Wextra -Werror -Wno-unused
OBJ_PATH = ./obj/
OBJ_NAME = $(SRC:.cpp=.o)
PACKAGES = sdl2 freetype2 glm

PATHS = $(shell pkg-config --cflags $(PACKAGES))
LIBS = $(shell pkg-config --libs $(PACKAGES))

OPENGL = -framework OpenGl -framework AppKit

OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))

all: $(NAME)
$(OBJ_PATH)%.o: %.cpp
	@mkdir $(OBJ_PATH) 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/src 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/libs 2> /dev/null || echo "" > /dev/null
	$(CC) $(CFLAGS) -o $@ -c -Isrc $(PATHS) $< -std=c++11

$(NAME): $(OBJ)
	$(CC) $(SDL) $(LIBS) $(OPENGL) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all
