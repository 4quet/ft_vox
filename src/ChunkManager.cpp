/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChunkManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 17:29:47 by lfourque          #+#    #+#             */
/*   Updated: 2017/12/01 17:51:15 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChunkManager.hpp"

ChunkManager::ChunkManager(glm::vec3 camPos) :
	_camPos(camPos),
	_maxDistWidth((CHUNK_RENDER_SIZE * VIEW_DISTANCE_WIDTH) / 2.0f),
	_maxDistHeight((CHUNK_RENDER_SIZE * VIEW_DISTANCE_HEIGHT) / 2.0f) {
		
	Chunk::loadTexturesAtlas("assets/textures/textures.png");
	Chunk::setUVs(3, 4, 10);

	setGroundFlags();
	
	Chunk *		chunk;
	glm::vec3	chunkPos;

	camPos.x = floor(camPos.x);
	camPos.y = floor(camPos.y);
	camPos.z = floor(camPos.z);
	
	for (float x = camPos.x - _maxDistWidth; x < camPos.x + _maxDistWidth; x += CHUNK_RENDER_SIZE) {
		for (float y = -_maxDistHeight; y < _maxDistHeight; y += CHUNK_RENDER_SIZE) {
			for (float z = camPos.z - _maxDistWidth; z < camPos.z + _maxDistWidth; z += CHUNK_RENDER_SIZE) {
				chunk = new Chunk(glm::vec3(x, y, z));
				bm.setupLandscape(*chunk);
				_chunkMap.insert( std::pair<index3D, Chunk*>(index3D(x, y, z), chunk) );
			}
		}
	}

	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it) {
		chunk = it->second;
		chunkPos = chunk->getPosition();

		if (shouldBeRendered(chunkPos)) {
			setNeighbors(*chunk);
			chunk->setup();
		}
	}
}

ChunkManager::~ChunkManager() {
	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it) {
		delete it->second;
	}
}

void	ChunkManager::setGroundFlags() {
	_isUnderGround = (_camPos.y < GROUND_LEVEL) ? true: false;
	_isAboveGround = (_camPos.y > CAVE_LEVEL) ? true: false;
}

void	ChunkManager::update(Camera & camera) {
	
	_camPos = camera.getPosition();

	setGroundFlags();

	updateVisibilityList();

	if (_unloadList.size())
		updateUnloadList();

	if (_loadList.size())
		updateLoadList();
	
	if (_setupMap.size())
		updateSetupList();

	setRenderList(camera);
}

void	ChunkManager::updateLoadList() {
	Chunk *		chunk;
	glm::vec3	chunkPos;
	index3D		chunkIndex;

	for (std::vector<Chunk*>::iterator it = _loadList.begin(); it != _loadList.end(); ++it) {
		chunk = *it;
		chunkPos = chunk->getPosition();
		chunkIndex = index3D(chunkPos.x, chunkPos.y, chunkPos.z);

		_chunkMap.insert( std::pair<index3D, Chunk*>(chunkIndex, chunk) );
	}
	_loadList.clear();
}

void	ChunkManager::setNeighbors(Chunk & chunk) {
	glm::vec3							chunkPos = chunk.getPosition();
	float								chunkRenderSize = CHUNK_RENDER_SIZE;
	std::map<index3D, Chunk*>::iterator	it;

	it = _chunkMap.find( index3D( chunkPos.x - chunkRenderSize, chunkPos.y, chunkPos.z ));
	chunk.left = (it != _chunkMap.end()) ? it->second : NULL;

	it = _chunkMap.find( index3D( chunkPos.x + chunkRenderSize, chunkPos.y, chunkPos.z ));
	chunk.right = (it != _chunkMap.end()) ? it->second : NULL;

	it = _chunkMap.find( index3D( chunkPos.x, chunkPos.y + chunkRenderSize, chunkPos.z ));
	chunk.top = (it != _chunkMap.end()) ? it->second : NULL;

	it = _chunkMap.find( index3D( chunkPos.x, chunkPos.y - chunkRenderSize, chunkPos.z ));
	chunk.bottom = (it != _chunkMap.end()) ? it->second : NULL;

	it = _chunkMap.find( index3D( chunkPos.x, chunkPos.y, chunkPos.z + chunkRenderSize ));
	chunk.front = (it != _chunkMap.end()) ? it->second : NULL;

	it = _chunkMap.find( index3D( chunkPos.x, chunkPos.y, chunkPos.z - chunkRenderSize ));
	chunk.back = (it != _chunkMap.end()) ? it->second : NULL;
}

void	ChunkManager::updateSetupList() {
	Chunk *		chunk;
	glm::vec3	chunkPos;
	int			setupThisFrame = 0;

	for (std::map<float, Chunk*>::iterator it = _setupMap.begin(); it != _setupMap.end(); ++it) {
		if (setupThisFrame >= MAX_CHUNK_SETUP_PER_FRAME) {
			break;
		}
		chunk = it->second;
		chunkPos = chunk->getPosition();

		if (shouldBeRendered(chunkPos)) {

			setNeighbors(*chunk);

			if (chunk->right && chunk->right->isLandscapeSetup() == false) {
				bm.setupLandscape(*chunk->right);
			}

			if (chunk->left && chunk->left->isLandscapeSetup() == false) {
				bm.setupLandscape(*chunk->left);
			}

			if (chunk->top && chunk->top->isLandscapeSetup() == false) {
				bm.setupLandscape(*chunk->top);
			}

			if (chunk->bottom && chunk->bottom->isLandscapeSetup() == false) {
				bm.setupLandscape(*chunk->bottom);
			}

			if (chunk->front && chunk->front->isLandscapeSetup() == false) {
				bm.setupLandscape(*chunk->front);
			}

			if (chunk->back && chunk->back->isLandscapeSetup() == false) {
				bm.setupLandscape(*chunk->back);
			}

			if (chunk->isLandscapeSetup() == false) {
				bm.setupLandscape(*chunk);
			}

			chunk->setup();
			setupThisFrame++;
		}

	}
	_setupMap.clear();
}

bool	ChunkManager::shouldBeRendered(glm::vec3 & chunkPos) const {
	glm::vec3	d = chunkPos - _camPos;
	float		dist = glm::distance(_camPos, chunkPos);

	if (_isUnderGround && chunkPos.y > GROUND_LEVEL)
		return false;
	if (_isAboveGround && chunkPos.y < CAVE_LEVEL)
		return false;
	if (_isUnderGround && dist > CHUNK_RENDER_SIZE * 5.0f)
		return false;
	if (fabs(d.x) > _maxDistWidth - CHUNK_RENDER_SIZE)
		return false;
	if (fabs(d.z) > _maxDistWidth - CHUNK_RENDER_SIZE)
		return false;
	if (fabs(d.y) > _maxDistHeight - CHUNK_RENDER_SIZE)
		return false;
	return true;
}

void	ChunkManager::setRenderList(Camera & camera) {
	float		halfChunk = CHUNK_RENDER_SIZE / 2.0f;
	float		renderDist, setupDist;

	Frustum	frustum(Shader::perspective);
	frustum.setView(camera.getMatrix());
	frustum.setPlanes();

	_renderMap.clear();

	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it) {
		Chunk *		chunk = it->second;
		glm::vec3	chunkPos = chunk->getPosition();

		if (shouldBeRendered(chunkPos)) {
			if (chunk->getActiveBlocks() > 0) {
				if (frustum.pointIn(chunkPos.x + halfChunk, chunkPos.y + halfChunk, chunkPos.z + halfChunk)) {
					renderDist = glm::distance(_camPos, glm::vec3( chunkPos.x + halfChunk, chunkPos.y + halfChunk, chunkPos.z + halfChunk ));
					_renderMap.insert(std::pair<float, Chunk*>(renderDist, chunk));
				}
			}
			if (chunk->isSetup() == false) {
				setupDist = glm::distance(_camPos, chunkPos);
				_setupMap.insert(std::pair<float, Chunk*>(setupDist, chunk));
			}
		}
	}
}

void	ChunkManager::updateUnloadList() {
		for (std::vector<index3D>::iterator it = _unloadList.begin(); it != _unloadList.end(); ++it) {
		_chunkMap.erase(*it);
	}
	_unloadList.clear();
}

void	ChunkManager::checkChunkDistance(Chunk & chunk) {
	glm::vec3	chunkPos, oppositePos, dist;
	bool		b = false;

	chunkPos = chunk.getPosition();
	dist = chunkPos - _camPos;

	if (fabs(dist.x) > _maxDistWidth) {
		oppositePos = glm::vec3(chunkPos.x - _maxDistWidth * 2.0f * std::copysign(1.0f, dist.x), chunkPos.y, chunkPos.z);
		b = true;
	}
	else if (fabs(dist.z) > _maxDistWidth) {
		oppositePos = glm::vec3(chunkPos.x, chunkPos.y, chunkPos.z - _maxDistWidth * 2.0f * std::copysign(1.0f, dist.z));
		b = true;
	}
	else if (fabs(dist.y) > _maxDistHeight) {
		oppositePos = glm::vec3(chunkPos.x, chunkPos.y - _maxDistHeight * 2.0f * std::copysign(1.0f, dist.y), chunkPos.z);
		if (oppositePos.y > -WORLD_BOTTOM * CHUNK_RENDER_SIZE)
			b = true;
	}

	if (b) {
		index3D	currentIndex(chunkPos.x, chunkPos.y, chunkPos.z);

		chunk.reset();
		chunk.setPosition(oppositePos);

		_loadList.push_back(&chunk);
		_unloadList.push_back(currentIndex);

		_setupMap.insert(std::pair<float, Chunk*>(glm::distance(_camPos, oppositePos), &chunk));
	}
}

void	ChunkManager::updateVisibilityList() {
	for (std::map<index3D, Chunk*>::iterator it = _chunkMap.begin(); it != _chunkMap.end(); ++it) {
		checkChunkDistance(*it->second);
	}
}

void	ChunkManager::render(Shader & shader) {
	Chunk *	chunk;

	_totalActiveBlocks = 0;
	_totalActiveChunks = 0;

	shader.use();
	if (_isUnderGround) {
		shader.setFloat("fogDensity", 0.06f);
		shader.setVec3("fogColor", 0.f, 0.f, 0.f);
	}
	else {
		shader.setFloat("fogDensity", 0.02f - VIEW_DISTANCE_WIDTH / 5000.f);
		shader.setVec3("fogColor", 1.f, 1.f, 1.f);
	}

	for (std::map<float, Chunk*>::reverse_iterator it = _renderMap.rbegin(); it != _renderMap.rend(); ++it) {
		chunk = it->second;
		if (chunk->isSetup()) {
			chunk->render();
			_totalActiveChunks += 1;
			_totalActiveBlocks += chunk->getActiveBlocks();
		}
	}
}

std::map<float, Chunk*> & ChunkManager::getRenderMap() { return _renderMap; }

size_t	ChunkManager::getTotalActiveBlocks() const { return _totalActiveBlocks; }
size_t	ChunkManager::getTotalActiveChunks() const { return _totalActiveChunks; }
