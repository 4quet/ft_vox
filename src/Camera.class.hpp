/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/18 15:01:19 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/24 15:32:21 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_CLASS_HPP
# define CAMERA_CLASS_HPP

// # include "constants.hpp"
# include <gtc/matrix_transform.hpp>
# include <gtx/rotate_vector.hpp>
# include <gtx/matrix_interpolation.hpp>
# include <iostream>

class Camera {
	public:
		Camera();
		~Camera();

		glm::mat4	getMatrix() const;
		void		setCameraCrd(float, float, float);
		void		setCameraX(float);
		void		setCameraY(float);
		void		setCameraZ(float);

	private:
		glm::mat4			matrix;
		float				x;
		float				y;
		float				z;
};

#endif