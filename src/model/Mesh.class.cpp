// /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.class.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/21 11:54:40 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/05 16:21:42 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mesh.class.hpp"

int	Mesh::i = 0;

void Mesh::printJointMatrices(Joint *joint) {
	glm::mat4 at = joint->getAnimatedTransform();
	glm::mat4 inv = joint->getInverseBindTransform();
	std::cout << "Bones: " << joint->index << std::endl;
	std::cout << "Animated Transform" << std::endl;
	std::cout << at[0][0] <<  "\t\t" << at[0][1]  <<  "\t\t" << at[0][2]  <<  "\t\t" << at[0][3] << std::endl;
	std::cout << at[1][0] <<  "\t\t" << at[1][1]  <<  "\t\t" << at[1][2]  <<  "\t\t" << at[1][3] << std::endl;
	std::cout << at[2][0] <<  "\t\t" << at[2][1]  <<  "\t\t" << at[2][2]  <<  "\t\t" << at[2][3] << std::endl;
	std::cout << at[3][0] <<  "\t\t" << at[3][1]  <<  "\t\t" << at[3][2]  <<  "\t\t" << at[3][3] << std::endl;
	std::cout << "Inverse Transform" << std::endl;
	std::cout << inv[0][0] <<  "\t\t" << inv[0][1]  <<  "\t\t" << inv[0][2]  <<  "\t\t" << inv[0][3] << std::endl;
	std::cout << inv[1][0] <<  "\t\t" << inv[1][1]  <<  "\t\t" << inv[1][2]  <<  "\t\t" << inv[1][3] << std::endl;
	std::cout << inv[2][0] <<  "\t\t" << inv[2][1]  <<  "\t\t" << inv[2][2]  <<  "\t\t" << inv[2][3] << std::endl;
	std::cout << inv[3][0] <<  "\t\t" << inv[3][1]  <<  "\t\t" << inv[3][2]  <<  "\t\t" << inv[3][3] << std::endl;
	std::cout << " -------------------------------------------" << std::endl;
	for (unsigned int i = 0; i < joint->children.size(); i++) {
		printJointMatrices(joint->children[i]);
	}
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
			std::vector<Texture> textures, aiColor3D color, Joint* rootJoint, unsigned int jointNbr)
: vertices(vertices), indices(indices), textures(textures), color(color), rootJoint(rootJoint), jointNbr(jointNbr) {
	// Mesh::i++;
	// this->rootJoint->calcInverseBindTransform(glm::mat4());
	// printJointMatrices(&rootJoint);
	setupMesh();
	// for (unsigned int i = 0; i < vertices.size(); i++) {
		// std::cout << "bonesID: " << vertices[i].bonesID[0] << " : " << vertices[i].bonesID[1] << " : " << vertices[i].bonesID[2] << std::endl;
		// std::cout << "bonesWeigths: " << vertices[i].weigths[0] << " : " << vertices[i].weigths[1] << " : " << vertices[i].weigths[2] << std::endl;
	// }
	// std::cout << "MESH CONSTRUCT: " << ++Mesh::i << std::endl;
	return;
}

Mesh::Mesh(Mesh const & src) {
	
	// std::cout << "MESH CONSTRUCT: " << ++Mesh::i << std::endl;
	*this = src;
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->ebo);
	glDeleteBuffers(1, &this->ibo);
	glDeleteVertexArrays(1, &this->vao);
	for (unsigned int i = 0; i < this->textures.size(); i++) {
		glDeleteTextures(1, &textures[i].id);
	}
	delete this->rootJoint;
	Mesh::i--;
	// std::cout << "MESH DESTRUCT: " << Mesh::i << std::endl;
	return;
}

