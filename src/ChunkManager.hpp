/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:27:54 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/26 13:57:00 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_MANAGER_HPP
# define CHUNK_MANAGER_HPP

# include "constants.hpp"
# include "Chunk.hpp"

class ChunkManager
{
	public:
		ChunkManager();
		~ChunkManager();

		void	render(Shader &);
		void	update(Shader &);

		size_t	getTotalActiveBlocks() const;
		size_t	getTotalActiveChunks() const;

	private:
	//	Chunk***	_chunks;
		std::vector<Chunk*>	_vChunks;

		size_t		_totalActiveBlocks;
		size_t		_totalActiveChunks;
};


#endif /* CHUNK_MANAGER_HPP */
