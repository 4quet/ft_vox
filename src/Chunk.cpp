/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:27:26 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/22 11:14:34 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk.hpp"
#include "./stb_image.h"

std::vector<glm::vec2>	Chunk::uvs;
unsigned int 			Chunk::texturesID;

Chunk::Chunk(glm::vec3 position)
	: left(NULL), right(NULL), top(NULL), bottom(NULL), front(NULL), back(NULL),
	_activeBlocks(0), _totalVertices(0), _position(position),
	_setup(false), _landscapeSetup(false), _built(false), VAO(0), VBO(0) {

	_halfBlockSize = BLOCK_RENDER_SIZE / 2.0f;

	_blocks = new BlockTypes::Enum**[CHUNK_SIZE];
	for(int i = 0; i < CHUNK_SIZE; i++) {
		_blocks[i] = new BlockTypes::Enum*[CHUNK_SIZE];
		for(int j = 0; j < CHUNK_SIZE; j++) {
			_blocks[i][j] = new BlockTypes::Enum[CHUNK_SIZE];
		}
	}
}

Chunk::~Chunk() {
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			delete [] _blocks[i][j];
		}
		delete [] _blocks[i];
	}
	delete [] _blocks;
}

void	Chunk::render() {
	if (_totalVertices > 0) {
		glActiveTexture(texturesID);
		glBindTexture(GL_TEXTURE_2D, texturesID);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, _totalVertices);
		glBindVertexArray(0);
	}
}

void	Chunk::setup() {
	if (_setup == false) {
		fillMesh();
		_setup = true;
	}
}

void	Chunk::rebuild() {
	reset();
	_landscapeSetup = true;
	fillMesh();
}

void	Chunk::reset() {
	_setup = false;
	_built = false;
	_landscapeSetup = false;
	_activeBlocks = 0;
	_totalVertices = 0;
	left = NULL;
	right = NULL;
	top = NULL;
	bottom = NULL;
	front = NULL;
	back = NULL;
	if (mesh.size())
		mesh.clear();
}

bool	Chunk::isNeighborActive(Chunk * n, int x, int y, int z) const {
	if (n == NULL)
		return true;
	return (n->getBlock(x, y, z) > BlockTypes::WATER);
}

void	Chunk::fillMesh() {
	BlockTypes::Enum 		t;

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				if (_blocks[x][y][z] > BlockTypes::WATER) {
					t = _blocks[x][y][z];
					if ((y + 1 == CHUNK_SIZE && !isNeighborActive(top, x, 0, z)) ||
						(y + 1 < CHUNK_SIZE && _blocks[x][y + 1][z] <= BlockTypes::WATER)) {

						if (t == BlockTypes::DIRT && _position.y >= GROUND_LEVEL)
							t = BlockTypes::GRASS;
						createFace( glm::vec3(	(_position.x + x * BLOCK_RENDER_SIZE) - _halfBlockSize,
												(_position.y + y * BLOCK_RENDER_SIZE) + _halfBlockSize,
												(_position.z + z * BLOCK_RENDER_SIZE) + _halfBlockSize),
												Faces::TOP, t);
					}
					if ((x + 1 == CHUNK_SIZE && !isNeighborActive(right, 0, y, z)) ||
							(x + 1 < CHUNK_SIZE && _blocks[x + 1][y][z] <= BlockTypes::WATER)) {
						createFace( glm::vec3(	(_position.x + x * BLOCK_RENDER_SIZE) + _halfBlockSize,
												(_position.y + y * BLOCK_RENDER_SIZE) - _halfBlockSize,
												(_position.z + z * BLOCK_RENDER_SIZE) + _halfBlockSize),
												Faces::RIGHT, t);
					}
					if ((x - 1 < 0 && !isNeighborActive(left, CHUNK_SIZE - 1, y, z)) ||
							(x - 1 >= 0 && _blocks[x - 1][y][z] <= BlockTypes::WATER)) {
						createFace( glm::vec3(	(_position.x + x * BLOCK_RENDER_SIZE) - _halfBlockSize,
												(_position.y + y * BLOCK_RENDER_SIZE) - _halfBlockSize,
												(_position.z + z * BLOCK_RENDER_SIZE) - _halfBlockSize),
												Faces::LEFT, t);
					}
					if ((z + 1 == CHUNK_SIZE && !isNeighborActive(front, x, y, 0)) ||
							(z + 1 < CHUNK_SIZE && _blocks[x][y][z + 1] <= BlockTypes::WATER)) {
						createFace( glm::vec3(	(_position.x + x * BLOCK_RENDER_SIZE) - _halfBlockSize,
												(_position.y + y * BLOCK_RENDER_SIZE) - _halfBlockSize,
												(_position.z + z * BLOCK_RENDER_SIZE) + _halfBlockSize),
												Faces::FRONT, t);
					}
					if ((z - 1 < 0 && !isNeighborActive(back, x, y, CHUNK_SIZE - 1)) ||
							(z - 1 >= 0 && _blocks[x][y][z - 1] <= BlockTypes::WATER)) {
						createFace( glm::vec3(	(_position.x + x * BLOCK_RENDER_SIZE) + _halfBlockSize,
												(_position.y + y * BLOCK_RENDER_SIZE) - _halfBlockSize,
												(_position.z + z * BLOCK_RENDER_SIZE) - _halfBlockSize),
												Faces::BACK, t);
					}
					if ((y - 1 < 0 && !isNeighborActive(bottom, x, CHUNK_SIZE - 1, z)) ||
							(y - 1 >= 0 && _blocks[x][y - 1][z] <= BlockTypes::WATER)) {
						createFace( glm::vec3(	(_position.x + x * BLOCK_RENDER_SIZE) - _halfBlockSize,
												(_position.y + y * BLOCK_RENDER_SIZE) - _halfBlockSize,
												(_position.z + z * BLOCK_RENDER_SIZE) - _halfBlockSize),
												Faces::BOTTOM, t);
					}
					_activeBlocks++;
				}
				else if (_blocks[x][y][z] == BlockTypes::WATER) {
					if (y == CHUNK_SIZE - 1) {
						createFace( glm::vec3(	(_position.x + x * BLOCK_RENDER_SIZE) - _halfBlockSize,
												(_position.y + y * BLOCK_RENDER_SIZE) + _halfBlockSize,
												(_position.z + z * BLOCK_RENDER_SIZE) + _halfBlockSize),
												Faces::TOP, BlockTypes::WATER);
					}
					_activeBlocks++;
				}
			}
		}
	}

	if (waterMesh.size()) {
		mesh.insert(mesh.end(), waterMesh.begin(), waterMesh.end());
		waterMesh.clear();
	}
}