void	Mesh::setupMesh() {
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glGenBuffers(1, &this->ebo);

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bonesID));

	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(4, 3, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, weigths));
	glBindVertexArray(0);

	// std::vector<glm::vec2>      data;
	// data.push_back(glm::vec2(2, 3));
	// data.push_back(glm::vec2(15, 20));

	// unsigned int vbo;
	// glGenBuffers(1, &vbo);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * data.size(), &data[0], GL_STATIC_DRAW);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);


	// glEnableVertexAttribArray(5);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(0));
	// glBindBuffer(GL_ARRAY_BUFFER, 0);	
	// glVertexAttribDivisor(5, 1); 


	return;
}

void	Mesh::setInstanceBuffer(std::vector<glm::mat4> const & data) {

	glBindVertexArray(this->vao);
	glGenBuffers(1, &this->ibo);
	glBindBuffer(GL_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * data.size(), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindBuffer(GL_ARRAY_BUFFER, this->ibo);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(0));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(3 * sizeof(glm::vec4)));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	glVertexAttribDivisor(5, 1); 
	glVertexAttribDivisor(6, 1); 
	glVertexAttribDivisor(7, 1); 
	glVertexAttribDivisor(8, 1); 
	glBindVertexArray(0);
	
	glDeleteBuffers(1, &this->ibo);
}

void	Mesh::draw(Shader *shader, bool animated, unsigned int instanceCount) {

	unsigned int diffuseNbr = 1;
	unsigned int specularNbr = 1;
	for(unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string name = this->textures[i].type;
		std::string nbr = (name == "texture_diffuse") ? std::to_string(diffuseNbr++) : std::to_string(specularNbr++);
		glUniform1i(glGetUniformLocation(shader->getProgramID(), ("material." + name + nbr).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	if (textures.size() == 0)
		glUniform3f(glGetUniformLocation(shader->getProgramID(), "materialColor"), this->color.r, this->color.g, this->color.b);

	if (animated) {
		glm::mat4 *jointTransforms = getJointTransforms();
		glUniformMatrix4fv(glGetUniformLocation(shader->getProgramID(), "jointTransforms"), 16, GL_FALSE, glm::value_ptr(*jointTransforms));
	}
	// for (unsigned int i = 0; i < 16; i++) {
	// std::cout << "Animated Transform: " << i <<  std::endl;
	// std::cout << jointTransforms[i][0][0] <<  "\t\t" << jointTransforms[i][0][1]  <<  "\t\t" << jointTransforms[i][0][2]  <<  "\t\t" << jointTransforms[i][0][3] << std::endl;
	// std::cout << jointTransforms[i][1][0] <<  "\t\t" << jointTransforms[i][1][1]  <<  "\t\t" << jointTransforms[i][1][2]  <<  "\t\t" << jointTransforms[i][1][3] << std::endl;
	// std::cout << jointTransforms[i][2][0] <<  "\t\t" << jointTransforms[i][2][1]  <<  "\t\t" << jointTransforms[i][2][2]  <<  "\t\t" << jointTransforms[i][2][3] << std::endl;
	// std::cout << jointTransforms[i][3][0] <<  "\t\t" << jointTransforms[i][3][1]  <<  "\t\t" << jointTransforms[i][3][2]  <<  "\t\t" << jointTransforms[i][3][3] << std::endl;	

	// }


	glBindVertexArray(this->vao);

	if (instanceCount == 0 || instanceCount == 1)
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	else {
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instanceCount);
	}

	glBindVertexArray(0);
	
	return;
}

glm::mat4* Mesh::getJointTransforms() const {
	glm::mat4 *jointMatrices = new glm::mat4[this->jointNbr];
	this->addJointsToArray(this->rootJoint, jointMatrices);

	return jointMatrices;
}

void	Mesh::addJointsToArray(Joint *head, glm::mat4* jointMatrices) const {
	jointMatrices[head->index] = head->getFinalTransform();
	for (unsigned int i = 0; i < head->children.size(); i++) {
		addJointsToArray(head->children[i], jointMatrices);
	}
}

Joint *Mesh::getRootJoint() {
	return rootJoint;
}