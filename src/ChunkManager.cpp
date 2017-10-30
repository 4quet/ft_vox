/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:29:47 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/30 18:55:49 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChunkManager.hpp"

ChunkManager::ChunkManager(glm::vec3 camPos) : _chunkMap(std::map<index3D, Chunk*>()) {

	Chunk::sNoise.SetNoiseType(FastNoise::Perlin); // Set the desired noise type
	Chunk::sNoise.SetFrequency(0.04f); // Set the desired noise freq

	int	start = -(MAP_SIZE / 2);
	int end = MAP_SIZE / 2;
	for (int x = camPos.x + start; x < camPos.x + end; ++x)
	{
		for (int y = camPos.y + start; y < camPos.y + end; ++y)
		{
			for (int z = camPos.z + start; z < camPos.z + end; ++z)
			{
				float xPos = x * (CHUNK_SIZE * BLOCK_RENDER_SIZE);
				float yPos = y * (CHUNK_SIZE * BLOCK_RENDER_SIZE);
				float zPos = z * (CHUNK_SIZE * BLOCK_RENDER_SIZE);

				if (yPos <= -BLOCK_RENDER_SIZE * (CHUNK_SIZE))
				{
					index3D	index(xPos, yPos, zPos);

					_chunkMap.insert( std::pair<index3D, Chunk*>(index, new Chunk()) );

					_chunkMap.at(index)->setPosition(glm::vec3(xPos, yPos, zPos));
					_chunkMap.at(index)->setHeightMap(x * CHUNK_SIZE, y * CHUNK_SIZE, z * CHUNK_SIZE);
					_chunkMap.at(index)->setupLandscape();
					_chunkMap.at(index)->createMesh();

					// Add active blocks / chunks to total
					size_t	toAdd = _chunkMap.at(index)->getActiveBlocks();
					if (toAdd > 0)
					{
						_totalActiveBlocks += toAdd;
						_totalActiveChunks += 1;
					}
				}
			}
		}
	}
	std::cout << "ChunkManager successfully constructed" << std::endl;
}

ChunkManager::~ChunkManager() { }

void	ChunkManager::update(Shader & shader, Camera & camera) {
	Frustum	f(Shader::perspective);

	f.compute(camera.getMatrix(), _chunkMap);

	updateVisibilityList(camera);

//	f.compute(camera.getMatrix(), _loadList);

	updateLoadList();
	updateUnloadList();

	render(shader);
}

void	ChunkManager::updateLoadList() {
	for (std::vector<Chunk*>::iterator it = _loadList.begin(); it != _loadList.end(); ++it)
	{
		Chunk *		chunk = *it;
		if (chunk->isVisible())
		{
			glm::vec3	chunkPos = chunk->getPosition();
			index3D		chunkIndex(chunkPos.x, chunkPos.y, chunkPos.z);

			chunk->setHeightMap(chunkPos.x * CHUNK_SIZE, chunkPos.y * CHUNK_SIZE, chunkPos.z * CHUNK_SIZE);
			chunk->setupLandscape();
			chunk->createMesh();

			_chunkMap.insert( std::pair<index3D, Chunk*>(chunkIndex, chunk) );
		}

	}
	_loadList.clear();
}

void	ChunkManager::updateSetupList() {
	//...
}

void	ChunkManager::updateUnloadList() {
	for (std::vector<Chunk*>::iterator it = _unloadList.begin(); it != _unloadList.end(); ++it)
	{
		Chunk *		chunk = *it;
		glm::vec3	chunkPos = chunk->getPosition();
		index3D		chunkIndex(chunkPos.x, chunkPos.y, chunkPos.z);

		_chunkMap.erase(chunkIndex);
	}
	_unloadList.clear();
}

void	ChunkManager::updateVisibilityList(Camera & camera) {

	glm::vec3	camPos = camera.getPosition();
//	std::cout << "CAM " << camPos.x << " ; " << camPos.y << " ; " << camPos.z << std::endl;
	float		maxDist = ((BLOCK_RENDER_SIZE * CHUNK_SIZE) * (MAP_SIZE)) / 2.0f;

	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it)
	{
		index3D		index = it->first;
		glm::vec3	chunkPos = _chunkMap.at(index)->getPosition();

		glm::vec3	dist = chunkPos - camPos;

		if (fabs(dist.x) > maxDist)
		{
			glm::vec3	oppositePos;
			if (chunkPos.x > camPos.x)
				oppositePos = glm::vec3(chunkPos.x - (maxDist * 2.0f), chunkPos.y, chunkPos.z);
			else
				oppositePos = glm::vec3(chunkPos.x + (maxDist * 2.0f), chunkPos.y, chunkPos.z);

			_loadList.push_back(new Chunk(oppositePos));
			_unloadList.push_back(it->second);
		}
		if (fabs(dist.z) > maxDist)
		{
			glm::vec3	oppositePos;
			if (chunkPos.z > camPos.z)
				oppositePos = glm::vec3(chunkPos.x, chunkPos.y, chunkPos.z - (maxDist * 2.0f));
			else
				oppositePos = glm::vec3(chunkPos.x, chunkPos.y, chunkPos.z + (maxDist * 2.0f));

			_loadList.push_back(new Chunk(oppositePos));
			_unloadList.push_back(it->second);
		}
	}
}

void	ChunkManager::render(Shader & shader) {

	shader.use();
	shader.setView();

	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it)
	{
		index3D index = it->first;
		if(_chunkMap.at(index)->isVisible())
			_chunkMap.at(index)->render();
	}
}

std::map<std::tuple<float, float, float>, Chunk*> & ChunkManager::getChunks() {
	return _chunkMap;
}

size_t	ChunkManager::getTotalActiveBlocks() const { return _totalActiveBlocks; }
size_t	ChunkManager::getTotalActiveChunks() const { return _totalActiveChunks; }
