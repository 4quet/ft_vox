/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Frustum.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 11:08:28 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/30 14:10:58 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRUSTUM_HPP
# define FRUSTUM_HPP

# include <gtc/matrix_transform.hpp>
# include <vector>
# include <map>
# include "Chunk.hpp"

class Frustum {
	public:
		Frustum(glm::mat4 projection);
		// void	setView(glm::mat4 view);
		void	compute(glm::mat4 view, std::map<std::tuple<float, float, float>, Chunk*> & chunks);

	private:
		typedef	std::tuple<float, float, float>	index3D;
		Frustum();

		void		setPlanes();
		bool		pointIn(float x, float y, float z);

		glm::mat4	projection;
		glm::mat4	view;
		float		planes[6][4];
};

#endif