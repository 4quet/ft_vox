/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:23:42 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/26 15:54:26 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_HPP
# define CHUNK_HPP

# include "FastNoise.h"
# include "Shader.class.hpp"
# include "Block.hpp"
# include <vector>

struct AdjacentBlocks
{
	bool right, left, top, bottom, front, back;
};

class Chunk
{
	public:
		Chunk();
		~Chunk();

		void	update();
		void	render();

		void	setupLandscape();

		void	createMesh();
		void	createCube(float, float, float, AdjacentBlocks &);

		void	addVertex(glm::vec3 pos, glm::vec3 normal);

		glm::vec3	getPosition() const;
		void		setPosition(glm::vec3);

		size_t		getActiveBlocks() const;

		void		setHeightMap(float, float, float);

		static 	FastNoise	sNoise;

	private:
		Block***	_blocks;
		size_t		_activeBlocks;

		size_t		_totalVertices;

		float		_heightMap[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

		glm::vec3	_position;

		GLuint		VAO;
		GLuint		VBO;

		std::vector<float>	mesh;

};

#endif /* CHUNK_HPP */
