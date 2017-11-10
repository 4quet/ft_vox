/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Query.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 14:53:09 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/10 15:57:54 by lfourque         ###   ########.fr       */
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
	glGetQueryObjectuiv(_id, GL_QUERY_RESULT_AVAILABLE, &_ready);
	return _ready;
}

GLuint	Query::getResult() {
	_inUse = false;
	glGetQueryObjectuiv(_id, GL_QUERY_RESULT, &_result);
	return _result;
}
