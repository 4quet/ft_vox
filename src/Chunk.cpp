/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:27:26 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/25 11:18:19 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk.hpp"

//Chunk::Chunk() : _activeBlocks(0) { } 

Chunk::Chunk(ChunkManager & m) : _activeBlocks(0), _chunkManager(m) { 
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

	setupLandscape();

	createMesh();
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

void	Chunk::render(Shader & shader) {
	shader.use();

	shader.setView();
	shader.setModel(glm::mat4());

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36 * _activeBlocks);
	glBindVertexArray(0);
}

void	Chunk::setupLandscape() {

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            // Use the noise library to get the height value of x, z
			float height = _chunkManager.getHeightMapValue(x, z) * (CHUNK_SIZE-1);

            for (int y = 0; y < height; y++)
            {
                _blocks[x][y][z].setActive(true);
                //_blocks[x][y][z].setBlockType(BLOCKTYPE_GRASS);
            }
        }
    }

}

void	Chunk::createMesh() {
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if(_blocks[x][y][z].isActive() == false)
				{
					// Don't create triangle data for inactive blocks
					continue;
				}
				createCube(x * BLOCK_RENDER_SIZE, y * BLOCK_RENDER_SIZE, z * BLOCK_RENDER_SIZE);
				++_activeBlocks;
			}
		}
	}

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), &mesh[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);	
}

void	Chunk::createCube(float x, float y, float z) {
	glm::vec3	p1(x - BLOCK_RENDER_SIZE, y - BLOCK_RENDER_SIZE, z + BLOCK_RENDER_SIZE);
	glm::vec3	p2(x + BLOCK_RENDER_SIZE, y - BLOCK_RENDER_SIZE, z + BLOCK_RENDER_SIZE);
	glm::vec3	p3(x + BLOCK_RENDER_SIZE, y + BLOCK_RENDER_SIZE, z + BLOCK_RENDER_SIZE);
	glm::vec3	p4(x - BLOCK_RENDER_SIZE, y + BLOCK_RENDER_SIZE, z + BLOCK_RENDER_SIZE);
	glm::vec3	p5(x + BLOCK_RENDER_SIZE, y - BLOCK_RENDER_SIZE, z - BLOCK_RENDER_SIZE);
	glm::vec3	p6(x - BLOCK_RENDER_SIZE, y - BLOCK_RENDER_SIZE, z - BLOCK_RENDER_SIZE);
	glm::vec3	p7(x - BLOCK_RENDER_SIZE, y + BLOCK_RENDER_SIZE, z - BLOCK_RENDER_SIZE);
	glm::vec3	p8(x + BLOCK_RENDER_SIZE, y + BLOCK_RENDER_SIZE, z - BLOCK_RENDER_SIZE);

	glm::vec3	normal;

	// Front
	normal = glm::vec3(0.0f, 0.0f, 1.0f);
	// First triangle
	addVertex(p1, normal);
	addVertex(p2, normal);
	addVertex(p3, normal);
	// Second triangle
	addVertex(p1, normal);
	addVertex(p3, normal);
	addVertex(p4, normal);

	// Back
	normal = glm::vec3(0.0f, 0.0f, -1.0f);
	// First triangle
	addVertex(p5, normal);
	addVertex(p6, normal);
	addVertex(p7, normal);
	// Second triangle
	addVertex(p5, normal);
	addVertex(p7, normal);
	addVertex(p8, normal);

	// Right
	normal = glm::vec3(1.0f, 0.0f, 0.0f);
	// First triangle
	addVertex(p2, normal);
	addVertex(p5, normal);
	addVertex(p8, normal);
	// Second triangle
	addVertex(p2, normal);
	addVertex(p8, normal);
	addVertex(p3, normal);

	// Left
	normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	// First triangle
	addVertex(p6, normal);
	addVertex(p1, normal);
	addVertex(p4, normal);
	// Second triangle
	addVertex(p6, normal);
	addVertex(p4, normal);
	addVertex(p7, normal);

	// Top
	normal = glm::vec3(0.0f, 1.0f, 0.0f);
	// First triangle
	addVertex(p4, normal);
	addVertex(p3, normal);
	addVertex(p8, normal);
	// Second triangle
	addVertex(p4, normal);
	addVertex(p8, normal);
	addVertex(p7, normal);

	// Bottom
	normal = glm::vec3(0.0f, -1.0f, 0.0f);
	// First triangle
	addVertex(p6, normal);
	addVertex(p5, normal);
	addVertex(p2, normal);
	// Second triangle
	addVertex(p6, normal);
	addVertex(p2, normal);
	addVertex(p1, normal);
}

void	Chunk::addVertex(glm::vec3 pos, glm::vec3 normal) {
	mesh.insert(mesh.end(), { pos.x, pos.y, pos.z, normal.x, normal.y, normal.z });
}
