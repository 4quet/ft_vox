/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfourque <lfourque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 10:47:04 by lfourque          #+#    #+#             */
/*   Updated: 2017/10/23 11:02:23 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Block.hpp"

Block::Block() : _active(true), _blockType(BLOCKTYPE_DEFAULT) { }

Block::~Block() { }

bool		Block::isActive() const { return this->_active; }
BlockType	Block::getBlockType() const { return this->_blockType; }

void		Block::setActive(bool active) { this->_active = active; }
void		Block::setBlockType(BlockType blockType) { this->_blockType = blockType; }
