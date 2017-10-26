/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:29:47 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/26 14:15:30 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChunkManager.hpp"

ChunkManager::ChunkManager() {

	Chunk::sNoise.SetNoiseType(FastNoise::Perlin); // Set the desired noise type
	Chunk::sNoise.SetFrequency(0.04f); // Set the desired noise freq

	int maxSize = std::pow(MAP_SIZE, 3) + std::pow(MAP_SIZE, 2) + MAP_SIZE;
	_vChunks = std::vector<Chunk*>(maxSize);
	for (int x = 0; x < MAP_SIZE; ++x)
	{
		for (int y = 0; y < MAP_SIZE; ++y)
		{
			for (int z = 0; z < MAP_SIZE; ++z)
			{
				int index = x*(MAP_SIZE*MAP_SIZE) + y*MAP_SIZE + z;
				_vChunks[index] = new Chunk();
			}
		}	
	}

	for (int x = 0; x < MAP_SIZE; ++x)
	{
		for (int y = 0; y < MAP_SIZE; ++y)
		{
			for (int z = 0; z < MAP_SIZE; ++z)
			{
				float xPos = x * (CHUNK_SIZE * BLOCK_RENDER_SIZE);
				float yPos = y * (CHUNK_SIZE * BLOCK_RENDER_SIZE);
				float zPos = z * (CHUNK_SIZE * BLOCK_RENDER_SIZE);

				int index = x*(MAP_SIZE*MAP_SIZE) + y*MAP_SIZE + z;

				_vChunks[index]->setPosition(glm::vec3(xPos, yPos, zPos));
				_vChunks[index]->setHeightMap(x * CHUNK_SIZE, y * CHUNK_SIZE, z * CHUNK_SIZE);
				_vChunks[index]->setupLandscape();
				_vChunks[index]->createMesh();

				// Add active blocks / chunks to total
				size_t	toAdd = _vChunks[index]->getActiveBlocks();
				if (toAdd > 0)
				{
					_totalActiveBlocks += toAdd;
					_totalActiveChunks += 1;
				}
			}
		}
	}
}

ChunkManager::~ChunkManager() { }

void	ChunkManager::update(Shader & shader) {
	render(shader);
}


void	ChunkManager::render(Shader & shader) {

	shader.use();
	shader.setView();

	for (int x = 0; x < MAP_SIZE; ++x)
	{
		for (int y = 0; y < MAP_SIZE; ++y)
		{
			for (int z = 0; z < MAP_SIZE; ++z)
			{
				int index = x*(MAP_SIZE*MAP_SIZE) + y*MAP_SIZE + z;
				_vChunks[index]->render();
			}
		}
	}
}

size_t	ChunkManager::getTotalActiveBlocks() const { return _totalActiveBlocks; }
size_t	ChunkManager::getTotalActiveChunks() const { return _totalActiveChunks; }
