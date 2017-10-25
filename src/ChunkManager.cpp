/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:29:47 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/25 17:32:10 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChunkManager.hpp"

ChunkManager::ChunkManager() {

	Chunk::sNoise.SetNoiseType(FastNoise::Perlin); // Set the desired noise type
	Chunk::sNoise.SetFrequency(0.04f); // Set the desired noise freq

	_chunks = new Chunk*[MAP_SIZE];
	for (int x = 0; x < MAP_SIZE; ++x)
	{
		_chunks[x] = new Chunk[MAP_SIZE];
	}

	for (int x = 0; x < MAP_SIZE; ++x)
	{
		for (int y = 0; y < MAP_SIZE; ++y)
		{
			float xPos = x * (CHUNK_SIZE * BLOCK_RENDER_SIZE);
			float yPos = y * (CHUNK_SIZE * BLOCK_RENDER_SIZE);
			_chunks[x][y].setPosition(glm::vec3(xPos, 0, yPos));

			_chunks[x][y].setHeightMap(x * CHUNK_SIZE, y * CHUNK_SIZE);

			_chunks[x][y].setupLandscape();
			_chunks[x][y].createMesh();
			
			// Add active blocks / chunks to total
			size_t	toAdd = _chunks[x][y].getActiveBlocks();
			if (toAdd > 0)
			{
				_totalActiveBlocks += toAdd;
				_totalActiveChunks += 1;
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
			_chunks[x][y].render();
		}
	}
}

size_t	ChunkManager::getTotalActiveBlocks() const { return _totalActiveBlocks; }
size_t	ChunkManager::getTotalActiveChunks() const { return _totalActiveChunks; }
