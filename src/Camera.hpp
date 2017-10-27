/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/25 13:39:37 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/27 12:05:42 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CAMERA_CLASS_HPP
# define CAMERA_CLASS_HPP

# include <gtc/matrix_transform.hpp>
# include <gtx/rotate_vector.hpp>
# include <gtx/matrix_interpolation.hpp>
# include <iostream>
# include "constants.hpp"

class Camera {
	public:
		Camera();
		~Camera();

		glm::mat4	getMatrix() const;
		glm::vec3   getPosition() const;
		void		move(Action::Enum, int, int);

	private:
		void				updateMatrix();
		void				manageMouse(glm::vec2);

		glm::mat4			matrix;
		glm::vec3			eyeVec;
		float				yaw;
		float				roll;
		glm::vec2			mousePosition;
};

#endif