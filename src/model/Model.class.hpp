/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.class.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 09:40:41 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/16 14:24:59 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODEL_CLASS_HPP
# define MODEL_CLASS_HPP

# include <vector>
# include <map>

# include <assimp/Importer.hpp>
# include <assimp/scene.h>
# include <assimp/postprocess.h>

# include "Mesh.class.hpp"

class Model {
	public:
		Model(std::string path, bool animated);
		~Model();

		void	draw(Shader *shader, unsigned int instanceCount);
		void	setInstanceBuffer(std::vector<glm::mat4> const &);
		// void drawInstanced(Shader *shader);
		
	private:
		Model();
		void						loadModel(std::string path);

		std::vector<Vertex>			loadVertices(aiMesh *mesh);
		std::vector<unsigned int>	loadIndices(aiMesh *mesh);
		std::vector<Texture>		loadMaterials(aiMesh *mesh, const aiScene *scene);
		Joint 						*loadJoints(aiMesh *mesh);

		void				processNode(aiNode *node, const aiScene *scene);
		// Mesh				processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture>		loadTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
		unsigned int				textureFromFile(const char* path, const std::string &directory);
		void						readBonesHierarchy(aiNode*, Joint*);
		void						readBonesHierarchy2(std::vector<glm::mat4> trs, aiNode*, glm::mat4 parentTransform);
		std::vector<VertexBoneData>	loadBones(const aiMesh *mesh);
		void						applyPoseToJoints(Joint *joint, glm::mat4 parentTransform);
		glm::mat4					asssimpToGlmMatrix(aiMatrix4x4 ai) const;
		void						printMat(glm::mat4 mat);
		std::vector<glm::mat4>		getKeyFrame(aiAnimation *animation);

		bool								animated;
		std::vector<Mesh*>					meshes;
		std::string 						directory;
		std::vector<Texture>    			texturesLoaded;
		// std::vector<VertexBoneData> bones;
		std::map<std::string, unsigned int> bonesMap;
		std::vector<glm::mat4> 				bonesMatrix;
		std::vector<glm::mat4> 				finalTransform;
		glm::mat4							globalInverse;

		static int i;
};

#endif