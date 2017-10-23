/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.class.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 10:02:40 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/19 16:38:33 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Model.class.hpp"
# define STB_IMAGE_IMPLEMENTATION
# include "../stb_image.h"

int	Model::i = 0;

Model::Model(std::string path, bool animated) {
	this->animated = animated;
	loadModel(path);
	Model::i++;
	// std::cout << "MODEL CONSTRUCT: " << Model::i << std::endl;
	return;
}

Model::~Model() {
	for (unsigned int i = 0; i < this->meshes.size(); i++) {
		delete this->meshes[i];
	}
	Model::i--;
	// std::cout << "MODEL DESTRUCT: " << Model::i << std::endl;
	return;
}

std::vector<glm::mat4>  Model::getKeyFrame(aiAnimation *animation) {
	std::vector<glm::mat4> boneMatrices;
	boneMatrices.resize(this->bonesMap.size());

	for (unsigned int i = 0; i < animation->mNumChannels; i++) {
	
		aiVector3D ScalingKey = animation->mChannels[i]->mScalingKeys[0].mValue;
		aiQuaternion RotationKey = animation->mChannels[i]->mRotationKeys[0].mValue;
		aiVector3D PositionKey = animation->mChannels[i]->mPositionKeys[0].mValue;

		glm::vec3 s(ScalingKey.x, ScalingKey.y, ScalingKey.z);
		glm::quat q(RotationKey.w, RotationKey.x, RotationKey.y, RotationKey.z);
		glm::vec3 t(PositionKey.x, PositionKey.y, PositionKey.z);

		glm::mat4 S = glm::scale(glm::mat4(1), s);
		glm::mat4 R = glm::mat4_cast(q);
		glm::mat4 T = glm::translate(glm::mat4(1), t);

		glm::mat4 M = T * R * S;

		boneMatrices[bonesMap[animation->mChannels[i]->mNodeName.data]] = M;	
	}
	return boneMatrices;
}

void	setTransforms(std::vector<glm::mat4> matrices, Joint *bone) {
	// glm::mat4 mat = bone->getAnimatedTransform();
	bone->setAnimatedTransform(matrices[bone->index]);
	for (unsigned int i = 0; i < bone->children.size(); i++) {
		setTransforms(matrices, bone->children[i]);
	}
}

void	Model::readBonesHierarchy2(std::vector<glm::mat4> trs, aiNode* node, glm::mat4 parentTransform) {
	std::string nodeName(node->mName.data);
	// const aiNodeAnim* nodeAnim = FindNodeAnim(animation, nodeName);

	// glm::mat4 nodeTransform = trs[BonesMap[nodeName]];
	glm::mat4 globalTransformation = parentTransform * trs[bonesMap[nodeName]];
	finalTransform[bonesMap[nodeName]] = globalInverse * globalTransformation *
											bonesMatrix[bonesMap[nodeName]];
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		readBonesHierarchy2(trs, node->mChildren[i], globalTransformation);
	}
}

void	Model::processNode(aiNode *node, const aiScene *scene) {
	// std::cout << "PROCESS NODE: "<< node->mNumChildren << " : " << this->directory << std::endl;
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		std::vector<Vertex>	vertices = loadVertices(mesh);
		std::vector<unsigned int> indices = loadIndices(mesh);
		std::vector<Texture> materials = loadMaterials(mesh, scene);
		aiColor3D color(0.f,0.f,0.f);
		if (materials.size() == 0)
			scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE,color);
		Joint *rootJoint;
			
		if (this->animated == true) {
			rootJoint = loadJoints(mesh);
			this->meshes.push_back(new Mesh(vertices, indices, materials, color, rootJoint, mesh->mNumBones));
			
			this->readBonesHierarchy(scene->mRootNode, rootJoint); //localBindTrans == offset matrix
			aiNode *rootBone = scene->mRootNode->FindNode(mesh->mBones[0]->mName.data);
			this->readBonesHierarchy2(getKeyFrame(scene->mAnimations[0]), rootBone, glm::mat4(1.0));
			setTransforms(finalTransform, meshes[0]->getRootJoint()); //animatedTra = TRS matrix
		}
		else {
			rootJoint = NULL;
			this->meshes.push_back(new Mesh(vertices, indices, materials, color, rootJoint, mesh->mNumBones));
		}
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

