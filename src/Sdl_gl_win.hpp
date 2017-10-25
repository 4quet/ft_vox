/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sdl_gl_win.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/19 13:18:30 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/25 15:11:09 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SDL_GL_WIN_HPP
# define SDL_GL_WIN_HPP

# include <OpenGL/gl3.h>
# include <SDL.h>
# include <iostream>

# include "constants.hpp"

class Sdl_gl_win {
	public:
		Sdl_gl_win();
		~Sdl_gl_win();

		Action::Enum	eventManager();
		int				getMouseX() const;
		int				getMouseY() const;
		SDL_Window		*getWin() const;

	private:
		void	initSDL();
		void	initGL() const;

		SDL_Event		events;
		SDL_GLContext	ctx;
		SDL_Window		*win;
		int				mouseX;
		int				mouseY;
};

#endif