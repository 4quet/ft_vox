/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 10:47:04 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/16 10:36:45 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Block.hpp"

Block::Block() : _blockType(BLOCKTYPE_INACTIVE) { }

Block::~Block() { }

bool		Block::isActive() const {
    if (this->_blockType == BLOCKTYPE_INACTIVE || this->_blockType == BLOCKTYPE_WATER)
        return false;
    return true;
}
BlockType	Block::getBlockType() const {return this->_blockType; }

// void		Block::setActive(bool active) { this->_active = active; }
void		Block::setBlockType(BlockType blockType) { this->_blockType = blockType; }