void	Model::loadModel(std::string path) {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Model loading error: " << importer.GetErrorString() << std::endl;
		return;
	}
	this->globalInverse = asssimpToGlmMatrix(scene->mRootNode->mTransformation);
	this->globalInverse = glm::inverse(this->globalInverse);
	this->directory = path.substr(0, path.find_last_of('/'));

	aiNode *node = scene->mRootNode;
	// for(unsigned int i = 0; i < node->mNumChildren; i++) {
	// 	if (node->mChildren[i]->mNumMeshes != 0) {
	// 		node = node->mChildren[i];
	// 		break;
	// 	}
	// }
	
	processNode(node, scene);
	// aiMesh *mesh = scene->mMeshes[node->mMeshes[0]];
	// std::vector<Vertex>	vertices = loadVertices(mesh);
	// std::vector<unsigned int> indices = loadIndices(mesh);
	// std::vector<Texture> materials = loadMaterials(mesh, scene);
	// Joint *rootJoint;
	// 			// std::cout << path << std::endl;
	// if (this->animated == true) {
	// 	rootJoint = loadJoints(mesh);
	// 	this->meshes.push_back(Mesh(vertices, indices, materials, rootJoint, mesh->mNumBones));
		
	// 	this->readBonesHierarchy(scene->mRootNode, rootJoint); //localBindTrans == offset matrix
	// 	aiNode *rootBone = scene->mRootNode->FindNode(mesh->mBones[0]->mName.data);
	// 	this->readBonesHierarchy2(getKeyFrame(scene->mAnimations[0]), rootBone, glm::mat4(1.0));
	// 	setTransforms(finalTransform, meshes[0].getRootJoint()); //animatedTra = TRS matrix
	// }
	// else {
	// 	rootJoint = NULL;
	// 	this->meshes.push_back(Mesh(vertices, indices, materials, rootJoint, mesh->mNumBones));
	// }



	// applyPoseToJoints(rootJoint, glm::mat4(1.0)); //must set finalTransform

	// printMat());
}

void	Model::applyPoseToJoints(Joint *joint, glm::mat4 parentTransform) {
	// glm::mat4 currentLocalTransform = bonesMatrix[joint->index];
	glm::mat4 nodeTransform = joint->getAnimatedTransform();
	glm::mat4 globalTransform = parentTransform * nodeTransform;
	glm::mat4 finalTransform = this->globalInverse * globalTransform * joint->getInverseBindTransform();
	
	joint->setFinalTransform(finalTransform);

	for (unsigned int i = 0; i < joint->children.size(); i++) {
		applyPoseToJoints(joint->children[i], globalTransform);
	}

}

void	Model::readBonesHierarchy(aiNode *node, Joint *joint) {

	aiNode *nodeFound = node->FindNode(joint->name.c_str());
	if (nodeFound) {
		for (unsigned int i = 0; i < nodeFound->mNumChildren; i++) {
			unsigned int index = bonesMap[nodeFound->mChildren[i]->mName.data];
			Joint *newChild = new Joint(index,
							nodeFound->mChildren[i]->mName.data,
							bonesMatrix[index]);
	// glm::mat4 mat = asssimpToGlmMatrix(nodeFound->mTransformation);
	// std::cout << "blba" << std::endl;
	// std::cout << mat[0][0] <<  "\t" << mat[0][1]  <<  "\t" << mat[0][2]  <<  "\t" << mat[0][3] << std::endl;
	// std::cout << mat[1][0] <<  "\t" << mat[1][1]  <<  "\t" << mat[1][2]  <<  "\t" << mat[1][3] << std::endl;
	// std::cout << mat[2][0] <<  "\t" << mat[2][1]  <<  "\t" << mat[2][2]  <<  "\t" << mat[2][3] << std::endl;
	// std::cout << mat[3][0] <<  "\t" << mat[3][1]  <<  "\t" << mat[3][2]  <<  "\t" << mat[3][3] << std::endl;
			// newChild.setAnimatedTransform(bonesMatrix[index]);
			// joint->setInverseBindTransform(node->mTransformation);
			joint->addChild(newChild);
			this->readBonesHierarchy(node, newChild);
		}
	}
}

std::vector<Vertex> Model::loadVertices(aiMesh *mesh) {
	std::vector<Vertex>			vertices;

	for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 posVector;
		glm::vec3 normalVector;
		glm::vec2 textVector;

		posVector.x = mesh->mVertices[i].x;
		posVector.y = mesh->mVertices[i].y;
		posVector.z = mesh->mVertices[i].z;

		if(mesh->HasNormals()) {
			normalVector.x = mesh->mNormals[i].x;
			normalVector.y = mesh->mNormals[i].y;
			normalVector.z = mesh->mNormals[i].z;
		} else {
			normalVector.x = 0;
			normalVector.y = 0;
			normalVector.z = 0;
		}

		if(mesh->mTextureCoords[0]) {
			textVector.x = mesh->mTextureCoords[0][i].x;
			textVector.y = mesh->mTextureCoords[0][i].y;
		} else {
			textVector.x = 0;
			textVector.y = 0;
		}

		vertex.position = posVector;
		vertex.normal = normalVector;
		vertex.texCoords = textVector;

		vertices.push_back(vertex);
	}

	std::vector<float> boneWeights;
	boneWeights.resize(mesh->mNumVertices * 3);
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		aiBone *bone = mesh->mBones[i];
		for (unsigned int j = 0; j < bone->mNumWeights; j++) {
			aiVertexWeight weight = bone->mWeights[j];
			unsigned int vertexStart = weight.mVertexId * 3;
			for (unsigned int k = 0; k < 3; k++) {
				if (boneWeights[vertexStart + k] == 0) {
					boneWeights[vertexStart + k] = weight.mWeight;
					vertices[weight.mVertexId].weigths[k] = weight.mWeight;
					vertices[weight.mVertexId].bonesID[k] = i;
					break;
				}
			}
		}
	}
	
	return vertices;
}

