/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/25 13:39:17 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/24 12:13:43 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"./Camera.hpp"

Camera::Camera() : eyeVec(12.234f, 58.712f, -11.444f), yaw(0.f),
                    roll(0.f), mousePosition(0.f) {
    updateMatrix();
}

Camera::~Camera() {}

void		Camera::move(std::vector<Action::Enum> & actions, int mouseX, int mouseY) {
    float dx = 0.f;
    float dy = 0.f;
    float dz = 0.f;
    float speed = 0.025f;
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
    glm::vec3 ray(-matrix[0][2], -matrix[1][2], -matrix[2][2]);
    return ray;
}

static int      sign(float x) { return (x > 0) ? 1 : ((x < 0) ? -1 : 0); }
static float    intBound(float s, float ds) { return ( ds > 0 ? (ceil(s) - s) / ds : (s - floor(s)) / -ds ); }

void        Camera::deleteBlock(std::map<float, Chunk*> & chunks) {      
    glm::vec3   direction = getRay();
    glm::vec3   origin = eyeVec;
    float       range = 10.f;
            
    float oX = origin.x;
    float oY = origin.y;
    float oZ = origin.z;
    
    float x = floor(oX);
    float y = floor(oY);
    float z = floor(oZ);
    
    float dX = direction.x;
    float dY = direction.y;
    float dZ = direction.z;

    int stepX = sign(dX);
    int stepY = sign(dY);
    int stepZ = sign(dZ);

    float tMaxX = intBound(oX, dX);
    float tMaxY = intBound(oY, dY);
    float tMaxZ = intBound(oZ, dZ);

    float deltaX = stepX / dX;
    float deltaY = stepY / dY;
    float deltaZ = stepZ / dZ;

    while (fabs(oX - x) < range && fabs(oY - y) < range && fabs(oZ - z) < range) {
        if(tMaxX <= tMaxY && tMaxX <= tMaxZ) {
            x += stepX;
            tMaxX += deltaX;
        } else if(tMaxY <= tMaxX && tMaxY <= tMaxZ) {
            y += stepY;
            tMaxY += deltaY;
        } else {
            z += stepZ;
            tMaxZ += deltaZ;
        }

        for (std::map<float, Chunk*>::iterator it = chunks.begin(); it != chunks.end(); ++it) {      
            Chunk & chunk = *it->second;
            glm::vec3  chunkPos = chunk.getPosition();

            if (x >= chunkPos.x && x < chunkPos.x + CHUNK_RENDER_SIZE &&
                y >= chunkPos.y && y < chunkPos.y + CHUNK_RENDER_SIZE &&
                z >= chunkPos.z && z < chunkPos.z + CHUNK_RENDER_SIZE) {

                    for (int cx = 0; cx < CHUNK_SIZE; cx++) {
                        for (int cy = 0; cy < CHUNK_SIZE; cy++) {
                            for (int cz = 0; cz < CHUNK_SIZE; cz++) {
                                
                                if (x == chunkPos.x + cx * BLOCK_RENDER_SIZE &&
                                    y == chunkPos.y + cy * BLOCK_RENDER_SIZE &&
                                    z == chunkPos.z + cz * BLOCK_RENDER_SIZE &&
                                    chunk.getBlock(cx, cy, cz) != BlockTypes::INACTIVE) {

                                        chunk.getBlock(cx, cy, cz) = BlockTypes::INACTIVE;
                                		if (chunk.right) chunk.right->rebuild();
                                    	if (chunk.left) chunk.left->rebuild();
                    					if (chunk.top) chunk.top->rebuild();
                						if (chunk.bottom) chunk.bottom->rebuild();
                        				if (chunk.front) chunk.front->rebuild();
                                        if (chunk.back) chunk.back->rebuild();
                                        chunk.rebuild();
                                        return;
                                }
                            }
                        }
                    }
            }
        }
    }
    
}
    