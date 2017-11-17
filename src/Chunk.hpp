/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:23:42 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/17 18:21:06 by lfourque         ###   ########.fr       */
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
	bool	right, left, top, bottom, front, back;
	bool	everywhere() { return (right && left && top && bottom && front && back); }
};

class Chunk
{
	public:
		Chunk(glm::vec3);
		~Chunk();

		void		setup();
		void		buildMesh();
		void		render();

		void		buildBoundingBox();
		void		renderBoundingBox();

		glm::vec3 const &	getPosition() const;
		size_t		getActiveBlocks() const;
		Block &		getBlock(int, int, int) const;
		bool		isVisible() const;
		bool		isSetup() const;
		bool		isBuilt() const;
		bool		isBBoxBuilt() const;

		void		setPosition(glm::vec3);
		void		setVisibility(bool);

		static void		loadTexturesAtlas(std::string file);
		static void		setUVs(unsigned int width, unsigned int height, unsigned int nbr);

		static 	FastNoise	sNoise;

	private:
		Block***						_blocks;
		size_t							_activeBlocks;
		size_t							_totalVertices;
		glm::vec3						_position;
		bool							_visible;
		bool							_setup;
		bool							_built;
		bool							_bboxBuilt;
		float							_halfBlockSize;

		GLuint							VAO;
		GLuint							VBO;
		std::vector<float>				mesh;
		std::vector<float>				waterMesh;
		GLuint							bboxVAO;
		GLuint							bboxVBO;
		std::vector<float>				bboxMesh;

		static unsigned int				texturesID;
		static std::vector<glm::vec2>	uvs;

		Chunk();

		void	fillMesh();
		void	createCube(float, float, float, AdjacentBlocks &, BlockType);
		void	createFace(glm::vec3 point, Faces::Enum face, BlockType type);
		void	addFace(glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec3 ,std::vector<glm::vec3> &, BlockType);

		void	getFaceUVs(Faces::Enum face, BlockType t, std::vector<glm::vec3> &) const;
};

#endif
