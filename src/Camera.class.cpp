/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/18 15:05:13 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/24 15:38:39 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./Camera.class.hpp"

Camera::Camera() {
	x = 0;
	y = 0;
    z = 0;
    
    matrix = glm::lookAt(
        glm::vec3(x, y, z),
        glm::vec3(0.f, y + 5, 0.f),
        glm::vec3(0.f, 1.f, 0.f)
    );
}

void    Camera::setCameraCrd(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    
    matrix = glm::lookAt(
        glm::vec3(x, y, z),
        glm::vec3(0.f, y + 5, 0.f),
        glm::vec3(0.f, 1.f, 0.f)
    );
}

void    Camera::setCameraX(float x) {
    
}

glm::mat4	Camera::getMatrix() const {
	return this->matrix;
}