std::vector<unsigned int>	Model::loadIndices(aiMesh *mesh) {
	std::vector<unsigned int>	indices;

	unsigned int n1 = 0;
	unsigned int n2 = 0;
	for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
			n1 = j;
		}
		n2 = i;
	}

	return indices;
}

std::vector<Texture>	Model::loadMaterials(aiMesh *mesh, const aiScene *scene) {

	std::vector<Texture>		textures;

	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	return textures;
}

Joint	*Model::loadJoints(aiMesh *mesh) {
	// std::cout << "A"  << std::endl;
	aiBone *rootBone = mesh->mBones[0];
	
	// glm::mat4 mat = asssimpToGlmMatrix(rootBone->mOffsetMatrix);
	Joint *rootJoint = new Joint(0, rootBone->mName.C_Str(), glm::mat4(1.0));

	// std::cout << "B"  << std::endl;
	std::vector<int> boneIDs;
	std::vector<float> boneWeights;
	std::vector<glm::mat4> boneMatrices;
	
	boneIDs.resize(mesh->mNumVertices * 3);
	boneWeights.resize(mesh->mNumVertices * 3);
	boneMatrices.resize(mesh->mNumBones);
	finalTransform.resize(mesh->mNumBones);
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		aiBone *bone = mesh->mBones[i];
		this->bonesMap[bone->mName.data] = i;
		boneMatrices[i] = asssimpToGlmMatrix(bone->mOffsetMatrix);
		for (unsigned int j = 0; j < bone->mNumWeights; j++) {
			aiVertexWeight weight = bone->mWeights[j];
			unsigned int vertexStart = weight.mVertexId * 3;
			for (unsigned int k = 0; k < 3; k++) {
				if (boneWeights[vertexStart + k] == 0) {
					boneWeights[vertexStart + k] = weight.mWeight;
					boneIDs[vertexStart + k] = i;
					break;
				}
			}
		}
	}

	this->bonesMatrix = boneMatrices;
	return rootJoint;
}


std::vector<Texture> Model::loadTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for(unsigned int j = 0; j < this->texturesLoaded.size(); j++) {
			if (std::strcmp(texturesLoaded[j].path.C_Str(), str.C_Str()) == 0) {
				textures.push_back(texturesLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = this->textureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			texturesLoaded.push_back(texture);
		}
	}
	return textures;
}

void	Model::draw(Shader *shader, unsigned int instanceCount) {
	for(unsigned int i = 0; i < this->meshes.size(); i++) {
		meshes[i]->draw(shader, this->animated, instanceCount);
	}
}

unsigned int		Model::textureFromFile(const char* path, const std::string &directory) {
	std::string filename = std::string(path);
	filename = directory + "/" + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, componentNbr;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &componentNbr, 0);
	if(data) {
		GLenum format;
		switch(componentNbr) {
			case 1: format = GL_RED; break;
			case 3: format = GL_RGB; break;
			case 4: format = GL_RGBA; break;
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Loading texture failed: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}


glm::mat4			Model::asssimpToGlmMatrix(aiMatrix4x4 ai) const {
		glm::mat4 mat;
		mat[0][0] = ai.a1; mat[1][0] = ai.a2; mat[2][0] = ai.a3; mat[3][0] = ai.a4;
		mat[0][1] = ai.b1; mat[1][1] = ai.b2; mat[2][1] = ai.b3; mat[3][1] = ai.b4;
		mat[0][2] = ai.c1; mat[1][2] = ai.c2; mat[2][2] = ai.c3; mat[3][2] = ai.c4;
		mat[0][3] = ai.d1; mat[1][3] = ai.d2; mat[2][3] = ai.d3; mat[3][3] = ai.d4;
		return mat;
}

void				Model::printMat(glm::mat4 mat) {
		std::cout << mat[0][0] << mat[0][1] << mat[0][2] << mat[0][3] << std::endl;
		std::cout << mat[1][0] << mat[1][1] << mat[1][2] << mat[1][3] << std::endl;
		std::cout << mat[2][0] << mat[2][1] << mat[2][2] << mat[2][3] << std::endl;
		std::cout << mat[3][0] << mat[3][1] << mat[3][2] << mat[3][3] << std::endl;
}

void				Model::setInstanceBuffer(std::vector<glm::mat4> const & data) {
	for(unsigned int i = 0; i < this->meshes.size(); i++) {
		meshes[i]->setInstanceBuffer(data);
	}
}