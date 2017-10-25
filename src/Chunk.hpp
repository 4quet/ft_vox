/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:23:42 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/25 16:53:08 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_HPP
# define CHUNK_HPP

# include "FastNoise.h"
# include "Shader.class.hpp"
# include "Block.hpp"
# include <vector>

class Chunk
{
	public:
		Chunk();
		~Chunk();

		void	update();
		void	render();

		void	setupLandscape();

		void	createMesh();
		void	createCube(float x, float y, float z);

		void	addVertex(glm::vec3 pos, glm::vec3 normal);

		glm::vec3	getPosition() const;
		void		setPosition(glm::vec3);

		size_t		getActiveBlocks() const;

		void		setHeightMap(float, float);

		static 	FastNoise	sNoise;

	private:
		Block***	_blocks;
		size_t		_activeBlocks;

		float		_heightMap[CHUNK_SIZE][CHUNK_SIZE];

		glm::vec3	_position;

		GLuint		VAO;
		GLuint		VBO;

		std::vector<float>	mesh;

};

#endif /* CHUNK_HPP */
