/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/18 16:15:29 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/17 10:31:29 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_HPP
# define CONSTANTS_HPP

namespace Action {
	enum Enum { NONE, LEFT, RIGHT, FORWARD, BACKWARD, UP, DOWN, DEBUG, ESCAPE, TOGGLESPEED, ERASE};
}

namespace Faces {
	enum Enum { FRONT, BACK, RIGHT, LEFT, TOP, BOTTOM };
}

namespace Biomes {
	enum Enum { FIELD, DESERT, SNOW, CANYON };
}

enum BlockType: char
{
	BLOCKTYPE_INACTIVE = 0,
	BLOCKTYPE_GRASS,
	BLOCKTYPE_STONE,
	BLOCKTYPE_WATER,
	BLOCKTYPE_SAND,
	BLOCKTYPE_SNOW,
	BLOCKTYPE_ROCK
};

# define CHUNK_SIZE	16
# define BLOCK_RENDER_SIZE	1.0f
# define CHUNK_RENDER_SIZE BLOCK_RENDER_SIZE * CHUNK_SIZE

# define GROUND_LEVEL 0.0f
# define CAVE_LEVEL	GROUND_LEVEL - CHUNK_RENDER_SIZE
# define WATER_LEVEL GROUND_LEVEL + CHUNK_RENDER_SIZE

# define SAND_LEVEL WATER_LEVEL + 2 * BLOCK_RENDER_SIZE
# define ROCK_LEVEL GROUND_LEVEL + 3 * CHUNK_RENDER_SIZE
# define SNOW_LEVEL GROUND_LEVEL + CHUNK_RENDER_SIZE * 4

// These constants are expressed in terms of chunks amount
# define VIEW_DISTANCE_WIDTH 24
# define VIEW_DISTANCE_HEIGHT 8
# define MAX_ALTITUDE 20
# define WORLD_BOTTOM 16
# define MAX_CHUNK_SETUP_PER_FRAME 8

#endif
