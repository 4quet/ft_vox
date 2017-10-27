/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:29:47 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/27 16:55:43 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChunkManager.hpp"

ChunkManager::ChunkManager() : _chunkMap(std::map<index3D, Chunk*>()) {

	Chunk::sNoise.SetNoiseType(FastNoise::Perlin); // Set the desired noise type
	Chunk::sNoise.SetFrequency(0.04f); // Set the desired noise freq

	/*
	   for (int x = 0; x < MAP_SIZE; ++x)
	   {
	   for (int y = 0; y < MAP_SIZE; ++y)
	   {
	   for (int z = 0; z < MAP_SIZE; ++z)
	   {
	   _chunkMap.insert( std::pair<index3D, Chunk*>(index3D(x, y, z), new Chunk()) );
	   }
	   }	
	   }
	   */

	for (int x = 0; x < MAP_SIZE; ++x)
	{
		for (int y = 0; y < MAP_SIZE; ++y)
		{
			for (int z = 0; z < MAP_SIZE; ++z)
			{
				float xPos = x * (CHUNK_SIZE * BLOCK_RENDER_SIZE);
				float yPos = y * (CHUNK_SIZE * BLOCK_RENDER_SIZE);
				float zPos = z * (CHUNK_SIZE * BLOCK_RENDER_SIZE);

				//index3D	index(x, y, z);
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
	std::cout << "ChunkManager successfully constructed" << std::endl;
}

ChunkManager::~ChunkManager() { }

void	ChunkManager::update(Shader & shader, Camera & camera) {
	(void)camera;
	updateVisibilityList(camera);
	render(shader);
}

void	ChunkManager::updateLoadList() {
	//...
}

void	ChunkManager::updateSetupList() {
	//...
}

void	ChunkManager::updateUnloadList() {
	//...
}

void	ChunkManager::updateVisibilityList(Camera & camera) {

	glm::vec3	camPos = camera.getPosition();
	float		maxDist = (BLOCK_RENDER_SIZE * CHUNK_SIZE) * (MAP_SIZE);

//	std::cout << "camPos: " << camPos.x << ";" << camPos.y << ";" << camPos.z << std::endl;
//	std::cout << maxDist << std::endl;

	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it)
	{
		index3D		index = it->first;

		glm::vec3	chunkPos = _chunkMap.at(index)->getPosition();

//		index3D	chunkWorldCoord(chunkPos.x, chunkPos.y, chunkPos.z);

		glm::vec3	oppositePos(chunkPos.x + MAP_SIZE, chunkPos.y, chunkPos.z);
		index3D		oppositeChunk(chunkPos.x + MAP_SIZE, chunkPos.y, chunkPos.z);

		if (camPos.x - chunkPos.x > maxDist)
		{
		//	_chunkMap.erase(index);
			if (_chunkMap.find(oppositeChunk) == _chunkMap.end())
			{
				std::cout << "offseting... ";
				_chunkMap.insert( std::pair<index3D, Chunk*>(oppositeChunk, new Chunk()) );

				_chunkMap.at(oppositeChunk)->setPosition(glm::vec3(oppositePos));
				_chunkMap.at(oppositeChunk)->setHeightMap(oppositePos.x * CHUNK_SIZE, oppositePos.y * CHUNK_SIZE, oppositePos.z * CHUNK_SIZE);
	
				_chunkMap.at(oppositeChunk)->setupLandscape();
				_chunkMap.at(oppositeChunk)->createMesh();
				std::cout << "done" << std::endl;
			}
		}
	}
}

void	ChunkManager::render(Shader & shader) {

	shader.use();
	shader.setView();

	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it)
	{
		index3D index = it->first;
		(void)index;
		_chunkMap.at(index)->render();
	}
	/*
	   for (int x = 0; x < MAP_SIZE; ++x)
	   {
	   for (int y = 0; y < MAP_SIZE; ++y)
	   {
	   for (int z = 0; z < MAP_SIZE; ++z)
	   {
	   index3D	index(x, y, z);
	   _chunkMap.at(index)->render();
	   }
	   }
	   }
	   */
}

size_t	ChunkManager::getTotalActiveBlocks() const { return _totalActiveBlocks; }
size_t	ChunkManager::getTotalActiveChunks() const { return _totalActiveChunks; }
