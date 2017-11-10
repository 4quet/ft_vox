/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/18 16:15:29 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/10 12:01:25 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_HPP
# define CONSTANTS_HPP

namespace Action {
	enum Enum { NONE, LEFT, RIGHT, FORWARD, BACKWARD, UP, DOWN, DEBUG, ESCAPE };
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

# define MAP_SIZE 16

# define MAX_CHUNK_SETUP_PER_FRAME 8

# define GROUND_LEVEL BLOCK_RENDER_SIZE * CHUNK_SIZE

#endif
