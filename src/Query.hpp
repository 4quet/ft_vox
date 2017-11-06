/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Query.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 14:48:25 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/05 16:25:44 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUERY_HPP
# define QUERY_HPP

# include <OpenGL/gl3.h>
# include <OpenGL/glext.h>

class Query
{
	private:
		GLuint	_id;
		GLuint	_type;
		bool	_inUse;

		Query();

	public:
		Query(GLuint);
		~Query();

		void	start();
		void	end();

		bool	isInUse() const;
		GLuint	isResultReady();
		GLuint	getResult();
};

#endif /* QUERY_HPP */
