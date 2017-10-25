/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 11:23:42 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/25 10:30:14 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNK_HPP
# define CHUNK_HPP

# include "Shader.class.hpp"
# include "Block.hpp"
# include "ChunkManager.hpp"
# include <vector>

class Chunk
{
	public:
		Chunk(ChunkManager & m);
		~Chunk();

		void	update();
		void	render(Shader & shader);

		void	setupLandscape();

		void	createMesh();
		void	createCube(float x, float y, float z);

		void	addVertex(glm::vec3 pos, glm::vec3 normal);

	private:
		Block***	_blocks;
		int			_activeBlocks;

		ChunkManager &	_chunkManager;

		GLuint		VAO;
		GLuint		VBO;

		std::vector<float>	mesh;

		Chunk();
};

#endif /* CHUNK_HPP */
