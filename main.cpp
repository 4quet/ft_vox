/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 09:23:13 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/23 11:41:05 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"

int     main() {
    
    Sdl_gl_win window;

    Action::Enum action = Action::NONE;

    Skybox skybox("./skybox");
    
    while(action != Action::ESCAPE) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        action = window.eventManager();
        SDL_GL_SwapWindow(window.getWin());
    }
    return 0;
}