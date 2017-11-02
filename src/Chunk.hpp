/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:23:42 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/02 15:53:22 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_HPP
# define CHUNK_HPP

# include "FastNoise.h"
# include "Shader.class.hpp"
# include "Block.hpp"
# include <vector>

typedef	std::tuple<float, float, float>	index3D;

struct AdjacentBlocks
{
	bool right, left, top, bottom, front, back;
};

class Chunk
{
	public:
		Chunk(glm::vec3);
		~Chunk();

		void	update();
		void	render();

		void	setupLandscape();

		void	createMesh();
		void	createCube(float, float, float, AdjacentBlocks &, BlockType);

		void	addVertex(glm::vec3, glm::vec3, glm::vec3);

		glm::vec3	getPosition() const;
		void		setPosition(glm::vec3);

		size_t		getActiveBlocks() const;

		void		setHeightMap();

		void		setVisibility(bool);
		bool		isVisible() const;

		void		setup();
		bool		isSetup() const;

		static 	FastNoise	sNoise;

	private:
		Chunk();

		Block***	_blocks;
		size_t		_activeBlocks;

		size_t		_totalVertices;

		float		_heightMap[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

		glm::vec3	_position;
		bool		_visible;
		bool		_setup;

		GLuint		VAO;
		GLuint		VBO;

		std::vector<float>	mesh;

};

#endif /* CHUNK_HPP */
