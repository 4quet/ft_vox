/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 09:23:13 by tpierron          #+#    #+#             */
/*   Updated: 2017/11/22 10:02:38 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vox.hpp"

static void     UIManager(GLString & info, int fps, ChunkManager & chunkManager, glm::vec3 & camPos) {
        
	std::string totalBlock = std::to_string(chunkManager.getTotalActiveBlocks());
	std::string totalChunk = std::to_string(chunkManager.getTotalActiveChunks());
	
	info.renderText("x: " + std::to_string(camPos.x), 50, 300, glm::vec3(1.f, 1.f, 1.f));
	info.renderText("y: " + std::to_string(camPos.y), 50, 250, glm::vec3(1.f, 1.f, 1.f));
	info.renderText("z: " + std::to_string(camPos.z), 50, 200, glm::vec3(1.f, 1.f, 1.f));
	info.renderText("FPS: " + std::to_string(fps), 50, 150, glm::vec3(1.f, 1.f, 1.f));
	info.renderText("rendered chunks: " + totalChunk, 50, 100, glm::vec3(1.f, 1.f, 1.f));
	info.renderText("rendered blocks: " + totalBlock, 50, 50, glm::vec3(1.f, 1.f, 1.f));
}

int     main() {
    int	start, delta, fps;
    Sdl_gl_win window;
    GLString info;

    std::vector<Action::Enum> actions;

    Camera          camera;
	glm::vec3		camPos = camera.getPosition();
	ChunkManager	m(camPos);
    
    Shader	shader("src/shaders/vertex_shader.glvs", "src/shaders/fragment_shader.glfs");
    //Shader	shader("src/shaders/vertex_fog_shader.glvs", "src/shaders/fragment_fog_shader.glfs");
    Skybox  skybox("skybox");
    
    start = 0;
	delta = 0;
	fps = 0;
    static unsigned int trigger = 0;
    while(actions.size() == 0 || actions[0] != Action::ESCAPE) {
        start = SDL_GetTicks();

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
		if (find(actions.begin(), actions.end(), Action::ERASE) != actions.end())
		{
            trigger++;
            if (trigger > 100) {
                camera.deleteBlock(m.getRenderMap());
                trigger = 0;
            }
        }
		m.render(shader);
        skybox.draw();
        
        delta += start;
		
		if (delta > 100000)
		{
			fps = 1000 / (SDL_GetTicks() - start);
			delta = 0;
		}
		if (find(actions.begin(), actions.end(), Action::DEBUG) != actions.end())
		{
        	UIManager(info, fps, m, camPos);
		}
    	info.renderText("+", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, glm::vec3(1.f, 1.f, 1.f));
        SDL_GL_SwapWindow(window.getWin());
    }
    return 0;
}
