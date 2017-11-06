/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Query.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 14:53:09 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/05 16:25:33 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Query.hpp"

Query::Query(GLuint type) : _type(type), _inUse(false) {
	glGenQueries(1, &_id);
}

Query::~Query() {
	glDeleteQueries(1, &_id);
}

void	Query::start() {
	glBeginQuery(_type, _id);
	_inUse = true;
}

void	Query::end() {
	glEndQuery(_type);
}

bool	Query::isInUse() const { return _inUse; }

GLuint	Query::isResultReady() {
	GLuint	result;
	glGetQueryObjectuiv(_id, GL_QUERY_RESULT_AVAILABLE, &result);
	return result;
}

GLuint	Query::getResult() {
	_inUse = false;
	GLuint	result;
	glGetQueryObjectuiv(_id, GL_QUERY_RESULT, &result);
	return result;
}
