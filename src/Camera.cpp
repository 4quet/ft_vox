/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/25 13:39:17 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/20 17:44:12 by tpierron         ###   ########.fr       */
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
    static float speed = 0.25f;

    switch(action) {
        case Action::FORWARD: dy++; break;
        case Action::BACKWARD: dy--; break;
        case Action::LEFT: dx--; break;
        case Action::RIGHT: dx++; break;
        case Action::UP: eyeVec.y++; break;
        case Action::DOWN: eyeVec.y--; break;
        case Action::TOGGLESPEED: speed = (speed == 0.25f) ? 5.f : 0.25f; break;
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
    glm::vec3 ray(matrix[0][2], matrix[1][2], matrix[2][2]);

    return ray;
}


bool     Camera::findBlockInchunk(glm::vec3 ray, float n, Chunk & chunk) {
    glm::vec3 chunkPos = chunk.getPosition();
    for (float i = n; i < CHUNK_RENDER_SIZE * 2; i += BLOCK_RENDER_SIZE) {
        glm::vec3 posCheck = eyeVec + ray * i;
        if (posCheck.x < chunkPos.x && posCheck.x > chunkPos.x + CHUNK_RENDER_SIZE &&
            posCheck.y < chunkPos.y && posCheck.y > chunkPos.y + CHUNK_RENDER_SIZE &&
            posCheck.z < chunkPos.z && posCheck.z > chunkPos.z + CHUNK_RENDER_SIZE) {
                return false;
        }

        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                for (int z = 0; z < CHUNK_SIZE; z++)
                {
                    if (posCheck.x > chunkPos.x + x * BLOCK_RENDER_SIZE && posCheck.x < chunkPos.x + x * BLOCK_RENDER_SIZE + BLOCK_RENDER_SIZE &&
                    	posCheck.y > chunkPos.y + y * BLOCK_RENDER_SIZE && posCheck.y < chunkPos.y + y * BLOCK_RENDER_SIZE + BLOCK_RENDER_SIZE &&
                        posCheck.z > chunkPos.z + z * BLOCK_RENDER_SIZE && posCheck.z < chunkPos.z + z * BLOCK_RENDER_SIZE + BLOCK_RENDER_SIZE &&
                        chunk.getBlock(x, y, z).getBlockType() != BLOCKTYPE_INACTIVE) {
                            
                        std::cout  << chunkPos.x + x * BLOCK_RENDER_SIZE << " : " << chunkPos.y + y * BLOCK_RENDER_SIZE << " : " << chunkPos.z + z * BLOCK_RENDER_SIZE << std::endl;
                        chunk.getBlock(x, y, z).setBlockType(BLOCKTYPE_INACTIVE);
                        chunk.rebuild();
                        return true;
                    }
                }
            }
        }
        return false;
    }
    return false;
}

void        Camera::getPointedChunk(std::map<float, Chunk*> & chunks) {
    glm::vec3 ray = getRay();
    glm::vec3 startPoint = eyeVec;
    
    // glm::vec3 step = ray * static_cast<float>(CHUNK_RENDER_SIZE);
    // std::cout << chunks.size() << std::endl;
    for (float i = 0.f; i < 10.f * CHUNK_RENDER_SIZE; i += CHUNK_RENDER_SIZE) {
        glm::vec3 posCheck = startPoint + ray * i;
        // std::cout  << "posCheck: " posCheck.x << " : " << posCheck.y << " : " << posCheck.z << std::endl;
        std::cout << i << std::endl;
        for (std::map<float, Chunk*>::iterator it = chunks.begin(); it != chunks.end(); ++it) {
            // std::cout << it->second->getActiveBlocks() << std::endl;
            glm::vec3 chunkPos = it->second->getPosition();
            // std::cout << "|" << std::endl;
            // std::cout << "chunk: " << chunkPos.x << " : " << chunkPos.y << " : " << chunkPos.z << std::endl;
            // std::cout << "pos: " << posCheck.x << " : " << posCheck.y << " : " << posCheck.z << std::endl;
            if (posCheck.x > chunkPos.x && posCheck.x < chunkPos.x + CHUNK_RENDER_SIZE &&
                posCheck.y > chunkPos.y && posCheck.y < chunkPos.y + CHUNK_RENDER_SIZE &&
                posCheck.z > chunkPos.z && posCheck.z < chunkPos.z + CHUNK_RENDER_SIZE) {
                
                float dist = glm::distance(posCheck, chunkPos);
                std::cout  << dist  << std::endl;
                bool b = findBlockInchunk(ray, i - 1, *it->second);
                // if (b)
                //     return;
                // else
                //     continue;
            }
        }
    }
}
