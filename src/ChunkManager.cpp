/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:29:47 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/06 18:27:57 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChunkManager.hpp"

std::mutex	ChunkManager::mutex;

ChunkManager::ChunkManager(glm::vec3 camPos) :
	_chunkMap(), 
	_query(GL_ANY_SAMPLES_PASSED) {

	Chunk::sNoise.SetNoiseType(FastNoise::Perlin); // Set the desired noise type

	int	start = -(MAP_SIZE / 2);
	int end = MAP_SIZE / 2;
	for (int x = camPos.x + start; x < camPos.x + end; ++x)
	{
		for (int y = camPos.y + start; y < camPos.y + end; ++y)
		{
			for (int z = camPos.z + start; z < camPos.z + end; ++z)
			{
				initChunkAt(x, y, z);
			}
		}
	}
}

ChunkManager::~ChunkManager() { }

void	ChunkManager::initChunkAt(float x, float y, float z) {
	float	chunkRenderSize = CHUNK_SIZE * BLOCK_RENDER_SIZE;
	float	xPos = x * chunkRenderSize;
	float	yPos = y * chunkRenderSize;
	float	zPos = z * chunkRenderSize;

	index3D		index(xPos, yPos, zPos);
	glm::vec3	chunkPos(xPos, yPos, zPos);

	_chunkMap.insert( std::pair<index3D, Chunk*>(index, new Chunk(chunkPos)) );
	_chunkMap.at(index)->setup();
}

void	ChunkManager::update(Shader & shader, Camera & camera) {

	glm::vec3	camPos = camera.getPosition();

	shader.use();
	shader.setView();
	shader.setVec3("lightPos", camPos.x, camPos.y, camPos.z);

	updateVisibilityList(camera);

	updateLoadList();

	updateUnloadList();

	setRenderList(camera);

	render();

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

Chunk *	ChunkManager::setupChunkInFrustum(Frustum & frustum, Chunk & chunk) {

	float		halfChunk = CHUNK_SIZE * BLOCK_RENDER_SIZE / 2.0f;
	glm::vec3	pos = chunk.getPosition();

	if (frustum.pointIn(pos.x + halfChunk, pos.y + halfChunk, pos.z + halfChunk))
	{
		if (chunk.isSetup() == false)
			chunk.setup();
		if (pos.y < BLOCK_RENDER_SIZE * CHUNK_SIZE) // This probably shouldn't be here
			return &chunk;
	}
	return NULL;
}

void	ChunkManager::setRenderList(Camera & camera) {
	Frustum	f(Shader::perspective);
	f.setView(camera.getMatrix());
	f.setPlanes();

	_renderList.clear();

	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it)
	{
		Chunk *		chunk = it->second;

		_renderList.push_back( setupChunkInFrustum(f, *chunk) );
	}
}

void	ChunkManager::updateUnloadList() {
	for (std::vector<Chunk*>::iterator it = _unloadList.begin(); it != _unloadList.end(); ++it)
	{
		Chunk *		chunk = *it;
		glm::vec3	chunkPos = chunk->getPosition();
		index3D		chunkIndex(chunkPos.x, chunkPos.y, chunkPos.z);

		try {
			delete _chunkMap.at(chunkIndex);
		}
		catch (std::exception & e)
		{
			std::cout << e.what() << ": " << chunkPos.x << ", " << chunkPos.y << ", " << chunkPos.z <<  std::endl;
		}
		_chunkMap.erase(chunkIndex);
	}
	_unloadList.clear();
}

void	ChunkManager::checkChunkDistance(glm::vec3 camPos, Chunk & chunk) {

	float		maxDist = ((BLOCK_RENDER_SIZE * CHUNK_SIZE) * (MAP_SIZE)) / 2.0f;
	glm::vec3	chunkPos = chunk.getPosition();
	glm::vec3	dist = chunkPos - camPos;

	bool		b = false;
	glm::vec3	oppositePos;

	if (fabs(dist.x) > maxDist)
	{
		oppositePos = glm::vec3(chunkPos.x - maxDist * 2.0f * std::copysign(1.0f, dist.x), chunkPos.y, chunkPos.z);
		b = true;
	}
	else if (fabs(dist.z) > maxDist)
	{
		oppositePos = glm::vec3(chunkPos.x, chunkPos.y, chunkPos.z - maxDist * 2.0f * std::copysign(1.0f, dist.z));
		b = true;
	}
	else if (fabs(dist.y) > maxDist)
	{
		oppositePos = glm::vec3(chunkPos.x, chunkPos.y - maxDist * 2.0f * std::copysign(1.0f, dist.y), chunkPos.z);
		b = true;
	}

	if (b)
	{
		_loadList.push_back(new Chunk(oppositePos));
		_unloadList.push_back(&chunk);
	}
}

void	ChunkManager::updateVisibilityList(Camera & camera) {

	glm::vec3	camPos = camera.getPosition();

	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it)
	{
		checkChunkDistance(camPos, *(it->second));
	}
}

bool	ChunkManager::isOccluded(Chunk * chunk) {
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	_query.start();
		chunk->render();
	_query.end();
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	return !_query.getResult();
}

void	ChunkManager::render() {

	_totalActiveBlocks = 0;
	_totalActiveChunks = _renderList.size();
	for (std::vector<Chunk*>::iterator it = _renderList.begin(); it != _renderList.end(); ++it)
	{
		Chunk *	chunk = (*it);
		if (chunk != NULL)
		{
			if (chunk->isBuilt() == false)
				chunk->buildMesh();
			chunk->render();
			_totalActiveBlocks += chunk->getActiveBlocks();
		}
	}
}

std::map<index3D, Chunk*> & ChunkManager::getChunks() {
	return _chunkMap;
}

size_t	ChunkManager::getTotalActiveBlocks() const { return _totalActiveBlocks; }
size_t	ChunkManager::getTotalActiveChunks() const { return _totalActiveChunks; }
