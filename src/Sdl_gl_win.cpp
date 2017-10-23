/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sdl_gl_win.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 09:34:29 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/23 10:05:35 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sdl_gl_win.hpp"

Sdl_gl_win::Sdl_gl_win() {
		initSDL();
        initGL();
}

Sdl_gl_win::~Sdl_gl_win() {

	SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();

	return;
}

void	Sdl_gl_win::initSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    win = SDL_CreateWindow("ft_vox",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        1024, 1024,
                                        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    ctx = SDL_GL_CreateContext(win);
    if(ctx == 0) {
        std::cout << SDL_GetError() << std::endl;
        exit(0);
    }
}

void	Sdl_gl_win::initGL() const {
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

Action::Enum    Sdl_gl_win::eventManager() {
    static int repeatFlag = 0;
    SDL_PollEvent(&events);
    if (events.window.event == SDL_WINDOWEVENT_CLOSE || events.key.keysym.sym == SDLK_ESCAPE) {
        return Action::ESCAPE;
    }
    if (events.type == SDL_KEYUP)
    repeatFlag = 0;
    if (events.type == SDL_KEYDOWN && repeatFlag == 0) {
        repeatFlag = 1;
        switch(events.key.keysym.sym) {
            // case SDLK_RIGHT: return Action::RIGHT; break;
            // case SDLK_LEFT: return Action::LEFT; break;
        }
	}
	return Action::NONE;
}

SDL_Window      *Sdl_gl_win::getWin() const {
    return win;
}