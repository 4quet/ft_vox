/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:29:47 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/13 15:28:30 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChunkManager.hpp"
#include <chrono>

std::mutex	ChunkManager::mutex;

ChunkManager::ChunkManager(glm::vec3 camPos) :
	_chunkMap(),
	_query(GL_ANY_SAMPLES_PASSED) {

	Chunk::loadTexturesAtlas("./textures/texturesLowRes.png");
	Chunk::setUVs(2, 2, 3);
	
	Chunk::sNoise.SetNoiseType(FastNoise::Perlin); // Set the desired noise type

	std::vector<std::future<std::pair<index3D, Chunk*>>>	futures;

	int	start = -(MAP_SIZE / 2);
	int end = MAP_SIZE / 2;
	for (int x = camPos.x + start; x < camPos.x + end; ++x)
	{
		for (int y = camPos.y + start; y < camPos.y + end; ++y)
		{
			for (int z = camPos.z + start; z < camPos.z + end; ++z)
			{
				futures.emplace_back( std::async( &ChunkManager::initChunkAt, this, x, y, z ) );
			}
		}
	}

	for (std::vector<std::future<std::pair<index3D, Chunk*>>>::iterator it = futures.begin(); it != futures.end(); ++it)
	{
		std::pair<index3D, Chunk*>	p = (*it).get();

		if (p.second != NULL)
		{
			_chunkMap.insert(p);
		}
	}
}

ChunkManager::~ChunkManager() { }

std::pair<index3D, Chunk*>	ChunkManager::initChunkAt(float xx, float yy, float zz) {
	float	chunkRenderSize = CHUNK_SIZE * BLOCK_RENDER_SIZE;
	float	xPos = xx * chunkRenderSize;
	float	yPos = yy * chunkRenderSize;
	float	zPos = zz * chunkRenderSize;
	if (yPos > -WORLD_BOTTOM * CHUNK_SIZE && yPos < CHUNK_SIZE * MAX_ALTITUDE)
	{
		Chunk *		chunk = new Chunk(glm::vec3(xPos, yPos, zPos));
		chunk->setup();
		return std::pair<index3D, Chunk*>(index3D(xPos, yPos, zPos), chunk);
	}
	else
	{
		return std::pair<index3D, Chunk*>(index3D(0, 0, 0), NULL);
	}
}

void	ChunkManager::update(Camera & camera) {

	glm::vec3	camPos = camera.getPosition();

	updateVisibilityList(camPos);

	//std::async( std::launch::async, &ChunkManager::updateLoadList, this );
	updateLoadList();

	if (_setupMap.size() > 0)
	{
		updateSetupList();
	}

	//std::async( std::launch::async, &ChunkManager::updateUnloadList, this );
	updateUnloadList();

	//updateLoadList();
	//updateUnloadList();

	setRenderList(camera);

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

void	ChunkManager::updateSetupList() {
	//std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	int	setupThisFrame = 0;

	for (std::map<float, Chunk*>::iterator it = _setupMap.begin(); it != _setupMap.end(); ++it)
	{
		if (setupThisFrame >= MAX_CHUNK_SETUP_PER_FRAME)
		{
			break;
		}
		Chunk *		chunk = it->second;
		chunk->setup();
		setupThisFrame++;
	}

	_setupMap.clear();

	/*
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

	if (duration)
		std::cout << " duration: " << duration << " ms - " << setupThisFrame << " setup this frame" << std::endl;
		*/
}


void	ChunkManager::setRenderList(Camera & camera) {

	Frustum	frustum(Shader::perspective);
	frustum.setView(camera.getMatrix());
	frustum.setPlanes();

	glm::vec3	camPos = camera.getPosition();

	float		halfChunk = CHUNK_SIZE * BLOCK_RENDER_SIZE / 2.0f;
	int			setupThisFrame = 0;

	_renderList.clear();

	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it)
	{
		Chunk *		chunk = it->second;
		glm::vec3	pos = chunk->getPosition();

		if (frustum.pointIn(pos.x + halfChunk, pos.y + halfChunk, pos.z + halfChunk))
		{
			_renderList.push_back(chunk);
		}

		// this may move
		if (chunk->isSetup() == false)
		{
			_setupMap.insert(std::pair<float, Chunk*>(glm::distance(camPos, pos), chunk));
		}
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

void	ChunkManager::checkChunkDistance(glm::vec3 & camPos, Chunk & chunk) {

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
		Chunk * newChunk = new Chunk(oppositePos);
		_loadList.push_back(newChunk);
		_setupMap.insert(std::pair<float, Chunk*>(glm::distance(camPos, chunkPos), newChunk));
		_unloadList.push_back(&chunk);
	}
}

void	ChunkManager::updateVisibilityList(glm::vec3 & camPos) {

	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it)
	{
		Chunk *	chunk = it->second;

		checkChunkDistance(camPos, *chunk);
	}
}

bool	ChunkManager::isOccluded(Chunk * chunk) {
	if (chunk->isBBoxBuilt() == false)
		chunk->buildBoundingBox();
	if (!_query.isInUse()) {
		_query.start();
		chunk->renderBoundingBox();
		_query.end();
	}
	if (_query.isResultReady())
		return !_query.getResult();
	else
		return false;
}

void	ChunkManager::render(Shader & shader, Shader & bboxShader) {

	(void)bboxShader;

	_totalActiveBlocks = 0;
	_totalActiveChunks = 0;
	for (std::vector<Chunk*>::iterator it = _renderList.begin(); it != _renderList.end(); ++it)
	{
		Chunk *	chunk = (*it);
		
		shader.use();
		if (chunk->isBuilt() == false && chunk->isSetup() == true)
		{
			chunk->buildMesh();
		}
		chunk->render();
		_totalActiveChunks += 1;
		_totalActiveBlocks += chunk->getActiveBlocks();

	}
}

std::map<index3D, Chunk*> & ChunkManager::getChunks() {
	return _chunkMap;
}

size_t	ChunkManager::getTotalActiveBlocks() const { return _totalActiveBlocks; }
size_t	ChunkManager::getTotalActiveChunks() const { return _totalActiveChunks; }
