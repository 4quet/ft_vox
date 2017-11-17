/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/18 16:15:29 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/16 15:54:57 by tpierron         ###   ########.fr       */
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

# define BLOCK_RENDER_SIZE	1.0f
# define CHUNK_SIZE	16
# define GROUND_LEVEL BLOCK_RENDER_SIZE * CHUNK_SIZE
# define WATER_LEVEL GROUND_LEVEL * 2
# define ROCK_LEVEL GROUND_LEVEL * 2 - 2
# define SAND_LEVEL GROUND_LEVEL * 2 + 2
# define SNOW_LEVEL GROUND_LEVEL * 4

// These constants are expressed in terms of chunks amount
# define VIEW_DISTANCE_WIDTH 12
# define VIEW_DISTANCE_HEIGHT 8
# define MAX_ALTITUDE 4
# define WORLD_BOTTOM 16
# define MAX_CHUNK_SETUP_PER_FRAME 12

#endif
