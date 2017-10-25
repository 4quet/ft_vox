/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 09:23:13 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/25 16:51:34 by tpierron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"

int     main() {
    
    Sdl_gl_win window;

    Action::Enum action = Action::NONE;

	ChunkManager	m;
    Camera          camera;

    Shader	shader("src/shaders/basic_vertex_shader.glvs", "src/shaders/basic_fragment_shader.glfs");
    Skybox  skybox("skybox");
    GLString info;
    
    while(action != Action::ESCAPE) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        action = window.eventManager();

        camera.move(action, window.getMouseX(), window.getMouseY());
        shader.setCamera(camera.getMatrix());

		m.render(shader);

        info.renderText("OLA", 50, 50, glm::vec3(0.2f, 0.7f, 1.f));
        skybox.draw();
        SDL_GL_SwapWindow(window.getWin());
    }
    return 0;
}
