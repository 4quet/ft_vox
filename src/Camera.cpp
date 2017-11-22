/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/25 13:39:17 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/22 10:47:40 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./Camera.hpp"

Camera::Camera() : eyeVec(12.234f, 23.712f, -11.444f), yaw(0.f),
                    roll(0.f), mousePosition(0.f) {
    updateMatrix();
}

Camera::~Camera() {}

void		Camera::move(std::vector<Action::Enum> & actions, int mouseX, int mouseY) {
    float dx = 0.f;
    float dy = 0.f;
    float dz = 0.f;
    float speed = 0.05f;
    bool moveUp = false;
    bool moveDown = false;

    for(unsigned int i = 0; i < actions.size(); i++) {
        switch(actions[i]) {
            case Action::FORWARD: dy++; break;
            case Action::BACKWARD: dy--; break;
            case Action::LEFT: dx--; break;
            case Action::RIGHT: dx++; break;
            case Action::UP: moveUp = true; break;
            case Action::DOWN: moveDown = true; break;
            case Action::TOGGLESPEED: speed *= 20.f; break;
            default: break;
        }
    }
    glm::vec3 forward(matrix[0][2], matrix[1][2], matrix[2][2]);
    glm::vec3 strafe(matrix[0][0], matrix[1][0], matrix[2][0]);
    glm::vec3 vertical(matrix[0][1], matrix[1][1], matrix[2][1]);
    
    eyeVec += (-dy * forward + dx * strafe + -dz * vertical) * speed;
    if (moveUp)
        eyeVec.y += speed;
    else if (moveDown)
        eyeVec.y -= speed;
        
    manageMouse(glm::vec2(mouseX, mouseY));
    updateMatrix();
}

void        Camera::manageMouse(glm::vec2 mouseDelta) {
    if (mousePosition == mouseDelta)
        return;
        
    float sensitivity = 1.f;

    yaw +=  sensitivity * mouseDelta.x;
    roll +=  sensitivity * mouseDelta.y;
    mousePosition = mouseDelta;
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
  return  eyeVec;
}

glm::vec3		Camera::getRay() const {
    glm::vec3 ray(matrix[0][2], matrix[1][2], matrix[2][2]);
    return ray;
}


bool     Camera::findBlockInchunk(glm::vec3 ray, glm::vec3 startPoint, Chunk & chunk) {
    glm::vec3 chunkPos = chunk.getPosition();
    for (float i = 0; i < CHUNK_RENDER_SIZE; i += BLOCK_RENDER_SIZE) {
        glm::vec3 posCheck = startPoint + ray * -i;
        if (posCheck.x < chunkPos.x && posCheck.x > chunkPos.x + CHUNK_RENDER_SIZE &&
            posCheck.y < chunkPos.y && posCheck.y > chunkPos.y + CHUNK_RENDER_SIZE &&
            posCheck.z < chunkPos.z && posCheck.z > chunkPos.z + CHUNK_RENDER_SIZE) {
                return false;
        }
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    if (posCheck.x > chunkPos.x + x * BLOCK_RENDER_SIZE && posCheck.x < chunkPos.x + x * BLOCK_RENDER_SIZE + BLOCK_RENDER_SIZE &&
                    	posCheck.y > chunkPos.y + y * BLOCK_RENDER_SIZE && posCheck.y < chunkPos.y + y * BLOCK_RENDER_SIZE + BLOCK_RENDER_SIZE &&
                        posCheck.z > chunkPos.z + z * BLOCK_RENDER_SIZE && posCheck.z < chunkPos.z + z * BLOCK_RENDER_SIZE + BLOCK_RENDER_SIZE &&
                        chunk.getBlock(x, y, z) != BLOCKTYPE_INACTIVE) {
                            
                        chunk.getBlock(x, y, z) = BLOCKTYPE_INACTIVE;
                        chunk.rebuild();
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void        Camera::deleteBlock(std::map<float, Chunk*> & chunks) {
    glm::vec3 ray = getRay();
    glm::vec3 startPoint = eyeVec;
    
    for (float i = 0.f; i < 2.f * CHUNK_RENDER_SIZE; i += BLOCK_RENDER_SIZE / 2) {
        glm::vec3 posCheck = startPoint + (ray * -i);
        for (std::map<float, Chunk*>::iterator it = chunks.begin(); it != chunks.end(); ++it) {
            glm::vec3 chunkPos = it->second->getPosition();
            if (posCheck.x > chunkPos.x && posCheck.x < chunkPos.x + CHUNK_RENDER_SIZE &&
                posCheck.y > chunkPos.y && posCheck.y < chunkPos.y + CHUNK_RENDER_SIZE &&
                posCheck.z > chunkPos.z && posCheck.z < chunkPos.z + CHUNK_RENDER_SIZE) {
                    
                bool b = findBlockInchunk(ray, startPoint + ray * -(i - static_cast<float>(BLOCK_RENDER_SIZE)), *it->second);
                if (b)
                    return;
                else
                    continue;
            }
        }
    }
}
