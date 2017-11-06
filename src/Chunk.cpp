/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:27:26 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/06 17:53:38 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk.hpp"

FastNoise	Chunk::sNoise;

Chunk::Chunk(glm::vec3 position) : _activeBlocks(0), _totalVertices(0), _position(position), _visible(false), _setup(false), _built(false) { 
	//std::cout << "--- Creating new chunk ---" << std::endl;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Create the blocks
	_blocks = new Block**[CHUNK_SIZE];
	for(int i = 0; i < CHUNK_SIZE; i++)
	{
		_blocks[i] = new Block*[CHUNK_SIZE];

		for(int j = 0; j < CHUNK_SIZE; j++)
		{
			_blocks[i][j] = new Block[CHUNK_SIZE];
		}
	}
}

Chunk::~Chunk() {
	// Delete the blocks
	for (int i = 0; i < CHUNK_SIZE; ++i)
	{
		for (int j = 0; j < CHUNK_SIZE; ++j)
		{
			delete [] _blocks[i][j];
		}

		delete [] _blocks[i];
	}
	delete [] _blocks;
}

void	Chunk::update() {
	// ...
}

void	Chunk::render() {
	//	std::cout << "OPP " << _position.x << "; " << _position.y << "; " << _position.z << std::endl;
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, _totalVertices);
	glBindVertexArray(0);
}

void	Chunk::setup() {
	setupLandscape();
	fillMesh();
	_setup = true;
}

void	Chunk::setupLandscape() {

	float	caveFreq = 0.05f;
	float	surfaceFreq = 0.05f;

	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int z = 0; z < CHUNK_SIZE; ++z)
		{
			// CAVES
			if (_position.y < 0.0f)
			{
				Chunk::sNoise.SetFrequency(caveFreq); // Set the desired noise freq
				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					float density = CHUNK_SIZE *  Chunk::sNoise.GetNoise(_position.x + x, _position.y + y, _position.z + z);
					if (density > 0.0f)
					{
						_blocks[x][y][z].setActive(true);
						//	_blocks[x][y][z].setBlockType(BLOCKTYPE_STONE);
					}
				}
			}
			// SURFACE
			else
			{
				Chunk::sNoise.SetFrequency(surfaceFreq); // Set the desired noise freq
				float	hm = Chunk::sNoise.GetNoise(_position.x + x, _position.z + z) * CHUNK_SIZE;
				for (int y = 0; y < hm; ++y)
				{
					Chunk::sNoise.SetFrequency(caveFreq); // Set the desired noise freq
					float density = CHUNK_SIZE *  Chunk::sNoise.GetNoise(_position.x + x, _position.y + y, _position.z + z);

					if (density > 0.0f)
					{
						_blocks[x][y][z].setActive(true);
						//	_blocks[x][y][z].setBlockType(BLOCKTYPE_GRASS);
					}
				}
			}
			/*
			// MULTI-CHUNK SURFACE
			else
			{
			Chunk::sNoise.SetFrequency(0.09f); // Set the desired noise freq
			float	hm = Chunk::sNoise.GetNoise(_position.x + x, _position.z + z) * CHUNK_SIZE;
			for (int y = 0; y < CHUNK_SIZE; ++y)
			_blocks[x][y][z].setActive(true);
			for (int y = 0; y > hm; --y)
			{
			_blocks[x][CHUNK_SIZE + y - 1][z].setActive(false);
			}
			}
			*/
		}
	}
}

void	Chunk::fillMesh() {
	//	std::cout << "creating chunk mesh" << std::endl;
	AdjacentBlocks	adj;
	bool			defaultState = false;
	BlockType 		t;
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if (_blocks[x][y][z].isActive() == true)
				{
					adj.right = (x + 1 < CHUNK_SIZE) ? _blocks[x + 1][y][z].isActive() : defaultState;
					adj.left = (x - 1 >= 0) ? _blocks[x - 1][y][z].isActive() : defaultState;
					adj.top = (y + 1 < CHUNK_SIZE) ? _blocks[x][y + 1][z].isActive() : defaultState;
					adj.bottom = (y - 1 >= 0) ? _blocks[x][y - 1][z].isActive() : defaultState;
					adj.front = (z + 1 < CHUNK_SIZE) ? _blocks[x][y][z + 1].isActive() : defaultState;
					adj.back = (z - 1 >= 0) ? _blocks[x][y][z - 1].isActive() : defaultState;

					if (adj.everywhere())
					{
						_blocks[x][y][z].setActive(false);
					}
					else
					{
						if (adj.top == false)
							t = BLOCKTYPE_GRASS;
						else
							t = BLOCKTYPE_STONE;
						_blocks[x][y][z].setBlockType(t);
						createCube(_position.x + x * BLOCK_RENDER_SIZE, _position.y + y * BLOCK_RENDER_SIZE, _position.z + z * BLOCK_RENDER_SIZE, adj, t);
					}
				}
			}
		}
	}
}

