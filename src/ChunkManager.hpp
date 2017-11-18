/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:27:54 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/18 18:01:09 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_MANAGER_HPP
# define CHUNK_MANAGER_HPP

# include "constants.hpp"
# include "Chunk.hpp"
# include "Camera.hpp"
# include "BiomeManager.hpp"
# include "Frustum.hpp"
# include <map>
# include <future>

class ChunkManager
{
	public:
		ChunkManager();
		ChunkManager(glm::vec3);
		~ChunkManager();

		std::pair<index3D, Chunk*>	initChunkAt(float, float, float);

		void	render(Shader &);
		void	update(Camera &);
		
		void	updateLoadList();
		void	updateSetupList();
		void	updateUnloadList();
		void	updateVisibilityList();
		void	setRenderList(Camera &);
		std::map<float, Chunk*> & getRenderMap();

		void	checkChunkDistance(Chunk &);

		std::map<index3D, Chunk*> & getChunks();

		size_t	getTotalActiveBlocks() const;
		size_t	getTotalActiveChunks() const;

	private:
		std::map<index3D, Chunk*>	_chunkMap;
		std::map<float, Chunk*>		_setupMap;
		std::map<float, Chunk*>		_renderMap;

		std::vector<Chunk*>	_loadList;
		std::vector<index3D>	_unloadList;

		glm::vec3			_camPos;

		BiomeManager		bm;

		size_t		_totalActiveBlocks;
		size_t		_totalActiveChunks;
		bool		_isUnderGround;
		bool		_isAboveGround;

		bool		shouldBeRendered(glm::vec3 &) const;
};


#endif /* CHUNK_MANAGER_HPP */
