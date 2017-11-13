/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:27:26 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/13 13:51:44 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk.hpp"
#include "./stb_image.h"

FastNoise				Chunk::sNoise;
std::vector<glm::vec2>	Chunk::uvs;
unsigned int 			Chunk::texturesID;

Chunk::Chunk(glm::vec3 position)
	: _activeBlocks(0), _totalVertices(0), _position(position),
		_visible(false), _setup(false), _built(false), _bboxBuilt(false) { 

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

void	Chunk::render() {
	glActiveTexture(texturesID);
	glBindTexture(GL_TEXTURE_2D, texturesID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, _totalVertices);
	glBindVertexArray(0);
}

void	Chunk::setup() {
	if (_setup == false)
	{
		setupLandscape();
		fillMesh();
		_setup = true;
	}
}

void	Chunk::setupLandscape() {
	float surfaceFreq = 0.015f;
	float caveFreq = 0.015f;
	bool hasCave = (_position.y <= -GROUND_LEVEL);
	bool inBetween = (_position.y > -GROUND_LEVEL && _position.y < GROUND_LEVEL);

	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int z = 0; z < CHUNK_SIZE; ++z)
		{
			if (hasCave)
			{
				Chunk::sNoise.SetFrequency(caveFreq); // Set the desired noise freq
				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					float	density = (Chunk::sNoise.GetNoise(_position.x + x, _position.z + z, _position.y + y));
					if (density > 0.0f)
					{
						_blocks[x][y][z].setActive(true);
					}
				}
				
			}
			else if (inBetween)
			{
				float gradient = 0.0f;
				Chunk::sNoise.SetFrequency(caveFreq); // Set the desired noise freq
				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					float	density = (Chunk::sNoise.GetNoise(_position.x + x, _position.z + z, _position.y + y)) + gradient;
					if (density > 0.0f) //&& height > 0.0f)
					{
						_blocks[x][y][z].setActive(true);
					}
					gradient += 0.75f / CHUNK_SIZE;
				}
			}
			else
			{
				Chunk::sNoise.SetFrequency(surfaceFreq); // Set the desired noise freq
				float	height = (Chunk::sNoise.GetNoise(_position.x + x, _position.z + z) + 1.0f) / 2.0f * (CHUNK_SIZE * MAX_ALTITUDE);
				for (int y = 0; y < CHUNK_SIZE; ++y)
				{
					if (_position.y + y < height)
						_blocks[x][y][z].setActive(true);
				}
			}
		}
	}
}

void	Chunk::fillMesh() {
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

					if (!adj.everywhere()) {
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
	if (_built == false)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), &mesh[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);	
		_built = true;
	}
}

void	Chunk::createCube(float x, float y, float z, AdjacentBlocks & adj, BlockType t) {
	float halfBlockSize = BLOCK_RENDER_SIZE / 2.0f;
	
	glm::vec3	p1(x - halfBlockSize, y - halfBlockSize, z + halfBlockSize);
	glm::vec3	p2(x + halfBlockSize, y - halfBlockSize, z + halfBlockSize);
	// glm::vec3	p3(x + halfBlockSize, y + halfBlockSize, z + halfBlockSize);
	glm::vec3	p4(x - halfBlockSize, y + halfBlockSize, z + halfBlockSize);
	glm::vec3	p5(x + halfBlockSize, y - halfBlockSize, z - halfBlockSize);
	glm::vec3	p6(x - halfBlockSize, y - halfBlockSize, z - halfBlockSize);
	// glm::vec3	p7(x - halfBlockSize, y + halfBlockSize, z - halfBlockSize);
	// glm::vec3	p8(x + halfBlockSize, y + halfBlockSize, z - halfBlockSize);

	if (!adj.front)
		createFace(p1, Faces::FRONT, t);
	if (!adj.back)
		createFace(p5, Faces::BACK, t);
	if (!adj.right)
		createFace(p2, Faces::RIGHT, t);
	if (!adj.left)
		createFace(p6, Faces::LEFT, t);
	if (!adj.top)
		createFace(p4, Faces::TOP, t);
	if (!adj.bottom)
		createFace(p6, Faces::BOTTOM, t);

	_activeBlocks++;
}

