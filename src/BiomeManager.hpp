/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BiomeManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/13 08:46:36 by thibautpier       #+#    #+#             */
/*   Updated: 2017/11/16 18:50:29 by lfourque         ###   ########.fr       */
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

		void	setupLandscape(Chunk &);

    private:
       	FastNoise	heightMapNoise;
		float		surfaceFreq;

        float		getHeightAt(float, float);

       	FastNoise	elevationNoise;
		float		elevationFreq;
		float		elevationMultiplier;
		float		getElevation(float, float);

       	FastNoise	moistureNoise;
		float		moistureFreq;
		float		getMoisture(float, float);

       	FastNoise	offsetNoise;
		float		offsetFreq;
		float		getOffset(float, float);

       	FastNoise	caveNoise;
		float		caveFreq;
};

#endif