void	Chunk::buildMesh() {
	if (_built == false) {
		if (!VAO)
			glGenVertexArrays(1, &VAO);
		if (!VBO)
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
		mesh.clear();
		waterMesh.clear();
	}
}

void	Chunk::createFace(glm::vec3 point, Faces::Enum face, BlockTypes::Enum type) {
	glm::vec3 incX = glm::vec3(BLOCK_RENDER_SIZE, 0, 0);
	glm::vec3 incY = glm::vec3(0, BLOCK_RENDER_SIZE, 0);
	glm::vec3 incZ = glm::vec3(0, 0, BLOCK_RENDER_SIZE);

	std::vector<glm::vec2>	uv;

	switch(face) {
		case Faces::FRONT:
			getFaceUVs(face, type, uv);
			addFace(point, point + incX, point + incX + incY, point + incY, glm::vec3(0.f, 0.f, 1.f), uv, type);
			return;
		case Faces::BACK:
			getFaceUVs(face, type, uv);
			addFace(point, point - incX, point - incX + incY, point + incY, glm::vec3(0.f, 0.f, -1.f), uv, type);
			return;
		case Faces::RIGHT:
			getFaceUVs(face, type, uv);
			addFace(point, point - incZ, point - incZ + incY, point + incY, glm::vec3(1.f, 0.f, 0.f), uv, type);
			return;
		case Faces::LEFT:
			getFaceUVs(face, type, uv);
			addFace(point, point + incZ, point + incZ + incY, point + incY, glm::vec3(-1.f, 0.f, 0.f), uv, type);
			return;
		case Faces::TOP:
			getFaceUVs(face, type, uv);
			addFace(point, point + incX, point + incX - incZ, point - incZ, glm::vec3(0.f, 1.f, 0.f), uv, type);
			return;
		case Faces::BOTTOM:
			getFaceUVs(face, type, uv);
			addFace(point, point + incX, point + incX + incZ, point + incZ, glm::vec3(0.f, -1.f, 0.f), uv, type);
			return;
	}
}

void	Chunk::getFaceUVs(Faces::Enum face, BlockTypes::Enum type, std::vector<glm::vec2> & uv) const {
	unsigned int	texture = 0;
	int r = 0;
	uv.clear();

	switch(type) {
		case BlockTypes::GRASS:
			if (face == Faces::TOP)
				texture = 8;
			else if (face == Faces::BOTTOM)
				texture = 4;
			else
				texture = 0;
			break;
		case BlockTypes::SNOW:
			if (face == Faces::TOP)
				texture = 7;
			else if (face == Faces::BOTTOM)
				texture = 4;
			else
				texture = 6;
			break;
		case BlockTypes::DIRT:
			texture = 4;
			break;
		case BlockTypes::SAND: texture = 5; break;
		case BlockTypes::WATER: texture = 9; break;
		case BlockTypes::ROCK: texture = 2; break;
		case BlockTypes::INACTIVE: break;
	}
	
	texture *= 4;
	
	uv.insert(uv.end(), { uvs[texture + 2], uvs[texture + 3], uvs[texture], uvs[texture + 2], uvs[texture], uvs[texture + 1] });
}

void	Chunk::addFace(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3  normal, std::vector<glm::vec2> & uv, BlockTypes::Enum type) {
	std::vector<float> &	rmesh = (type == BlockTypes::WATER) ? waterMesh : mesh;
	rmesh.insert(rmesh.end(), {
		p1.x, p1.y, p1.z, normal.x, normal.y, normal.z, uv[0].x, uv[0].y,
		p2.x, p2.y, p2.z, normal.x, normal.y, normal.z, uv[1].x, uv[1].y,
		p3.x, p3.y, p3.z, normal.x, normal.y, normal.z, uv[2].x, uv[2].y,

		p1.x, p1.y, p1.z, normal.x, normal.y, normal.z, uv[3].x, uv[3].y,
		p3.x, p3.y, p3.z, normal.x, normal.y, normal.z, uv[4].x, uv[4].y,
		p4.x, p4.y, p4.z, normal.x, normal.y, normal.z, uv[5].x, uv[5].y
	});
	_totalVertices += 6;
}

glm::vec3 const &	Chunk::getPosition() const {
	return _position;
}

void		Chunk::setPosition(glm::vec3 pos) {
	_position = pos;
}

size_t		Chunk::getActiveBlocks() const {
	return _activeBlocks;
}

BlockTypes::Enum &		Chunk::getBlock(int x, int y, int z) const {
	return _blocks[x][y][z];
}

bool		Chunk::isSetup() const {
	return _setup;
}

bool		Chunk::isLandscapeSetup() const {
	return _landscapeSetup;
}

void		Chunk::setLandscapeSetup(bool b) {
	_landscapeSetup = b;
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
	Chunk::uvs = uvs;
}
