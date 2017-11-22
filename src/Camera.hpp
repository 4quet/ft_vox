/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/25 13:39:37 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/22 10:45:51 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CAMERA_CLASS_HPP
# define CAMERA_CLASS_HPP

# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtx/rotate_vector.hpp>
# include <glm/gtx/matrix_interpolation.hpp>
# include <iostream>
# include <map>
# include "constants.hpp"
# include "Chunk.hpp"

class Camera {
	public:
		Camera();
		~Camera();

		glm::mat4	getMatrix() const;
		glm::vec3   getPosition() const;
		void		move(std::vector<Action::Enum> &, int, int);
		void        deleteBlock(std::map<float, Chunk*> & chunks);
		
	private:
		void		updateMatrix();
		void		manageMouse(glm::vec2);
		glm::vec3	getRay() const;
		bool     	findBlockInchunk(glm::vec3 ray, glm::vec3 startPoint, Chunk & chunk);

		glm::mat4	matrix;
		glm::vec3	eyeVec;
		float		yaw;
		float		roll;
		glm::vec2	mousePosition;
};

#endif