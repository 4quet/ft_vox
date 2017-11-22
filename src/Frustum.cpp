/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Frustum.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 13:53:42 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/22 11:19:58 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Frustum.hpp"

Frustum::Frustum(glm::mat4 projection) : projection(projection) {
	view = glm::mat4(1.0);
}

void	Frustum::setView(glm::mat4 v) {
	this->view = v;
}

void	Frustum::setPlanes() {
	glm::mat4 matrix = projection * view;

	planes[0][0] = matrix[0][3] + matrix[0][0];
	planes[0][1] = matrix[1][3] + matrix[1][0];
	planes[0][2] = matrix[2][3] + matrix[2][0];
	planes[0][3] = matrix[3][3] + matrix[3][0];
	
	planes[1][0] = matrix[0][3] - matrix[0][0];
	planes[1][1] = matrix[1][3] - matrix[1][0];
	planes[1][2] = matrix[2][3] - matrix[2][0];
	planes[1][3] = matrix[3][3] - matrix[3][0];
	
	planes[2][0] = matrix[0][3] - matrix[0][1];
	planes[2][1] = matrix[1][3] - matrix[1][1];
	planes[2][2] = matrix[2][3] - matrix[2][1];
	planes[2][3] = matrix[3][3] - matrix[3][1];
	
	planes[3][0] = matrix[0][3] + matrix[0][1];
	planes[3][1] = matrix[1][3] + matrix[1][1];
	planes[3][2] = matrix[2][3] + matrix[2][1];
	planes[3][3] = matrix[3][3] + matrix[3][1];
	
	planes[4][0] = matrix[0][3] + matrix[0][2];
	planes[4][1] = matrix[1][3] + matrix[1][2];
	planes[4][2] = matrix[2][3] + matrix[2][2];
	planes[4][3] = matrix[3][3] + matrix[3][2];
	
	planes[5][0] = matrix[0][3] - matrix[0][2];
	planes[5][1] = matrix[1][3] - matrix[1][2];
	planes[5][2] = matrix[2][3] - matrix[2][2];
	planes[5][3] = matrix[3][3] - matrix[3][2];
}

bool		Frustum::pointIn(float x, float y, float z) {
	for(unsigned int i = 0; i < 6; i++) {
		if(planes[i][0] * x + (CHUNK_SIZE / 2) + planes[i][1] * y + planes[i][2] * z + planes[i][3] <= -CHUNK_SIZE * BLOCK_RENDER_SIZE)
			return false;
	}
	return true;
}
