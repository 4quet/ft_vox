/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 09:23:13 by tpierron          #+#    #+#             */
/*   Updated: 2017/10/27 12:11:17 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"

static void     UIManager(Action::Enum action, int frameDuration, ChunkManager & chunkManager) {
    static bool debugSwitch = false;
    GLString info;
    
    if (action == Action::DEBUG)
        debugSwitch = !debugSwitch;
        
    if(debugSwitch) {
        std::string totalBlock = std::to_string(chunkManager.getTotalActiveBlocks());
        std::string totalChunk = std::to_string(chunkManager.getTotalActiveChunks());

        info.renderText("frameDuration: " + std::to_string(frameDuration), 50, 150, glm::vec3(0.2f, 0.7f, 1.f));
        info.renderText("Total chunks: " + totalChunk, 50, 100, glm::vec3(0.2f, 0.7f, 1.f));
        info.renderText("Total blocks: " + totalBlock, 50, 50, glm::vec3(0.2f, 0.7f, 1.f));
    }
}

int     main() {
    clock_t start, end;
    Sdl_gl_win window;

    Action::Enum action = Action::NONE;

	ChunkManager	m;
    Camera          camera;

    Shader	shader("src/shaders/basic_vertex_shader.glvs", "src/shaders/basic_fragment_shader.glfs");
    Skybox  skybox("skybox");
    
    start = 0;
    while(action != Action::ESCAPE) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        action = window.eventManager();
        
        camera.move(action, window.getMouseX(), window.getMouseY());
        shader.setCamera(camera.getMatrix());
        
		m.update(shader, camera);
        skybox.draw();
        
        end = std::clock();
        UIManager(action, difftime( end, start ), m);
        SDL_GL_SwapWindow(window.getWin());
        start = std::clock();
    }
    return 0;
}
