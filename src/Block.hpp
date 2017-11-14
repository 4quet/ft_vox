/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 10:40:00 by lfourque          #+#    #+#             */
/*   Updated: 2017/11/13 19:16:41 by thibautpier      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_HPP
# define BLOCK_HPP

# include "constants.hpp"

class Block
{
	public:
		Block();
		~Block();

		bool		isActive() const;
		BlockType	getBlockType() const;

		// void		setActive(bool active);
		void		setBlockType(BlockType blockType);

	private:
		// bool		_active;
		BlockType	_blockType;
};

#endif /* BLOCK_HPP */
