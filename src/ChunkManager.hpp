/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:27:54 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/25 10:32:59 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_MANAGER_HPP
# define CHUNK_MANAGER_HPP

# include "FastNoise.h"
# include "constants.hpp"

class ChunkManager
{
	public:
		ChunkManager();
		~ChunkManager();

		float	getHeightMapValue(unsigned int, unsigned int) const;

	private:
		float	_heightMap[CHUNK_SIZE][CHUNK_SIZE];
};

#endif /* CHUNK_MANAGER_HPP */