void	Chunk::buildMesh() {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), &mesh[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);	
	_built = true;
}

void	Chunk::createCube(float x, float y, float z, AdjacentBlocks & adj, BlockType t) {
	float halfBlockSize = BLOCK_RENDER_SIZE / 2.0f;
	
	glm::vec3	p1(x - halfBlockSize, y - halfBlockSize, z + halfBlockSize);
	glm::vec3	p2(x + halfBlockSize, y - halfBlockSize, z + halfBlockSize);
	glm::vec3	p3(x + halfBlockSize, y + halfBlockSize, z + halfBlockSize);
	glm::vec3	p4(x - halfBlockSize, y + halfBlockSize, z + halfBlockSize);
	glm::vec3	p5(x + halfBlockSize, y - halfBlockSize, z - halfBlockSize);
	glm::vec3	p6(x - halfBlockSize, y - halfBlockSize, z - halfBlockSize);
	glm::vec3	p7(x - halfBlockSize, y + halfBlockSize, z - halfBlockSize);
	glm::vec3	p8(x + halfBlockSize, y + halfBlockSize, z - halfBlockSize);

	glm::vec3	normal;
	glm::vec3	color;

	if (t == BLOCKTYPE_GRASS)
		color = glm::vec3(0.3f, 0.4f, 0.25f);
	else if (t == BLOCKTYPE_STONE)
		color = glm::vec3(0.3f, 0.3f, 0.31f);
	else
		color = glm::vec3(1.0f, 1.0f, 1.0f);

	_activeBlocks++;

	if (!adj.front)
	{
		normal = glm::vec3(0.0f, 0.0f, 1.0f);
		addTriangle(p1, p2, p3, normal, color);
		addTriangle(p1, p3, p4, normal, color);
	}

	if (!adj.back)
	{
		normal = glm::vec3(0.0f, 0.0f, -1.0f);
		addTriangle(p5, p6, p7, normal, color);
		addTriangle(p5, p7, p8, normal, color);
	}

	if (!adj.right)
	{
		normal = glm::vec3(1.0f, 0.0f, 0.0f);
		addTriangle(p2, p5, p8, normal, color);
		addTriangle(p2, p8, p3, normal, color);
	}

	if (!adj.left)
	{
		normal = glm::vec3(-1.0f, 0.0f, 0.0f);
		addTriangle(p6, p1, p4, normal, color);
		addTriangle(p6, p4, p7, normal, color);
	}

	if (!adj.top)
	{
		normal = glm::vec3(0.0f, 1.0f, 0.0f);
		addTriangle(p4, p3, p8, normal, color);
		addTriangle(p4, p8, p7, normal, color);
	}

	if (!adj.bottom)
	{
		normal = glm::vec3(0.0f, -1.0f, 0.0f);
		addTriangle(p6, p5, p2, normal, color);
		addTriangle(p6, p2, p1, normal, color);
	}
}

void	Chunk::addTriangle(glm::vec3 & p1, glm::vec3 & p2, glm::vec3 & p3, glm::vec3 & normal, glm::vec3 & color) {
	mesh.insert(mesh.end(), { p1.x, p1.y, p1.z, normal.x, normal.y, normal.z, color.x, color.y, color.z });
	mesh.insert(mesh.end(), { p2.x, p2.y, p2.z, normal.x, normal.y, normal.z, color.x, color.y, color.z });
	mesh.insert(mesh.end(), { p3.x, p3.y, p3.z, normal.x, normal.y, normal.z, color.x, color.y, color.z });
	_totalVertices += 3;
}

glm::vec3	Chunk::getPosition() const { return _position; }
void		Chunk::setPosition(glm::vec3 pos) { _position = pos; }

size_t		Chunk::getActiveBlocks() const { return _activeBlocks; }

void		Chunk::setHeightMap() {
	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int y = 0; y < CHUNK_SIZE; ++y)
		{
			for (int z = 0; z < CHUNK_SIZE; ++z)
				_heightMap[x][y][z] = Chunk::sNoise.GetNoise(_position.x + x, _position.y + y, _position.z + z);
		}
	}
}

void		Chunk::setVisibility(bool b) {
	_visible = b;
}

bool		Chunk::isVisible() const {
	return _visible;
}

bool		Chunk::isSetup() const {
	return _setup;
}

bool		Chunk::isBuilt() const {
	return _built;
}
