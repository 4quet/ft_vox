/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:27:54 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/06 18:05:26 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_MANAGER_HPP
# define CHUNK_MANAGER_HPP

# include "constants.hpp"
# include "Chunk.hpp"
# include "Camera.hpp"
# include "Frustum.hpp"
# include "Query.hpp"
# include <map>
# include <future>

class ChunkManager
{
	public:
		ChunkManager();
		ChunkManager(glm::vec3);
		~ChunkManager();

		void	initChunkAt(float, float, float);

		void	render();

		void	update(Shader &, Camera &);
		
		void	updateLoadList();
		void	updateUnloadList();
		void	updateVisibilityList(Camera &);
		void	setRenderList(Camera &);

		Chunk *	setupChunkInFrustum(Frustum &, Chunk &);
		void	checkChunkDistance(glm::vec3, Chunk &);

		std::map<index3D, Chunk*> & getChunks();

		size_t	getTotalActiveBlocks() const;
		size_t	getTotalActiveChunks() const;

	private:
		std::map<index3D, Chunk*>	_chunkMap;

		std::vector<Chunk*>	_loadList;
		std::vector<Chunk*>	_renderList;
		std::vector<Chunk*>	_unloadList;

		std::vector<std::future<Chunk*>>	_renderListAsync;

		size_t		_totalActiveBlocks;
		size_t		_totalActiveChunks;

		bool		isOccluded(Chunk *);

		Query		_query;

		static		std::mutex	mutex;
};


#endif /* CHUNK_MANAGER_HPP */
