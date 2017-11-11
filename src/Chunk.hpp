/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:23:42 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/11 17:35:20 by lfourque         ###   ########.fr       */
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

		glm::vec3	getPosition() const;
		size_t		getActiveBlocks() const;
		bool		isVisible() const;
		bool		isSetup() const;
		bool		isBuilt() const;

		void		setPosition(glm::vec3);
		void		setVisibility(bool);

		static 	FastNoise	sNoise;

	private:
		Block***						_blocks;
		size_t							_activeBlocks;
		size_t							_totalVertices;
		glm::vec3						_position;
		bool							_visible;
		bool							_setup;
		bool							_built;

		GLuint							VAO;
		GLuint							VBO;
		std::vector<float>				mesh;

		static unsigned int				texturesID;
		static std::vector<glm::vec2>	uvs;

		Chunk();

		void					setupLandscape();
		void					fillMesh();
		void					createCube(float, float, float, AdjacentBlocks &, BlockType);
		void					createFace(glm::vec3 point, Faces::Enum face, BlockType type);
		void					addTriangle(glm::vec3 , glm::vec3 , glm::vec3 , glm::vec3 , std::vector<glm::vec2> &);

		std::vector<glm::vec2>	getTriangleUVs(bool firstTriangle, Faces::Enum face, BlockType t) const;
		static void				loadTexturesAtlas(std::string file);
		static void				setUVs(unsigned int width, unsigned int height, unsigned int nbr);
};

#endif
