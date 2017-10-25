/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:29:47 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/25 14:56:53 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChunkManager.hpp"

ChunkManager::ChunkManager() {

	FastNoise myNoise; // Create a FastNoise object
	myNoise.SetNoiseType(FastNoise::Perlin); // Set the desired noise type
	myNoise.SetFrequency(0.015f); // Set the desired noise freq

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			_heightMap[x][y] = myNoise.GetNoise(x,y);
		}
	}

	_chunks = new Chunk*[2];
	for (int x = 0; x < 2; ++x)
	{
		_chunks[x] = new Chunk[2];
	}

	for (int x = 0; x < 2; ++x)
	{
		for (int y = 0; y < 2; ++y)
		{
			_chunks[x][y].setPosition(glm::vec3(x * (CHUNK_SIZE * BLOCK_RENDER_SIZE), 0, y * (CHUNK_SIZE * BLOCK_RENDER_SIZE)));
			_chunks[x][y].setHeightMap(_heightMap);
			_chunks[x][y].setupLandscape();
			_chunks[x][y].createMesh();
		}
	}
}

ChunkManager::~ChunkManager() { }

void	ChunkManager::render(Shader & shader) {

	for (int x = 0; x < 2; ++x)
	{
		for (int y = 0; y < 2; ++y)
		{
			_chunks[x][y].render(shader);
		}
	}

}

float	ChunkManager::getHeightMapValue(unsigned int x, unsigned int y) const {
	if (x < CHUNK_SIZE && y < CHUNK_SIZE)
		return _heightMap[x][y];
	return 0.0f;
}
