/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:29:47 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/10 17:31:44 by tpierron         ###   ########.fr       */
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
	float surfaceFreq = 0.01;
	if (yPos > - GROUND_LEVEL)
	{
		Chunk *		chunk = new Chunk(glm::vec3(xPos, yPos, zPos));
		for (int x = 0; x < CHUNK_SIZE; ++x)
		{
			for (int z = 0; z < CHUNK_SIZE; ++z)
			{
				Chunk::sNoise.SetFrequency(surfaceFreq); // Set the desired noise freq
				float	hm = (Chunk::sNoise.GetNoise(xPos + x, zPos + z) + 1) / 2 * (CHUNK_SIZE * 2);
				// Chunk::sNoise.SetFrequency(surfaceFreq * 2); // Set the desired noise freq
				// float	hm2 = (Chunk::sNoise.GetNoise(x + x, z + z) + 1);
				// Chunk::sNoise.SetFrequency(surfaceFreq  * 4); // Set the desired noise freq
				// float	hm3 = (Chunk::sNoise.GetNoise(x + x, z + z) + 1);
				// float hm = 1 * hm1 + 0.5 * hm2 + 0.25 * hm3;
				// float hm =hm1 *  CHUNK_SIZE / 2 ;
				// std::cout << hm << std::endl;

				// std::cout <<  hm << std::endl;
				// if (hm < yPos + CHUNK_SIZE)
				chunk->activeBlock(x, z, hm);
				// std::cout << "B" << hm << std::endl;
			}
		}
		chunk->setup();
		return std::pair<index3D, Chunk*>(index3D(xPos, yPos, zPos), chunk);
	}
	else
	{
		return std::pair<index3D, Chunk*>(index3D(0, 0, 0), NULL);
	}
}

void	ChunkManager::update(Shader & shader, Camera & camera) {

	glm::vec3	camPos = camera.getPosition();

	shader.use();
	shader.setView();
	shader.setVec3("lightPos", camPos.x, camPos.y, camPos.z);

	updateVisibilityList(camera);

	//std::async( std::launch::async, &ChunkManager::updateLoadList, this );
	updateLoadList();

	if (_setupList.size() > 0)
	{
		updateSetupList();
	}

	//std::async( std::launch::async, &ChunkManager::updateUnloadList, this );
	updateUnloadList();

	//updateLoadList();
	//updateUnloadList();

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

void	ChunkManager::updateSetupList() {
	//std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	int	setupThisFrame = 0;

	for (std::vector<Chunk*>::iterator it = _setupList.begin(); it != _setupList.end(); ++it)
	{
		if (setupThisFrame >= MAX_CHUNK_SETUP_PER_FRAME)
		{
			break;
		}
		Chunk *		chunk = *it;
		chunk->setup();
		setupThisFrame++;
	}

	_setupList.clear();

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
			_setupList.push_back(chunk);
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

	if (b && oppositePos.y < GROUND_LEVEL)
	{
		Chunk * newChunk = new Chunk(oppositePos);
		_loadList.push_back(newChunk);
		_setupList.push_back(newChunk);
		_unloadList.push_back(&chunk);
	}
}

void	ChunkManager::updateVisibilityList(Camera & camera) {

	glm::vec3	camPos = camera.getPosition();

	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it)
	{
		Chunk *	chunk = it->second;

		checkChunkDistance(camPos, *chunk);
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
	_totalActiveChunks = 0;
	for (std::vector<Chunk*>::iterator it = _renderList.begin(); it != _renderList.end(); ++it)
	{
		Chunk *	chunk = (*it);
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
