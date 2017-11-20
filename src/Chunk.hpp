/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:23:42 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/20 16:15:29 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_HPP
# define CHUNK_HPP

# include "FastNoise.h"
# include "Shader.class.hpp"
# include "Block.hpp"
# include <vector>

typedef	std::tuple<float, float, float>	index3D;

class Chunk
{
	public:
		Chunk(glm::vec3);
		~Chunk();

		void		setup();
		void		buildMesh();
		void		rebuild();
		void		render();
		void		reset();

		glm::vec3 const &	getPosition() const;
		size_t		getActiveBlocks() const;
		Block &		getBlock(int, int, int) const;
		bool		isSetup() const;
		bool		isLandscapeSetup() const;
		bool		isBuilt() const;

		void		setPosition(glm::vec3);
		void		setLandscapeSetup(bool);

		static void		loadTexturesAtlas(std::string file);
		static void		setUVs(unsigned int width, unsigned int height, unsigned int nbr);

		Chunk *	left;
		Chunk *	right;
		Chunk *	top;
		Chunk *	bottom;
		Chunk *	front;
		Chunk *	back;

	private:
		Block***						_blocks;
		size_t							_activeBlocks;
		size_t							_totalVertices;
		glm::vec3						_position;
		bool							_setup;
		bool							_landscapeSetup;
		bool							_built;
		float							_halfBlockSize;


		GLuint							VAO;
		GLuint							VBO;
		std::vector<float> 				mesh;
		std::vector<float>				waterMesh;

		static unsigned int				texturesID;
		static std::vector<glm::vec2>	uvs;

		Chunk();

		bool		isNeighborActive(Chunk *, int, int, int) const;

		void	fillMesh();
		void	createFace(glm::vec3 point, Faces::Enum face, BlockType type);
		void	addFace(glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec3, std::vector<glm::vec2> &, BlockType);

		void	getFaceUVs(Faces::Enum face, BlockType t, std::vector<glm::vec2> &) const;
};

#endif
