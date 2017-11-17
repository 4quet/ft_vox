/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/25 13:39:17 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/17 10:30:44 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./Camera.hpp"

Camera::Camera() {
    eyeVec = glm::vec3(1.f);
    // eyeVec += glm::vec3(0.f, 16.f, 0.f);
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

glm::vec3		Camera::getRay() const {
    glm::vec2 norm = glm::vec2(1.f);
    norm.x = mousePosition.x / (1024 * 0.5f) - 1.f;
    norm.y = mousePosition.y / (1024 * 0.5f) - 1.f;
    
    glm::mat4 proj = glm::perspective(45.f, 1.f, 0.1f, 4000.f);

    glm::vec4 screenPos = glm::vec4(norm.x, -norm.y, 1.f, 1.f);
    glm::mat4 invMat = glm::inverse(proj * matrix);
    glm::vec4 worldPos = invMat * screenPos;
    
    glm::vec3 ray = glm::normalize(glm::vec3(worldPos));
    
    return ray;
    //  std::cout << "Ray: " << ray.x << " : " << ray.y << " : " << ray.z << std::endl;
    //  std::cout << "EyeVec: " << eyeVec.x << " : " << eyeVec.y << " : " << eyeVec.z << std::endl;
}

void        Camera::getPointedChunk(std::map<float, Chunk*> & chunks) {
    glm::vec3 ray = getRay();
    glm::vec3 startPoint = eyeVec;
    
    glm::vec3 step = ray * static_cast<float>(CHUNK_SIZE * BLOCK_RENDER_SIZE);
    // std::cout << "step: " << step.x << " : " << step.y << " : " << step.z << " : " << std::endl;
    for (float i = 0.f; i < 100.f; i++) {
        for (std::map<float, Chunk*>::iterator it = chunks.begin(); it != chunks.end(); ++it) {
            glm::vec3 posCheck = startPoint + step * i;
            glm::vec3 chunkPos = it->second->getPosition();
            // std::cout << "startPoint: " << startPoint.x << " : " << startPoint.y << " : " << startPoint.z << " : " << std::endl;
            // std::cout << "ray: " << ray.x << " : " << ray.y << " : " << ray.z << " : " << std::endl;
            // std::cout << "posCheck: " << posCheck.x << " : " << posCheck.y << " : " << posCheck.z << " : " << std::endl;
            // std::cout << "chunkPos: " << chunkPos.x << " : " << chunkPos.y << " : " << chunkPos.z << " : " << std::endl;
            // std::cout << "chunkPos +: " << chunkPos.x + CHUNK_SIZE * BLOCK_RENDER_SIZE << " : " << chunkPos.y + CHUNK_SIZE * BLOCK_RENDER_SIZE << " : " << chunkPos.z + CHUNK_SIZE * BLOCK_RENDER_SIZE << " : " << std::endl;
            // std::cout << "verif: " << chunkPos.x + CHUNK_SIZE * BLOCK_RENDER_SIZE << std::endl;
            
            if (posCheck.x > chunkPos.x && posCheck.x < chunkPos.x + CHUNK_SIZE * BLOCK_RENDER_SIZE &&
                posCheck.y > chunkPos.y && posCheck.y < chunkPos.y + CHUNK_SIZE * BLOCK_RENDER_SIZE &&
                posCheck.z > chunkPos.z && posCheck.z < chunkPos.z + CHUNK_SIZE * BLOCK_RENDER_SIZE) {
                    
                std::cout << "Erase: " << chunkPos.x << " : " << chunkPos.y << " : " << chunkPos.z << " : " << std::endl;
                chunks.erase(it);
                // return;
            }
        }
    }
}