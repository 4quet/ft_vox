/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BiomeManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/13 08:56:54 by thibautpier       #+#    #+#             */
/*   Updated: 2017/11/20 15:30:05 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BiomeManager.hpp"

BiomeManager::BiomeManager() {

	elevationFreq = 0.003f;
    elevationNoise.SetNoiseType(FastNoise::Perlin);
    elevationNoise.SetFrequency(elevationFreq);

	surfaceFreq = 0.025f;
    heightMapNoise.SetNoiseType(FastNoise::Perlin);
    heightMapNoise.SetFrequency(surfaceFreq);

	caveFreq = 0.09f;
    caveNoise.SetNoiseType(FastNoise::Perlin);
    caveNoise.SetFrequency(caveFreq);

	moistureFreq = 0.01f;
    moistureNoise.SetNoiseType(FastNoise::Perlin);
    moistureNoise.SetFrequency(moistureFreq);

	offsetFreq = 0.015f;
    offsetNoise.SetNoiseType(FastNoise::Perlin);
    offsetNoise.SetFrequency(offsetFreq);

	elevationMultiplier = 1.55f;

    return;
}

BiomeManager::~BiomeManager() {
    return;
}

float			BiomeManager::getElevation(float x, float y) {
    return ((elevationNoise.GetNoise(x , y) + 1.f) / 2.f) * elevationMultiplier;
}

float			BiomeManager::getOffset(float x, float y) {
    return ((offsetNoise.GetNoise(x , y) + 1.f) / 2.f) * CHUNK_RENDER_SIZE;
}

float			BiomeManager::getMoisture(float x, float y) {
    return ((moistureNoise.GetNoise(x , y) + 1.f) / 2.f);
}

float           BiomeManager::getHeightAt(float x, float y) {

	float exponent = getElevation(x, y);
	float offset = getOffset(x, y);

    float	height = ((heightMapNoise.GetNoise(x , y) + 1)) * CHUNK_SIZE
		+ 0.5f * ((heightMapNoise.GetNoise(x * 2 , y * 2) + 1)) * CHUNK_SIZE
		+ 0.25f * ((heightMapNoise.GetNoise(x * 4, y * 4) + 1)) * CHUNK_SIZE;

	return pow(height, exponent) + offset;
}

void	BiomeManager::setupLandscape(Chunk & chunk) {

	glm::vec3	chunkPos = chunk.getPosition();
	bool hasCave = (chunkPos.y < CAVE_LEVEL);
	bool inBetween = (chunkPos.y >= CAVE_LEVEL && chunkPos.y < GROUND_LEVEL);

	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int z = 0; z < CHUNK_SIZE; ++z)
		{
			if (hasCave)
			{
				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					float	density = (caveNoise.GetNoise(chunkPos.x + x, chunkPos.z + z, chunkPos.y + y));
					if (density > 0.0f)
					{
						chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_DIRT);
					}
					else
						chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_INACTIVE);
				}
				
			}
			else if (inBetween)
			{
				float gradient = 0.0f;
				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					float	density = (caveNoise.GetNoise(chunkPos.x + x, chunkPos.z + z, chunkPos.y + y)) + gradient;
					if (density > 0.0f)
					{
						chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_DIRT);
					}
					else
						chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_INACTIVE);
					gradient += 0.75f / CHUNK_SIZE;
				}
			}
			else
			{
				float height = getHeightAt(chunkPos.x + x, chunkPos.z + z);
				float m = getMoisture(chunkPos.x + x, chunkPos.z + z);
				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					if (chunkPos.y + y < height) {

						if (height < SAND_LEVEL)
							chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_SAND);
						
						else if (height < ROCK_LEVEL)
						{
							if (m < 0.33f) 
								chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_SAND);
							else if (m < 0.66f) 
								chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_DIRT);
							else
								chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_ROCK);
						}

						else if (height < SNOW_LEVEL)
						{
							if (m < 0.5f) 
								chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_ROCK);
							else
								chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_SNOW);
						}
						else
							chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_SNOW);

					}
					else if (chunkPos.y + y < WATER_LEVEL)
						chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_WATER);
					else
						chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_INACTIVE);
				}
			}
		}
	}
}
