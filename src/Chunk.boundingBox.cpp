/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.boundingBox.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/10 16:09:11 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/11 21:06:01 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk.hpp"

bool	Chunk::isBBoxBuilt() const { return _bboxBuilt; }

void	Chunk::renderBoundingBox() {
	if (_bboxBuilt)
	{
		glColorMask(false, false, false, false); 
		glDepthMask(GL_FALSE); 
		glBindVertexArray(bboxVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glColorMask(true, true, true, true); 
		glDepthMask(GL_TRUE); 
	}
}

void	Chunk::buildBoundingBox() {
	float	chunkRenderSize = CHUNK_SIZE * BLOCK_RENDER_SIZE;
	glm::vec3	p1(_position.x, _position.y, _position.z);
	glm::vec3	p2(_position.x + chunkRenderSize, _position.y, _position.z);
	glm::vec3	p3(_position.x + chunkRenderSize, _position.y + chunkRenderSize, _position.z);
	glm::vec3	p4(_position.x, _position.y + chunkRenderSize, _position.z);

	glm::vec3	p5(_position.x, _position.y, _position.z - chunkRenderSize);
	glm::vec3	p6(_position.x + chunkRenderSize, _position.y, _position.z - chunkRenderSize);
	glm::vec3	p7(_position.x + chunkRenderSize, _position.y + chunkRenderSize, _position.z - chunkRenderSize);
	glm::vec3	p8(_position.x, _position.y + chunkRenderSize, _position.z - chunkRenderSize);

	// FRONT
	bboxMesh.insert(bboxMesh.begin(), { p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z });
	bboxMesh.insert(bboxMesh.begin(), { p1.x, p1.y, p1.z, p3.x, p3.y, p3.z, p4.x, p4.y, p4.z });

	// BACK
	bboxMesh.insert(bboxMesh.begin(), { p5.x, p5.y, p5.z, p6.x, p6.y, p6.z, p7.x, p7.y, p7.z });
	bboxMesh.insert(bboxMesh.begin(), { p5.x, p5.y, p5.z, p7.x, p7.y, p7.z, p8.x, p8.y, p8.z });

	// LEFT
	bboxMesh.insert(bboxMesh.begin(), { p2.x, p2.y, p2.z, p5.x, p5.y, p5.z, p8.x, p8.y, p8.z });
	bboxMesh.insert(bboxMesh.begin(), { p2.x, p2.y, p2.z, p8.x, p8.y, p8.z, p3.x, p3.y, p3.z });

	// RIGHT
	bboxMesh.insert(bboxMesh.begin(), { p6.x, p6.y, p6.z, p1.x, p1.y, p1.z, p4.x, p4.y, p4.z });
	bboxMesh.insert(bboxMesh.begin(), { p6.x, p6.y, p6.z, p4.x, p4.y, p4.z, p7.x, p7.y, p7.z });

	// TOP
	bboxMesh.insert(bboxMesh.begin(), { p4.x, p4.y, p4.z, p3.x, p3.y, p3.z, p8.x, p8.y, p8.z });
	bboxMesh.insert(bboxMesh.begin(), { p4.x, p4.y, p4.z, p8.x, p8.y, p8.z, p7.x, p7.y, p7.z });

	// BOTTOM
	bboxMesh.insert(bboxMesh.begin(), { p6.x, p6.y, p6.z, p5.x, p5.y, p5.z, p2.x, p2.y, p2.z });
	bboxMesh.insert(bboxMesh.begin(), { p6.x, p6.y, p6.z, p2.x, p2.y, p2.z, p1.x, p1.y, p1.z });

	// OpenGL
	glGenVertexArrays(1, &bboxVAO);
	glGenBuffers(1, &bboxVBO);

	glBindVertexArray(bboxVAO);

	glBindBuffer(GL_ARRAY_BUFFER, bboxVBO);
	glBufferData(GL_ARRAY_BUFFER, bboxMesh.size() * sizeof(float), &bboxMesh[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	_bboxBuilt = true;
}

