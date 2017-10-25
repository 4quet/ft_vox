/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:23:42 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/25 13:39:48 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_HPP
# define CHUNK_HPP

# include "Shader.class.hpp"
# include "Block.hpp"
# include <vector>

class Chunk
{
	public:
		Chunk();
		~Chunk();

		void	update();
		void	render(Shader & shader);

		void	setupLandscape();

		void	createMesh();
		void	createCube(float x, float y, float z);

		void	addVertex(glm::vec3 pos, glm::vec3 normal);

		glm::vec3	getPosition() const;
		void		setPosition(glm::vec3 pos);

		void		setHeightMap(float hm[CHUNK_SIZE][CHUNK_SIZE]);

	private:
		Block***	_blocks;
		int			_activeBlocks;

		float		_heightMap[CHUNK_SIZE][CHUNK_SIZE];

		glm::vec3	_position;

		GLuint		VAO;
		GLuint		VBO;

		std::vector<float>	mesh;

};

#endif /* CHUNK_HPP */
