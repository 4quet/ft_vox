/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:27:54 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/23 17:22:12 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_MANAGER_HPP
# define CHUNK_MANAGER_HPP

# include "constants.hpp"
# include "Shader.class.hpp"
# include "Chunk.hpp"
# include "Camera.hpp"
# include "BiomeManager.hpp"
# include "Frustum.hpp"
# include <map>

class ChunkManager
{
	public:
		ChunkManager(glm::vec3);
		~ChunkManager();

		void						render(Shader &);
		void						update(Camera &);
		
		std::map<float, Chunk*> &	getRenderMap();

		size_t						getTotalActiveBlocks() const;
		size_t						getTotalActiveChunks() const;

	private:
		BiomeManager				bm;

		std::map<index3D, Chunk*>	_chunkMap;
		std::map<float, Chunk*>		_setupMap;
		std::map<float, Chunk*>		_renderMap;
		std::vector<Chunk*>			_loadList;
		std::vector<index3D>		_unloadList;

		glm::vec3					_camPos;

		int							_maxDistWidth;
		int							_maxDistHeight;

		size_t						_totalActiveBlocks;
		size_t						_totalActiveChunks;
		bool						_isUnderGround;
		bool						_isAboveGround;

		void						updateLoadList();
		void						updateSetupList();
		void						updateUnloadList();
		void						updateVisibilityList();
		void						setRenderList(Camera &);

		std::pair<index3D, Chunk*>	initChunkAt(float, float, float);
		bool						shouldBeRendered(glm::vec3 &) const;
		void						setGroundFlags();
		void						setNeighbors(Chunk &);
		void						checkChunkDistance(Chunk &);

		ChunkManager();
};


#endif /* CHUNK_MANAGER_HPP */
