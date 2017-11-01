/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:27:54 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/01 13:15:08 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_MANAGER_HPP
# define CHUNK_MANAGER_HPP

# include "constants.hpp"
# include "Chunk.hpp"
# include "Camera.hpp"
# include "Frustum.hpp"
# include <map>

class ChunkManager
{
	public:
		ChunkManager();
		ChunkManager(glm::vec3);
		~ChunkManager();

		void	render(Shader &);

		void	update(Shader &, Camera &);
		
		void	updateLoadList();
		void	updateRenderList(Camera &);
		void	updateUnloadList();
		void	updateVisibilityList(Camera &);
		std::map<std::tuple<float, float, float>, Chunk*> & getChunks();

		size_t	getTotalActiveBlocks() const;
		size_t	getTotalActiveChunks() const;

	private:
		std::map<index3D, Chunk*>	_chunkMap;

		std::vector<Chunk*>	_loadList;
		std::vector<Chunk*>	_renderList;
		std::vector<Chunk*>	_unloadList;

		size_t		_totalActiveBlocks;
		size_t		_totalActiveChunks;
};


#endif /* CHUNK_MANAGER_HPP */
