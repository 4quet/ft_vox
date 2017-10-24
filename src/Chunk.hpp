/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:23:42 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/23 18:09:16 by lfourque         ###   ########.fr       */
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

		void	createMesh();
		void	createCube(float x, float y, float z);

		void	addVertex(glm::vec3 pos, glm::vec3 normal);

	private:
		Block***	_blocks;
		int			_activeBlocks;

		GLuint		VAO;
		GLuint		VBO;

		std::vector<float>	mesh;
};

#endif /* CHUNK_HPP */
