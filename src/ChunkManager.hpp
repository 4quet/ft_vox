/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:27:54 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/25 13:44:01 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_MANAGER_HPP
# define CHUNK_MANAGER_HPP

# include "FastNoise.h"
# include "constants.hpp"
# include "Chunk.hpp"

class ChunkManager
{
	public:
		ChunkManager();
		~ChunkManager();

		void	render(Shader &);
		float	getHeightMapValue(unsigned int, unsigned int) const;

	private:
		Chunk**		_chunks;
		float		_heightMap[CHUNK_SIZE][CHUNK_SIZE];
};


#endif /* CHUNK_MANAGER_HPP */
