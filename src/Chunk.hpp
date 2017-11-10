/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:23:42 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/10 16:17:12 by tpierron         ###   ########.fr       */
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

	bool	everywhere() { return (right && left && top && bottom && front && back); }
};

class Chunk
{
	public:
		Chunk(glm::vec3);
		~Chunk();

		void	update();
		void	render();

		void	activeBlock(int x, int z, float height);

		void	fillMesh();
		void	buildMesh();
		bool	isBuilt() const;

		void					createCube(float, float, float, AdjacentBlocks &, BlockType);
		void					createFace(glm::vec3 point, Faces::Enum face, BlockType type);
		std::vector<glm::vec2>	getTriangleUVs(bool firstTriangle, Faces::Enum face, BlockType t) const;
		void					addTriangle(glm::vec3 , glm::vec3 , glm::vec3 , glm::vec3 , std::vector<glm::vec2> &);

		glm::vec3	getPosition() const;
		void		setPosition(glm::vec3);

		size_t		getActiveBlocks() const;

		void		setHeightMap();

		void		setVisibility(bool);
		bool		isVisible() const;

		void		setup();
		bool		isSetup() const;

		static void	loadTexturesAtlas(std::string file);
		static void	setUVs(unsigned int width, unsigned int height, unsigned int nbr);

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
		bool		_built;

		static unsigned int texturesID;
		static std::vector<glm::vec2> uvs;

		GLuint		VAO;
		GLuint		VBO;

		std::vector<float>	mesh;

};

#endif
