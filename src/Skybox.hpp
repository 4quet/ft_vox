/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Skybox.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 10:35:17 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/23 11:38:24 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SKYBOX_HPP
# define SKYBOX_HPP

# include <iostream>
# include <OpenGL/gl3.h>
# include <vector>

class Skybox {
	public:
		Skybox(std::string directory);
		~Skybox();
		
		// void	draw() const;
		unsigned int getTextureID() const;
	private:
		Skybox();
		void	getFacesPath(std::string directory);
		void	loadTextures();

		unsigned int textureID;
		std::vector<const char*> textures_faces;

};

#endif