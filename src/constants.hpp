/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/18 16:15:29 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/30 16:44:10 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_HPP
# define CONSTANTS_HPP

namespace Action {
	enum Enum { NONE, LEFT, RIGHT, FORWARD, BACKWARD, UP, DOWN, DEBUG, ESCAPE };
}

enum BlockType
{
	BLOCKTYPE_DEFAULT = 0,
	BlOCKTYPE_GRASS,
	BLOCKTYPE_STONE
};

# define BLOCK_RENDER_SIZE	1.0f
# define CHUNK_SIZE	16

# define MAP_SIZE 6

#endif
