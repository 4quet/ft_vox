/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:23:42 by lfourque          #+#    #+#             */
/*   Updated: 2017/12/01 17:52:44 by lfourque         ###   ########.fr       */
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
		void				render();
		void				reset();
		void				rebuild();
		void				buildMesh();

		glm::vec3 const &	getPosition() const;
		BlockTypes::Enum &	getBlock(int, int, int);
		size_t				getActiveBlocks() const;

		bool				isSetup() const;
		bool				isLandscapeSetup() const;

		void				setPosition(glm::vec3);
		void				setLandscapeSetup(bool);

		Chunk *				left;
		Chunk *				right;
		Chunk *				top;
		Chunk *				bottom;
		Chunk *				front;
		Chunk *				back;

		static void			loadTexturesAtlas(std::string file);
		static void			setUVs(unsigned int width, unsigned int height, unsigned int nbr);

	private:
		Chunk();

		glm::vec3						_position;

		BlockTypes::Enum				_blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
		size_t							_activeBlocks;
		size_t							_totalVertices;
		float							_halfBlockSize;

		bool							_setup;
		bool							_landscapeSetup;

		GLuint							VAO;
		GLuint							VBO;
		std::vector<float> 				mesh;
		std::vector<float>				waterMesh;

		static unsigned int				texturesID;
		static std::vector<glm::vec2>	uvs;

		void	fillMesh();
		void	createFace(glm::vec3 point, Faces::Enum face, BlockTypes::Enum & type);
		void	addFace(glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec3, std::vector<glm::vec2> &, BlockTypes::Enum &);
		void	getFaceUVs(Faces::Enum face, BlockTypes::Enum t, std::vector<glm::vec2> &) const;
		bool	isNeighborActive(Chunk *, int, int, int) const;
};

#endif
