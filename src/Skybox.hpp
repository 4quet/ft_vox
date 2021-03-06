/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Skybox.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 10:35:17 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/22 11:25:02 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SKYBOX_HPP
# define SKYBOX_HPP

# include <iostream>
# include <OpenGL/gl3.h>
# include <vector>

# include "Shader.class.hpp"

class Skybox {
	public:
		Skybox(std::string directory);
		~Skybox();
		
		void			draw();
		unsigned int	getTextureID() const;

	private:
		Skybox();
		
		unsigned int				textureID;
		std::vector<std::string>	textures_faces;

		GLuint						vao;
		GLuint						vbo;
		Shader						*shader;

		void	getFacesPath(std::string directory);
		void	loadTextures();
		void	setBuffers();
};

#endif