/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:29:47 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/26 11:45:36 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChunkManager.hpp"

ChunkManager::ChunkManager() {

	Chunk::sNoise.SetNoiseType(FastNoise::Perlin); // Set the desired noise type
	Chunk::sNoise.SetFrequency(0.04f); // Set the desired noise freq

	// 3D array of chunks
	_chunks = new Chunk**[MAP_SIZE];
	for (int i = 0; i < MAP_SIZE; ++i)
	{
		_chunks[i] = new Chunk*[MAP_SIZE];
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			_chunks[i][j] = new Chunk[MAP_SIZE];
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

				_chunks[x][y][z].setPosition(glm::vec3(xPos, yPos, zPos));

				_chunks[x][y][z].setHeightMap(x * CHUNK_SIZE, y * CHUNK_SIZE, z * CHUNK_SIZE);

				_chunks[x][y][z].setupLandscape();
				_chunks[x][y][z].createMesh();
				
				// Add active blocks / chunks to total
				size_t	toAdd = _chunks[x][y][z].getActiveBlocks();
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

void	ChunkManager::render(Shader & shader) {

	shader.use();
	shader.setView();

	for (int x = 0; x < MAP_SIZE; ++x)
	{
		for (int y = 0; y < MAP_SIZE; ++y)
		{
			for (int z = 0; z < MAP_SIZE; ++z)
				_chunks[x][y][z].render();
		}
	}
}

size_t	ChunkManager::getTotalActiveBlocks() const { return _totalActiveBlocks; }
size_t	ChunkManager::getTotalActiveChunks() const { return _totalActiveChunks; }
