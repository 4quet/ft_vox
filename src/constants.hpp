/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/18 16:15:29 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/22 10:51:42 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_HPP
# define CONSTANTS_HPP

# define WINDOW_WIDTH 1920.f
# define WINDOW_HEIGHT 1080.f
# define FOV 80.f
# define Z_NEAR 0.1f
# define Z_FAR 4000.f

namespace Action {
	enum Enum { NONE, LEFT, RIGHT, FORWARD, BACKWARD, UP, DOWN, DEBUG,
				ESCAPE, TOGGLESPEED, ERASE};
}

namespace Faces {
	enum Enum { FRONT, BACK, RIGHT, LEFT, TOP, BOTTOM };
}

namespace Biomes {
	enum Enum { FIELD, DESERT, SNOW, CANYON };
}

namespace BlockTypes {
	enum Enum: char { INACTIVE = 0, WATER, GRASS, DIRT, SAND, SNOW, ROCK};
}

# define CHUNK_SIZE	16
# define BLOCK_RENDER_SIZE	1.0f
# define CHUNK_RENDER_SIZE BLOCK_RENDER_SIZE * CHUNK_SIZE

# define SEED 1337

# define GROUND_LEVEL 0.0f
# define CAVE_LEVEL	GROUND_LEVEL - CHUNK_RENDER_SIZE
# define WATER_LEVEL GROUND_LEVEL + CHUNK_RENDER_SIZE

# define SAND_LEVEL WATER_LEVEL + 2 * BLOCK_RENDER_SIZE
# define ROCK_LEVEL GROUND_LEVEL + 3 * CHUNK_RENDER_SIZE
# define SNOW_LEVEL GROUND_LEVEL + 6 * CHUNK_RENDER_SIZE

// These constants are expressed in terms of chunks amount
# define VIEW_DISTANCE_WIDTH 24
# define VIEW_DISTANCE_HEIGHT 12
# define WORLD_BOTTOM 24
# define MAX_CHUNK_SETUP_PER_FRAME 12

#endif
