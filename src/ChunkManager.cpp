/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:29:47 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/02 14:37:46 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChunkManager.hpp"

ChunkManager::ChunkManager(glm::vec3 camPos) : _chunkMap(std::map<index3D, Chunk*>()) {

	Chunk::sNoise.SetNoiseType(FastNoise::Perlin); // Set the desired noise type
//	Chunk::sNoise.SetFrequency(0.04f); // Set the desired noise freq

	int	start = -(MAP_SIZE / 2);
	int end = MAP_SIZE / 2;
	for (int x = camPos.x + start; x < camPos.x + end; ++x)
	{
		for (int y = camPos.y + start; y < camPos.y + end; ++y)
		{
			for (int z = camPos.z + start; z < camPos.z + end; ++z)
			{
				float xPos = x * (CHUNK_SIZE * BLOCK_RENDER_SIZE);
				float yPos = y * (CHUNK_SIZE * BLOCK_RENDER_SIZE);
				float zPos = z * (CHUNK_SIZE * BLOCK_RENDER_SIZE);

				index3D		index(xPos, yPos, zPos);
				glm::vec3	chunkPos(xPos, yPos, zPos);

				_chunkMap.insert( std::pair<index3D, Chunk*>(index, new Chunk(chunkPos)) );
				_chunkMap.at(index)->setup();

		}
	}
}
std::cout << "ChunkManager successfully constructed" << std::endl;
}

ChunkManager::~ChunkManager() { }

void	ChunkManager::update(Shader & shader, Camera & camera) {

	glm::vec3	camPos = camera.getPosition();
	shader.use();
	shader.setVec3("lightPos", camPos.x, camPos.y, camPos.z);

	updateVisibilityList(camera);

	updateLoadList();

	updateUnloadList();

	updateRenderList(camera.getMatrix());

	render(shader);
}

void	ChunkManager::updateLoadList() {
	for (std::vector<Chunk*>::iterator it = _loadList.begin(); it != _loadList.end(); ++it)
	{
		Chunk *		chunk = *it;
		glm::vec3	chunkPos = chunk->getPosition();
		index3D		chunkIndex(chunkPos.x, chunkPos.y, chunkPos.z);

		_chunkMap.insert( std::pair<index3D, Chunk*>(chunkIndex, chunk) );
	}
	_loadList.clear();
}

void	ChunkManager::updateRenderList(glm::mat4 viewMatrix) {
	Frustum	f(Shader::perspective);
	f.setView(viewMatrix);
	f.setPlanes();

	_renderList.clear();

	float	halfChunk = CHUNK_SIZE * BLOCK_RENDER_SIZE / 2.0f; // This is improvable
	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it)
	{
		Chunk *		chunk = it->second;
		glm::vec3	pos = chunk->getPosition();
		if (f.pointIn(pos.x + halfChunk, pos.y + halfChunk, pos.z + halfChunk))
		{
			chunk->setVisibility(true);
			if (chunk->isSetup() == false)
			{
				chunk->setup();
			}
			if (pos.y < BLOCK_RENDER_SIZE * CHUNK_SIZE)
				_renderList.push_back(chunk);
		}
		else
		{
			chunk->setVisibility(false);
		}
	}
//		std::cout << _renderList.size() << std::endl;
}

void	ChunkManager::updateUnloadList() {
	for (std::vector<Chunk*>::iterator it = _unloadList.begin(); it != _unloadList.end(); ++it)
	{
		Chunk *		chunk = *it;
		glm::vec3	chunkPos = chunk->getPosition();
		index3D		chunkIndex(chunkPos.x, chunkPos.y, chunkPos.z);

		_chunkMap.erase(chunkIndex);
	}
	_unloadList.clear();
}

void	ChunkManager::updateVisibilityList(Camera & camera) {

	glm::vec3	camPos = camera.getPosition();
//	std::cout << "CAM " << camPos.x << " ; " << camPos.y << " ; " << camPos.z << std::endl;
	float		maxDist = ((BLOCK_RENDER_SIZE * CHUNK_SIZE) * (MAP_SIZE)) / 2.0f;

	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it)
	{
		index3D		index = it->first;
		glm::vec3	chunkPos = _chunkMap.at(index)->getPosition();

		glm::vec3	dist = chunkPos - camPos;

		if (fabs(dist.x) > maxDist)
		{
			glm::vec3	oppositePos = glm::vec3(chunkPos.x - maxDist * 2.0f * std::copysign(1.0f, dist.x), chunkPos.y, chunkPos.z);
			_loadList.push_back(new Chunk(oppositePos));
			_unloadList.push_back(it->second);
		}
		if (fabs(dist.z) > maxDist)
		{
			glm::vec3	oppositePos = glm::vec3(chunkPos.x, chunkPos.y, chunkPos.z - maxDist * 2.0f * std::copysign(1.0f, dist.z));
			_loadList.push_back(new Chunk(oppositePos));
			_unloadList.push_back(it->second);
		}
		if (fabs(dist.y) > maxDist)
		{
			glm::vec3	oppositePos = glm::vec3(chunkPos.x, chunkPos.y - maxDist * 2.0f * std::copysign(1.0f, dist.y), chunkPos.z);
			_loadList.push_back(new Chunk(oppositePos));
			_unloadList.push_back(it->second);
		}
	}
}

void	ChunkManager::render(Shader & shader) {

	//shader.use();
	shader.setView();

	_totalActiveBlocks = 0;
	_totalActiveChunks = 0;
	for (std::vector<Chunk*>::iterator it = _renderList.begin(); it != _renderList.end(); ++it)
	{
		(*it)->render();
		_totalActiveBlocks += (*it)->getActiveBlocks();
		_totalActiveChunks += 1;
	}
}

std::map<index3D, Chunk*> & ChunkManager::getChunks() {
	return _chunkMap;
}

size_t	ChunkManager::getTotalActiveBlocks() const { return _totalActiveBlocks; }
size_t	ChunkManager::getTotalActiveChunks() const { return _totalActiveChunks; }
