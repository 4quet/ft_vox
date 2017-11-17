/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:29:47 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/17 18:23:23 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChunkManager.hpp"
#include <chrono>

ChunkManager::ChunkManager(glm::vec3 camPos) {

	Chunk::loadTexturesAtlas("./textures/textures.png");
	Chunk::setUVs(3, 4, 10);
	
	std::vector<std::future<std::pair<index3D, Chunk*>>>	futures;

	int	startWidth = -(VIEW_DISTANCE_WIDTH / 2);
	int endWidth = VIEW_DISTANCE_WIDTH / 2;
	int	startHeight = -(VIEW_DISTANCE_HEIGHT / 2);
	int endHeight = VIEW_DISTANCE_HEIGHT / 2;
	for (int x = camPos.x + startWidth; x < camPos.x + endWidth; ++x)
	{
		for (int y = camPos.y + startHeight; y < camPos.y + endHeight; ++y)
		{
			for (int z = camPos.z + startWidth; z < camPos.z + endWidth; ++z)
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

	if (yPos <= CHUNK_SIZE * MAX_ALTITUDE)
	{
		Chunk *		chunk = new Chunk(glm::vec3(xPos, yPos, zPos));
		bm.setupLandscape(*chunk);
		chunk->setup();
		return std::pair<index3D, Chunk*>(index3D(xPos, yPos, zPos), chunk);
	}
	else
	{
		return std::pair<index3D, Chunk*>(index3D(0, 0, 0), NULL);
	}
}

void	ChunkManager::update(Camera & camera) {

	_camPos = camera.getPosition();

	_isUnderGround = (_camPos.y < GROUND_LEVEL) ? true: false;
	_isAboveGround = (_camPos.y > CAVE_LEVEL) ? true: false;

	updateVisibilityList();

	updateLoadList();

	if (_setupMap.size() > 0)
	{
		updateSetupList();
	}

	updateUnloadList();

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
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	int	setupThisFrame = 0;

	for (std::map<float, Chunk*>::iterator it = _setupMap.begin(); it != _setupMap.end(); ++it)
	{
		if (setupThisFrame >= MAX_CHUNK_SETUP_PER_FRAME)
		{
			break;
		}
		Chunk *		chunk = it->second;
		glm::vec3	chunkPos = chunk->getPosition();

		if (shouldBeRendered(chunkPos))
		{
			bm.setupLandscape(*chunk);
			chunk->setup();
			setupThisFrame++;
		}

	}

	_setupMap.clear();

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

	if (duration)
		std::cout << " duration: " << duration << " ms - " << setupThisFrame << " setup this frame" << std::endl;
	/*
		*/
}

bool	ChunkManager::shouldBeRendered(glm::vec3 & chunkPos) const {

//	float	dist = glm::distance(_camPos, chunkPos);
	if (_isUnderGround && chunkPos.y > GROUND_LEVEL)
	   return false;
	if (_isAboveGround && chunkPos.y <= CAVE_LEVEL)
		return false;
	return true;
}

void	ChunkManager::setRenderList(Camera & camera) {

	Frustum	frustum(Shader::perspective);
	frustum.setView(camera.getMatrix());
	frustum.setPlanes();

	float		halfChunk = CHUNK_RENDER_SIZE / 2.0f;
	int			setupThisFrame = 0;

	_renderMap.clear();

	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it)
	{
		Chunk *		chunk = it->second;
		glm::vec3	chunkPos = chunk->getPosition();

		if (shouldBeRendered(chunkPos))
		{
			float dist = glm::distance(_camPos, glm::vec3( chunkPos.x + halfChunk, chunkPos.y + halfChunk, chunkPos.z + halfChunk ));

			if (frustum.pointIn(chunkPos.x + halfChunk, chunkPos.y + halfChunk, chunkPos.z + halfChunk))
			{
				_renderMap.insert(std::pair<float, Chunk*>(dist, chunk));
			}

			// this may move
			if (chunk->isSetup() == false)
			{
				_setupMap.insert(std::pair<float, Chunk*>(dist, chunk));
			}
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

void	ChunkManager::checkChunkDistance(Chunk & chunk) {

	float		maxDistWidth = ((BLOCK_RENDER_SIZE * CHUNK_SIZE) * (VIEW_DISTANCE_WIDTH)) / 2.0f;
	float		maxDistHeight = ((BLOCK_RENDER_SIZE * CHUNK_SIZE) * (VIEW_DISTANCE_HEIGHT)) / 2.0f;
	glm::vec3	chunkPos = chunk.getPosition();
	glm::vec3	dist = chunkPos - _camPos;

	bool		b = false;
	glm::vec3	oppositePos;

	if (fabs(dist.x) > maxDistWidth)
	{
		oppositePos = glm::vec3(chunkPos.x - maxDistWidth * 2.0f * std::copysign(1.0f, dist.x), chunkPos.y, chunkPos.z);
		b = true;
	}
	else if (fabs(dist.z) > maxDistWidth)
	{
		oppositePos = glm::vec3(chunkPos.x, chunkPos.y, chunkPos.z - maxDistWidth * 2.0f * std::copysign(1.0f, dist.z));
		b = true;
	}
	else if (fabs(dist.y) > maxDistHeight)
	{
		oppositePos = glm::vec3(chunkPos.x, chunkPos.y - maxDistHeight * 2.0f * std::copysign(1.0f, dist.y), chunkPos.z);
		if (oppositePos.y > -WORLD_BOTTOM * CHUNK_RENDER_SIZE)
			b = true;
	}

	//if ((_isUnderGround && chunkPos.y < GROUND_LEVEL) || (_isAboveGround && chunkPos.y >= CAVE_LEVEL))
	if (b)
	{
		Chunk * newChunk = new Chunk(oppositePos);
		_loadList.push_back(newChunk);
		_setupMap.insert(std::pair<float, Chunk*>(glm::distance(_camPos, chunkPos), newChunk));
		_unloadList.push_back(&chunk);
	}
}

void	ChunkManager::updateVisibilityList() {

	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it)
	{
		Chunk *	chunk = it->second;

		checkChunkDistance(*chunk);
	}
}

void	ChunkManager::render(Shader & shader) {

	shader.use();

	_totalActiveBlocks = 0;
	_totalActiveChunks = 0;
	for (std::map<float, Chunk*>::reverse_iterator it = _renderMap.rbegin(); it != _renderMap.rend(); ++it)
	{
		Chunk *	chunk = it->second;
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

std::map<float, Chunk*> & ChunkManager::getRenderMap() {
	return _renderMap;
}

size_t	ChunkManager::getTotalActiveBlocks() const { return _totalActiveBlocks; }
size_t	ChunkManager::getTotalActiveChunks() const { return _totalActiveChunks; }
