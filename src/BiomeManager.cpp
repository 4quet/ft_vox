/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BiomeManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/13 08:56:54 by thibautpier       #+#    #+#             */
/*   Updated: 2017/11/13 09:34:17 by thibautpier      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BiomeManager.hpp"

FastNoise	BiomeManager::sNoise;

BiomeManager::BiomeManager() {
    BiomeManager::sNoise.SetNoiseType(FastNoise::Perlin);
    return;
}

BiomeManager::~BiomeManager() {
    return;
}

float           BiomeManager::getHeightAt(float x, float y) const {
    Biomes::Enum biome = getBiomeAt(x, y);

    if (biome == Biomes::FIELD)
        return getFieldHeightAt(x, y);
    else
        return getDesertHeightAt(x, y);
}

Biomes::Enum    BiomeManager::getBiomeAt(float x, float y) const {
    BiomeManager::sNoise.SetFrequency(0.001);
    float	hm = (BiomeManager::sNoise.GetNoise(x , y) + 1) / 2;
    if (hm < 0.5f)
        return Biomes::FIELD;
    else
        return Biomes::DESERT;
}

float           BiomeManager::getFieldHeightAt(float x, float y) const {
    BiomeManager::sNoise.SetFrequency(0.05);
    float	hm = (BiomeManager::sNoise.GetNoise(x, y) + 1) / 2 * (CHUNK_SIZE * 2);
    return hm;
}

float           BiomeManager::getDesertHeightAt(float x, float y) const {
    BiomeManager::sNoise.SetFrequency(0.01);
    float	hm = (BiomeManager::sNoise.GetNoise(x, y) + 1) / 2 * (CHUNK_SIZE * 2);
    return hm;
}

float           BiomeManager::getSnowHeightAt(float x, float y) const {
    return x + y;
}

float           BiomeManager::getCanyonHeightAt(float x, float y) const {
    return x + y;
}
