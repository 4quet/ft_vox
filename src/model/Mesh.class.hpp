/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.class.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/21 11:54:43 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/19 16:24:54 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESH_CLASS_HPP
# define MESH_CLASS_HPP

# include <vector>

# include <vec3.hpp>
# include <vec2.hpp>
# include <assimp/Importer.hpp>
# include <OpenGL/gl3.h>

# include "../Shader.class.hpp" 
# include "Joint.class.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

	int		bonesID[3];
	float	weigths[3];

	Vertex() {for (unsigned int i = 0; i < 3;i++){bonesID[i] = 0; weigths[i] = 0;}};
};

struct VertexBoneData {
    unsigned int IDs[3];
    float        weigths[3];
	VertexBoneData() {for (unsigned int i = 0; i < 3;i++){IDs[i] = 0; weigths[i] = 0;}};
};

struct Texture {
    unsigned int id;
    std::string type;
    aiString path;
};

class Mesh {
    public:
        Mesh(std::vector<Vertex>, std::vector<unsigned int>,
                std::vector<Texture>, aiColor3D color, 
                Joint *rootJoint, unsigned int jointNbr);
        Mesh(Mesh const & src);
        ~Mesh();
    

		void	draw(Shader *shader, bool animated, unsigned int instanceCount);
        void    setInstanceBuffer(std::vector<glm::mat4> const &);
		glm::mat4* getJointTransforms() const;
        Joint *getRootJoint();
        void printJointMatrices(Joint *joint);
    private:
        Mesh();
		void	setupMesh();
		void	addJointsToArray(Joint *head, glm::mat4* jointMatrices) const;

        std::vector<Vertex>     vertices;
        std::vector<unsigned int>     indices;
        std::vector<Texture>    textures;
        aiColor3D               color;
        Joint                   *rootJoint;
        unsigned int            jointNbr;
        unsigned int            vao;
        unsigned int            vbo;
        unsigned int            ebo;
        unsigned int            ibo;

        static int i;
};

#endif