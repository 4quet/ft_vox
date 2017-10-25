/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constants.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/18 16:15:29 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/24 15:23:19 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTANTS_HPP
# define CONSTANTS_HPP

namespace Action {
	enum Enum { NONE, LEFT, RIGHT, FORWARD, BACKWARD, ESCAPE };
}

enum BlockType
{
	BLOCKTYPE_DEFAULT = 0,
	BlOCKTYPE_GRASS,
	BLOCKTYPE_STONE
};

# define BLOCK_RENDER_SIZE	0.5f
# define CHUNK_SIZE	1

#endif
