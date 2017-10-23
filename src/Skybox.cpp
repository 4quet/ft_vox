/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Skybox.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 10:37:37 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/23 11:42:56 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Skybox.hpp"
# define STB_IMAGE_IMPLEMENTATION
# include "./stb_image.h"

Skybox::Skybox(std::string directory) {

	getFacesPath(directory);
	loadTextures();
}

Skybox::~Skybox() {
	return;
}

void		Skybox::getFacesPath(std::string directory) {
	std::vector<std::string> paths;
	paths.push_back(directory + "/right.jpg" );
	paths.push_back(directory + "/left.jpg" );
	paths.push_back(directory + "/top.jpg" );
	paths.push_back(directory + "/bottom.jpg" );
	paths.push_back(directory + "/back.jpg" );
	paths.push_back(directory + "/front.jpg" );

	for (unsigned int i = 0; i < 6; i++) {
		textures_faces.push_back(paths[i].c_str());
	}
}

void		Skybox::loadTextures() {

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	unsigned char *data;
	for(GLuint i = 0; i < 6; i++)
	{
		data = stbi_load(textures_faces[i], &width, &height, &nrChannels, 0);
		if(data) {
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else {
			std::cout << "Loading Skybox texture failed: " << textures_faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}