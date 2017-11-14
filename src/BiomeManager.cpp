/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BiomeManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/13 08:56:54 by thibautpier       #+#    #+#             */
/*   Updated: 2017/11/14 19:34:35 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BiomeManager.hpp"

FastNoise	BiomeManager::sNoise;

BiomeManager::BiomeManager() {
    BiomeManager::sNoise.SetNoiseType(FastNoise::Perlin);

	biomeFreqSelector = 0.001f;

	fieldFreq = 0.04f;
	desertFreq = 0.01f;
	caveFreq = 0.09f;

	fieldMaxAltitude = 3.f;
	desertMaxAltitude = 2.0f;

    return;
}

BiomeManager::~BiomeManager() {
    return;
}

float           BiomeManager::getHeightAt(float x, float y, Biomes::Enum biome) const {
    if (biome == Biomes::FIELD)
        return getFieldHeightAt(x, y);
    else
        return getDesertHeightAt(x, y);
}

Biomes::Enum    BiomeManager::getBiomeAt(float x, float y) const {
    BiomeManager::sNoise.SetFrequency(biomeFreqSelector);
    float	hm = (BiomeManager::sNoise.GetNoise(x , y) + 1) / 2;
    if (hm < 0.5f)
        return Biomes::FIELD;
    else
        return Biomes::DESERT;
}

float           BiomeManager::getFieldHeightAt(float x, float y) const {
    BiomeManager::sNoise.SetFrequency(fieldFreq);
    return ((BiomeManager::sNoise.GetNoise(x, y) + 1) / 2) * (CHUNK_SIZE * fieldMaxAltitude);
}

float           BiomeManager::getDesertHeightAt(float x, float y) const {
    BiomeManager::sNoise.SetFrequency(desertFreq);
    return ((BiomeManager::sNoise.GetNoise(x, y) + 1) / 2) * (CHUNK_SIZE * desertMaxAltitude);
}

float           BiomeManager::getSnowHeightAt(float x, float y) const {
    return x + y;
}

float           BiomeManager::getCanyonHeightAt(float x, float y) const {
    return x + y;
}

void	BiomeManager::setupLandscape(Chunk & chunk) {

	glm::vec3	chunkPos = chunk.getPosition();
	bool hasCave = (chunkPos.y <= -GROUND_LEVEL);
	bool inBetween = (chunkPos.y > -GROUND_LEVEL && chunkPos.y < GROUND_LEVEL);

	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int z = 0; z < CHUNK_SIZE; ++z)
		{
			if (hasCave)
			{
				BiomeManager::sNoise.SetFrequency(caveFreq); // Set the desired noise freq
				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					float	density = (BiomeManager::sNoise.GetNoise(chunkPos.x + x, chunkPos.z + z, chunkPos.y + y));
					if (density > 0.0f)
					{
						chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_STONE);
					}
				}
				
			}
			else if (inBetween)
			{
				float gradient = 0.0f;
				BiomeManager::sNoise.SetFrequency(caveFreq); // Set the desired noise freq
				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					float	density = (BiomeManager::sNoise.GetNoise(chunkPos.x + x, chunkPos.z + z, chunkPos.y + y)) + gradient;
					if (density > 0.0f) //&& height > 0.0f)
					{
						chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_STONE);
					}
					gradient += 0.75f / CHUNK_SIZE;
				}
			}
			else
			{
				Biomes::Enum	biome = getBiomeAt(chunkPos.x + x, chunkPos.z + z);
				float	height = getHeightAt(chunkPos.x + x, chunkPos.z + z, biome);

				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					if (chunkPos.y + y < height)
					{
						chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_STONE);
					}
				//	else if (chunkPos.y + y < WATER_LEVEL)
				//		_blocks[x][y][z].setBlockType(BLOCKTYPE_WATER);
				}
			}
		}
	}
}
