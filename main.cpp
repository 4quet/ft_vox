/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 09:23:13 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/21 13:20:57 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"

static bool     findAction(std::vector<Action::Enum> & actions, Action::Enum a) {
    for(unsigned int i = 0; i < actions.size(); i++) {
        if (actions[i] == a)
            return true;
    }
    return false;
}

static void     UIManager(std::vector<Action::Enum> actions, int frameDuration, ChunkManager & chunkManager, glm::vec3 & camPos) {
    GLString info;
        
    if(findAction(actions, Action::DEBUG)) {
        std::string totalBlock = std::to_string(chunkManager.getTotalActiveBlocks());
        std::string totalChunk = std::to_string(chunkManager.getTotalActiveChunks());
        
        info.renderText("x: " + std::to_string(camPos.x), 50, 300, glm::vec3(1.f, 1.f, 1.f));
        info.renderText("y: " + std::to_string(camPos.y), 50, 250, glm::vec3(1.f, 1.f, 1.f));
        info.renderText("z: " + std::to_string(camPos.z), 50, 200, glm::vec3(1.f, 1.f, 1.f));
        info.renderText("frameDuration: " + std::to_string(frameDuration), 50, 150, glm::vec3(1.f, 1.f, 1.f));
        info.renderText("Total chunks: " + totalChunk, 50, 100, glm::vec3(1.f, 1.f, 1.f));
        info.renderText("Total blocks: " + totalBlock, 50, 50, glm::vec3(1.f, 1.f, 1.f));
    }
    
    info.renderText("+", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, glm::vec3(1.f, 1.f, 1.f));
}

int     main() {
    clock_t start, end;
    Sdl_gl_win window;

    std::vector<Action::Enum> actions;

    Camera          camera;
	glm::vec3		camPos = camera.getPosition();
	ChunkManager	m(camPos);
    
    Shader	shader("src/shaders/vertex_shader.glvs", "src/shaders/fragment_shader.glfs");
    //Shader	shader("src/shaders/vertex_fog_shader.glvs", "src/shaders/fragment_fog_shader.glfs");
    Skybox  skybox("skybox");
    
    start = 0;
    while(actions.size() == 0 || actions[0] != Action::ESCAPE) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window.eventManager(actions);
        
        camera.move(actions, window.getMouseX(), window.getMouseY());
        // std::cout << window.getMouseX() << " : " << window.getMouseY() << std::endl;
        // camera.normalizeMouse();
		camPos = camera.getPosition();
        
		shader.use();
        shader.setCamera(camera.getMatrix());
		shader.setView();
		shader.setVec3("lightPos", camPos.x, camPos.y, camPos.z);
        
        m.update(camera);
        if (findAction(actions, Action::ERASE))
            camera.getPointedChunk(m.getRenderMap());
		m.render(shader);
        skybox.draw();
        
        end = std::clock();
        UIManager(actions, difftime( end, start ), m, camPos);
        SDL_GL_SwapWindow(window.getWin());
        start = std::clock();
    }
    return 0;
}
