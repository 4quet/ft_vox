/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:23:42 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/22 11:09:10 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_HPP
# define CHUNK_HPP

# include <iostream>
# include <fstream>
# include <vector>
# include <OpenGL/gl3.h>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>
# include "constants.hpp"

typedef	std::tuple<float, float, float>	index3D;

class Chunk
{
	public:
		Chunk(glm::vec3);
		~Chunk();

		void				setup();
		void				buildMesh();
		void				rebuild();
		void				render();
		void				reset();

		glm::vec3 const &	getPosition() const;
		size_t				getActiveBlocks() const;
		BlockTypes::Enum &	getBlock(int, int, int) const;
		bool				isSetup() const;
		bool				isLandscapeSetup() const;
		bool				isBuilt() const;

		void				setPosition(glm::vec3);
		void				setLandscapeSetup(bool);

		static void			loadTexturesAtlas(std::string file);
		static void			setUVs(unsigned int width, unsigned int height, unsigned int nbr);

		Chunk *left;
		Chunk *right;
		Chunk *top;
		Chunk *bottom;
		Chunk *front;
		Chunk *back;

	private:
		Chunk();

		BlockTypes::Enum***					_blocks;
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


		bool	isNeighborActive(Chunk *, int, int, int) const;

		void	fillMesh();
		void	createFace(glm::vec3 point, Faces::Enum face, BlockTypes::Enum type);
		void	addFace(glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec3, std::vector<glm::vec2> &, BlockTypes::Enum);

		void	getFaceUVs(Faces::Enum face, BlockTypes::Enum t, std::vector<glm::vec2> &) const;
};

#endif
