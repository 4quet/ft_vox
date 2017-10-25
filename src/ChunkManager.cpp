/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:29:47 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/25 11:20:14 by lfourque         ###   ########.fr       */
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

}

ChunkManager::~ChunkManager() { }

float	ChunkManager::getHeightMapValue(unsigned int x, unsigned int y) const {
	if (x < CHUNK_SIZE && y < CHUNK_SIZE)
		return _heightMap[x][y];
	return 0.0f;
}
