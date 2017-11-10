/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Skybox.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 10:37:37 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/10 10:49:23 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Skybox.hpp"
# define STB_IMAGE_IMPLEMENTATION
# include "./stb_image.h"

Skybox::Skybox(std::string directory) {

	shader = new Shader("./src/shaders/skybox.glvs", "./src/shaders/skybox.glfs");
	getFacesPath(directory);
	loadTextures();
	setBuffers();
}

Skybox::~Skybox() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteTextures(1, &textureID);
	return;
}

void		Skybox::draw() {
	glDepthFunc(GL_LEQUAL);
	shader->use();
	shader->setInt("skybox", 0);
	
	shader->setView(glm::mat4(glm::mat3(shader->getViewMatrix())));

	glBindVertexArray(vao);
	glActiveTexture(textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void		Skybox::setBuffers() {
	float vertices[108] = {
		-2000.0f,  2000.0f, -2000.0f,
		-2000.0f, -2000.0f, -2000.0f,
		 2000.0f, -2000.0f, -2000.0f,
		 2000.0f, -2000.0f, -2000.0f,
		 2000.0f,  2000.0f, -2000.0f,
		-2000.0f,  2000.0f, -2000.0f,
	
		-2000.0f, -2000.0f,  2000.0f,
		-2000.0f, -2000.0f, -2000.0f,
		-2000.0f,  2000.0f, -2000.0f,
		-2000.0f,  2000.0f, -2000.0f,
		-2000.0f,  2000.0f,  2000.0f,
		-2000.0f, -2000.0f,  2000.0f,
	
		 2000.0f, -2000.0f, -2000.0f,
		 2000.0f, -2000.0f,  2000.0f,
		 2000.0f,  2000.0f,  2000.0f,
		 2000.0f,  2000.0f,  2000.0f,
		 2000.0f,  2000.0f, -2000.0f,
		 2000.0f, -2000.0f, -2000.0f,
	
		-2000.0f, -2000.0f,  2000.0f,
		-2000.0f,  2000.0f,  2000.0f,
		 2000.0f,  2000.0f,  2000.0f,
		 2000.0f,  2000.0f,  2000.0f,
		 2000.0f, -2000.0f,  2000.0f,
		-2000.0f, -2000.0f,  2000.0f,
	
		-2000.0f,  2000.0f, -2000.0f,
		 2000.0f,  2000.0f, -2000.0f,
		 2000.0f,  2000.0f,  2000.0f,
		 2000.0f,  2000.0f,  2000.0f,
		-2000.0f,  2000.0f,  2000.0f,
		-2000.0f,  2000.0f, -2000.0f,
	
		-2000.0f, -2000.0f, -2000.0f,
		-2000.0f, -2000.0f,  2000.0f,
		 2000.0f, -2000.0f, -2000.0f,
		 2000.0f, -2000.0f, -2000.0f,
		-2000.0f, -2000.0f,  2000.0f,
		 2000.0f, -2000.0f,  2000.0f
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, &vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));

	glBindVertexArray(0);
}

void		Skybox::getFacesPath(std::string directory) {
	textures_faces.push_back(directory + "/right.jpg" );
	textures_faces.push_back(directory + "/left.jpg" );
	textures_faces.push_back(directory + "/top.jpg" );
	textures_faces.push_back(directory + "/bottom.jpg" );
	textures_faces.push_back(directory + "/back.jpg" );
	textures_faces.push_back(directory + "/front.jpg" );
}

void		Skybox::loadTextures() {

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	unsigned char *data;
	stbi_set_flip_vertically_on_load(false);
	for(GLuint i = 0; i < 6; i++)
	{
		data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
		if(data) {
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else {
			std::cout << "Loading Skybox texture failed: " << textures_faces[i].c_str() << std::endl;
			stbi_image_free(data);
			glDeleteTextures(1, &textureID);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}