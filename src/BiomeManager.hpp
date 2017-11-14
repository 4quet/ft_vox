/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BiomeManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/13 08:46:36 by thibautpier       #+#    #+#             */
/*   Updated: 2017/11/14 19:32:13 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIOMEMANAGER_HPP
# define BIOMEMANAGER_HPP

# include "constants.hpp"
# include "FastNoise.h"
# include "Chunk.hpp"

class BiomeManager
{
    public:
        BiomeManager();
        ~BiomeManager();
        float	getHeightAt(float, float, Biomes::Enum) const;
		void	setupLandscape(Chunk &);

    private:
        static 	FastNoise	sNoise;

		float	altitudeFreqSelector;
		float	biomeFreqSelector;

		float	fieldFreq;
		float	desertFreq;
		float	caveFreq;

		float	fieldMaxAltitude;
		float	desertMaxAltitude;

        
        Biomes::Enum    getBiomeAt(float, float) const;
        
        float           getFieldHeightAt(float, float) const;
        float           getDesertHeightAt(float, float) const;
        float           getSnowHeightAt(float, float) const;
        float           getCanyonHeightAt(float, float) const;
};

#endif
