/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BiomeManager.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thibautpierron <thibautpierron@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/13 08:46:36 by thibautpier       #+#    #+#             */
/*   Updated: 2017/11/15 11:46:50 by lfourque         ###   ########.fr       */
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
		float		caveFreq;

        float		getHeightAt(float, float);

       	FastNoise	elevationNoise;
		float		elevationFrequency;
		float		maxElevationMult;
		float		getElevationMult(float, float) const;

       	FastNoise	sharpnessNoise;
		float		sharpnessFrequency;
		float		sharpnessDivider;
		float		getSharpness(float, float) const;
};

#endif
