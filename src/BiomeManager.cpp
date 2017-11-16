/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BiomeManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/13 08:56:54 by thibautpier       #+#    #+#             */
/*   Updated: 2017/11/16 15:20:06 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BiomeManager.hpp"

BiomeManager::BiomeManager() {

	elevationFrequency = 0.002f;
    elevationNoise.SetNoiseType(FastNoise::Perlin);
    elevationNoise.SetFrequency(elevationFrequency);

	sharpnessFrequency = 0.001f;
    sharpnessNoise.SetNoiseType(FastNoise::Perlin);
    sharpnessNoise.SetFrequency(sharpnessFrequency);

    heightMapNoise.SetNoiseType(FastNoise::Perlin);

	maxElevationMult = 5.f;
	sharpnessDivider = 25.f;

	caveFreq = 0.09f;

    return;
}

BiomeManager::~BiomeManager() {
    return;
}

float			BiomeManager::getElevationMult(float x, float y) const {
    return ((BiomeManager::elevationNoise.GetNoise(x , y) + 1.f) / 2.f) * maxElevationMult; // 0 to 3
}

float			BiomeManager::getSharpness(float x, float y) const {
    return ((BiomeManager::sharpnessNoise.GetNoise(x , y) + 1.f) / 2.f) / sharpnessDivider;
}

float           BiomeManager::getHeightAt(float x, float y) {

	float elevation = getElevationMult(x, y);
	float sharpness = getSharpness(x, y);

    heightMapNoise.SetFrequency(sharpness);
    float	height = ((heightMapNoise.GetNoise(x , y) + 1)) * CHUNK_SIZE;

	return elevation * height;
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
				heightMapNoise.SetFrequency(caveFreq); // Set the desired noise freq
				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					float	density = (BiomeManager::heightMapNoise.GetNoise(chunkPos.x + x, chunkPos.z + z, chunkPos.y + y));
					if (density > 0.0f)
					{
						chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_STONE);
					}
				}
				
			}
			else if (inBetween)
			{
				float gradient = 0.0f;
				heightMapNoise.SetFrequency(caveFreq); // Set the desired noise freq
				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					float	density = (BiomeManager::heightMapNoise.GetNoise(chunkPos.x + x, chunkPos.z + z, chunkPos.y + y)) + gradient;
					if (density > 0.0f)
					{
						chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_STONE);
					}
					gradient += 0.75f / CHUNK_SIZE;
				}
			}
			else
			{
				float height = getHeightAt(chunkPos.x + x, chunkPos.z + z);
				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					if (chunkPos.y + y < height) {
						if (chunkPos.y + y < ROCK_LEVEL)
							chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_ROCK);
						else if (chunkPos.y + y < SAND_LEVEL)
							chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_SAND);
						else if (chunkPos.y + y > SNOW_LEVEL)
							chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_SNOW);
						else
							chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_STONE);

					}
					else if (chunkPos.y + y < WATER_LEVEL)
						chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_WATER);
					// else if (chunkPos.y + y < SNOW_LEVEL)
					// 	chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_SNOW);
				}
			}
		}
	}
}
