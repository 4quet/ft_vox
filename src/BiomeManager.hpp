/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BiomeManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/13 08:46:36 by thibautpier       #+#    #+#             */
/*   Updated: 2017/11/13 09:06:17 by thibautpier      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIOMEMANAGER_HPP
# define BIOMEMANAGER_HPP

# include "constants.hpp"
# include "FastNoise.h"


class BiomeManager
{
    public:
        BiomeManager();
        ~BiomeManager();
        float getHeightAt(float, float) const;

    private:
        static 	FastNoise	sNoise;
        
        Biomes::Enum    getBiomeAt(float, float) const;
        
        float           getFieldHeightAt(float, float) const;
        float           getDesertHeightAt(float, float) const;
        float           getSnowHeightAt(float, float) const;
        float           getCanyonHeightAt(float, float) const;
};

#endif