void	Chunk::createFace(glm::vec3 point, Faces::Enum face, BlockType type) {
	glm::vec3 incX = glm::vec3(BLOCK_RENDER_SIZE, 0, 0);
	glm::vec3 incY = glm::vec3(0, BLOCK_RENDER_SIZE, 0);
	glm::vec3 incZ = glm::vec3(0, 0, BLOCK_RENDER_SIZE);

	std::vector<glm::vec2>	uv;

	switch(face) {
		case Faces::FRONT:
			uv = getTriangleUVs(1, face, type);
			addTriangle(point, point + incX, point + incX + incY, glm::vec3(0.f, 0.f, 1.f), uv);
			uv = getTriangleUVs(0, face, type);
			addTriangle(point, point + incX + incY, point + incY, glm::vec3(0.f, 0.f, 1.f), uv);
			return;
		case Faces::BACK:
			uv = getTriangleUVs(1, face, type);
			addTriangle(point, point - incX, point - incX + incY, glm::vec3(0.f, 0.f, -1.f), uv);
			uv = getTriangleUVs(0, face, type);
			addTriangle(point, point - incX + incY, point + incY, glm::vec3(0.f, 0.f, -1.f), uv);
			return;
		case Faces::RIGHT:
			uv = getTriangleUVs(1, face, type);
			addTriangle(point, point - incZ, point - incZ + incY, glm::vec3(1.f, 0.f, 0.f), uv);
			uv = getTriangleUVs(0, face, type);
			addTriangle(point, point - incZ + incY, point + incY, glm::vec3(1.f, 0.f, 0.f), uv);
			return;
		case Faces::LEFT:
			uv = getTriangleUVs(1, face, type);
			addTriangle(point, point + incZ, point + incZ + incY, glm::vec3(-1.f, 0.f, 0.f), uv);
			uv = getTriangleUVs(0, face, type);
			addTriangle(point, point + incZ + incY, point + incY, glm::vec3(-1.f, 0.f, 0.f), uv);
			return;
		case Faces::TOP:
			uv = getTriangleUVs(1, face, type);
			addTriangle(point, point + incX, point + incX - incZ, glm::vec3(0.f, 1.f, 0.f), uv);
			uv = getTriangleUVs(0, face, type);
			addTriangle(point, point + incX - incZ, point - incZ, glm::vec3(0.f, 1.f, 0.f), uv);
			return;
		case Faces::BOTTOM:
			uv = getTriangleUVs(1, face, type);
			addTriangle(point, point + incX, point + incX + incZ, glm::vec3(0.f, -1.f, 0.f), uv);
			uv = getTriangleUVs(0, face, type);
			addTriangle(point, point + incX + incZ, point + incZ, glm::vec3(0.f, -1.f, 0.f), uv);
			return;
	}
}

std::vector<glm::vec2>	Chunk::getTriangleUVs(bool firstTriangle, Faces::Enum face, BlockType type) const {
	unsigned int	texture;
	std::vector<glm::vec2>	uv;

	switch(type) {
		case BLOCKTYPE_GRASS:
			if (face == Faces::TOP)
				texture = 1;
			else if (face == Faces::BOTTOM)
				texture = 2;
			else
				texture = 0;
			break;
		case BLOCKTYPE_STONE: texture = 2; break;
		case BLOCKTYPE_DEFAULT: texture = 2; break;
	}
	
	glm::vec2	topLeft = uvs[texture * 4];
	glm::vec2	topRight = uvs[texture * 4 + 1];
	glm::vec2	bottomRight = uvs[texture * 4 + 2];
	glm::vec2	bottomLeft = uvs[texture * 4 + 3];	
	
	if(firstTriangle) {
		uv.push_back(bottomRight);
		uv.push_back(bottomLeft);
		uv.push_back(topLeft);
	} else {
		uv.push_back(bottomRight);
		uv.push_back(topLeft);
		uv.push_back(topRight);
	}
	return uv;
}

void	Chunk::addTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3  normal, std::vector<glm::vec2> & uv) {
	mesh.insert(mesh.end(), { p1.x, p1.y, p1.z, normal.x, normal.y, normal.z, uv[0].x, uv[0].y });
	mesh.insert(mesh.end(), { p2.x, p2.y, p2.z, normal.x, normal.y, normal.z, uv[1].x, uv[1].y });
	mesh.insert(mesh.end(), { p3.x, p3.y, p3.z, normal.x, normal.y, normal.z, uv[2].x, uv[2].y });
	_totalVertices += 3;
}

glm::vec3	Chunk::getPosition() const { return _position; }
void		Chunk::setPosition(glm::vec3 pos) { _position = pos; }

size_t		Chunk::getActiveBlocks() const { return _activeBlocks; }

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

void	Chunk::loadTexturesAtlas(std::string file) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

    int width, height, componentNbr;
    stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(file.c_str(), &width, &height, &componentNbr, 0);
	if(data) {
		GLenum format;
		switch(componentNbr) {
			case 1: format = GL_RED; break;
			case 3: format = GL_RGB; break;
			case 4: format = GL_RGBA; break;
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Loading texture failed: " << file << std::endl;
		stbi_image_free(data);
	}
	Chunk::texturesID = textureID;
}

void	Chunk::setUVs(unsigned int width, unsigned int height, unsigned int nbr) {
	// from top/left to bottom/right
	float stepX = 1 / static_cast<float>(width);
	float stepY = 1 / static_cast<float>(height);
	std::vector<glm::vec2> uvs;
	
	for (int i = height; i > 0; i--) {
		for (unsigned int j = 0; j < width; j++) {
			uvs.push_back(glm::vec2(j * stepX, i * stepY));
			uvs.push_back(glm::vec2(j * stepX + stepX, i * stepY));
			uvs.push_back(glm::vec2(j * stepX + stepX, i * stepY - stepY));
			uvs.push_back(glm::vec2(j * stepX, i * stepY - stepY));
			if (uvs.size() == nbr * 4)
				break;
		}
	}

	// for (unsigned int i = 0; i < uvs.size(); i++) {
	// 	std::cout << uvs[i].x << " : " << uvs[i].y << std::endl;
	// }

	Chunk::uvs = uvs;
}
