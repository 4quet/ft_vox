/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 09:23:13 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/23 19:27:12 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"

int     main() {
    
    Sdl_gl_win window;

    Action::Enum action = Action::NONE;

	Chunk	chunk;

	Shader	shader("src/shaders/basic_vertex_shader.glvs", "src/shaders/basic_fragment_shader.glfs");
    
    while(action != Action::ESCAPE) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        action = window.eventManager();
        
		chunk.render(shader);

        SDL_GL_SwapWindow(window.getWin());
    }
    return 0;
}
