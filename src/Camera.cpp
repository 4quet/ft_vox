/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/25 13:39:17 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/25 14:54:19 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./Camera.hpp"

Camera::Camera() {
    eyeVec = glm::vec3(1.f);
    matrix = glm::mat4(1.f);
    // matrix = glm::translate(matrix, glm::vec3(0.f, 50.f, 0.f));
    yaw = 0.f;
    roll = 0.f;
    mousePosition = glm::vec2(1.f);
    updateMatrix();
}

Camera::~Camera() {}

void		Camera::move(Action::Enum action, int mouseX, int mouseY) {
    float dx = 0.f;
    float dy = 0.f;

    switch(action) {
        case Action::FORWARD: dy--; break;
        case Action::BACKWARD: dy++; break;
        case Action::LEFT: dx++; break;
        case Action::RIGHT: dx--; break;
        default: break;
    }
    glm::vec3 forward(matrix[0][2], matrix[1][2], matrix[2][2]);
    glm::vec3 strafe(matrix[0][0], matrix[1][0], matrix[2][0]);
    
    const float speed = 1.f;
    eyeVec += (-dy * forward + dx * strafe) * speed;
    manageMouse(glm::vec2(mouseX, mouseY));
    updateMatrix();
}

void        Camera::manageMouse(glm::vec2 mousePos) {
    float sensitivity = 1.f;
    glm::vec2 mouseDelta = mousePos - mousePosition;

    yaw +=  sensitivity * mouseDelta.x;
    roll +=  sensitivity * mouseDelta.y;
    mousePosition = mousePos;
}

void        Camera::updateMatrix() {
    glm::mat4 rollMat = glm::mat4(1.f);
    glm::mat4 yawMat = glm::mat4(1.f);
    glm::mat4 translate = glm::mat4(1.f);

    rollMat = glm::rotate(rollMat, glm::radians(roll), glm::vec3(1.f, 0.f, 0.f));
    yawMat = glm::rotate(yawMat, glm::radians(yaw), glm::vec3(0.f, 1.f, 0.f));
    translate = glm::translate(translate, eyeVec);

    matrix = rollMat * yawMat * translate;
}

glm::mat4	Camera::getMatrix() const {
	return this->matrix;
}
