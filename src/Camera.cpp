/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/25 13:39:17 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/10 14:40:58 by tpierron         ###   ########.fr       */
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
    float dz = 0.f;
    static float speed = 1.f;

    switch(action) {
        case Action::FORWARD: dy++; break;
        case Action::BACKWARD: dy--; break;
        case Action::LEFT: dx--; break;
        case Action::RIGHT: dx++; break;
        case Action::UP: dz--; break;
        case Action::DOWN: dz++; break;
        case Action::TOGGLESPEED: speed = (speed == 1.f) ? 5.f : 1.f; break;
        default: break;
    }
    glm::vec3 forward(matrix[0][2], matrix[1][2], matrix[2][2]);
    glm::vec3 strafe(matrix[0][0], matrix[1][0], matrix[2][0]);
    glm::vec3 vertical(matrix[0][1], matrix[1][1], matrix[2][1]);
    
    eyeVec += (-dy * forward + dx * strafe + -dz * vertical) * speed;
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
    translate = glm::translate(translate, -eyeVec);

    matrix = rollMat * yawMat * translate;
}

glm::mat4	Camera::getMatrix() const {
	return this->matrix;
}

glm::vec3   Camera::getPosition() const {
  //  std::cout << eyeVec.x << " : " << eyeVec.y << " : " << eyeVec.z << std::endl;
    return  eyeVec;
}
