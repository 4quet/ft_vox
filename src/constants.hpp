/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/18 16:15:29 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/13 16:40:37 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_HPP
# define CONSTANTS_HPP

namespace Action {
	enum Enum { NONE, LEFT, RIGHT, FORWARD, BACKWARD, UP, DOWN, DEBUG, ESCAPE, TOGGLESPEED};
}

namespace Faces {
	enum Enum { FRONT, BACK, RIGHT, LEFT, TOP, BOTTOM };
}

enum BlockType
{
	BLOCKTYPE_DEFAULT = 0,
	BLOCKTYPE_GRASS,
	BLOCKTYPE_STONE
};

# define BLOCK_RENDER_SIZE	1.0f
# define CHUNK_SIZE	16
# define GROUND_LEVEL BLOCK_RENDER_SIZE * CHUNK_SIZE

// These constants are expressed in terms of chunks amount
# define VIEW_DISTANCE_WIDTH 12
# define VIEW_DISTANCE_HEIGHT 6
# define MAX_ALTITUDE 4
# define WORLD_BOTTOM 16
# define MAX_CHUNK_SETUP_PER_FRAME 12

#endif
