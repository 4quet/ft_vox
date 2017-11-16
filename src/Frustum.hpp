/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Frustum.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 11:08:28 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/16 09:28:59 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRUSTUM_HPP
# define FRUSTUM_HPP

# include <glm/gtc/matrix_transform.hpp>
# include <vector>
# include <map>
# include "Chunk.hpp"

class Frustum {
	public:
		Frustum(glm::mat4 projection);
		// void	setView(glm::mat4 view);
		void	compute(glm::mat4 view, std::map<index3D, Chunk*> & chunks);
		void	compute(glm::mat4 view, std::vector<Chunk*> & chunks);
		bool	pointIn(float x, float y, float z);
		void	setPlanes();
		void	setView(glm::mat4 view);

	private:
		Frustum();

		glm::mat4	projection;
		glm::mat4	view;
		float		planes[6][4];
};

#endif
