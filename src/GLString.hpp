/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLString.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/25 16:36:24 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/22 11:20:57 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef GLSTRING_CLASS_H
# define GLSTRING_CLASS_H

# include <map>
# include "Shader.class.hpp"
# include <glm/gtc/matrix_transform.hpp>
# include <OpenGL/gl3.h>
# include <ft2build.h>
# include FT_FREETYPE_H

struct Character {
    GLuint     textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint     advance;
};

class GLString {
	public:
		GLString();
		~GLString();
		void		render();
		void		renderText(std::string str, float x, float y, glm::vec3 color) const;

	private:
		static std::map<GLchar, Character>	characters;
		Shader								*shader;
		GLuint								VAO;
		GLuint								VBO;
		
		void		initFont(std::string fontPath);
		void		setupGl();
};

#endif

