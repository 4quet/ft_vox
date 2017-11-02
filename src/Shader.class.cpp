/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/16 14:11:27 by thibautpier       #+#    #+#             */
/*   Updated: 2017/11/02 11:58:50 by lfourque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shader.class.hpp"

Shader::Shader() {
    return;
}

Shader::Shader(std::string vertexSrc, std::string fragmentSrc)
                 : vertexSrc(vertexSrc) {
    this->fragmentSrc = fragmentSrc;
    this->vertexID = this->load(GL_VERTEX_SHADER, this->vertexSrc);
    this->fragmentID = this->load(GL_FRAGMENT_SHADER, this->fragmentSrc);
    this->geometryID = 0;
    this->compile();
    return;
}

Shader::Shader(std::string vertexSrc, std::string geometrySrc, std::string fragmentSrc)
                : vertexSrc(vertexSrc), fragmentSrc(fragmentSrc), geometrySrc(geometrySrc) {
    this->vertexID = this->load(GL_VERTEX_SHADER, this->vertexSrc);
    this->geometryID = this->load(GL_GEOMETRY_SHADER, this->geometrySrc);
    this->fragmentID = this->load(GL_FRAGMENT_SHADER, this->fragmentSrc);
    this->compile();
    return;

}

Shader::~Shader() {
    glDeleteShader(this->vertexID);
    glDeleteShader(this->fragmentID);
    if (this->geometryID != 0)
        glDeleteShader(this->geometryID);
    glDeleteProgram(this->programID);
    return;
}

GLuint    Shader::load(GLenum type, std::string const src) {
    GLuint shaderID;
    std::ifstream stream(src);
    std::string buff;
    std::string shader;

    if(!stream) {
        std::cout << "Shader File: " << src << " not found."<< std::endl;
        return -1;
    }

    while(getline(stream, buff))
        shader += buff + '\n';
    stream.close();
    const GLchar *shaderSrc = shader.c_str();

    shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &shaderSrc, 0);
    glCompileShader(shaderID);

    if(!this->checkShaderCompilation(shaderID, type))
        exit(0);
    
    return shaderID;
}

void    Shader::compile() {
    this->programID = glCreateProgram();
    glAttachShader(this->programID, this->vertexID);
    glAttachShader(this->programID, this->fragmentID);
    if (this->geometryID != 0)
        glAttachShader(this->programID, this->geometryID);

    glLinkProgram(this->programID);

    if(!this->checkProgramCompilation(this->programID))
        exit(0);

    return;
}

bool        Shader::checkShaderCompilation(GLuint shaderID, GLenum type) {
    GLint status;
    GLint errorSize;
    char *error;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE)
        return true;

    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &errorSize);    
    error = new char[errorSize + 1];
    glGetShaderInfoLog(shaderID, errorSize, NULL, error);
    std::cout << "shader compilation error:" << std::endl;
    if (type == GL_VERTEX_SHADER)
        std::cout << "Compile failure in vertex shader" << std::endl;
    if (type == GL_FRAGMENT_SHADER)
        std::cout << "Compile failure in fragment shader" << std::endl;
    if (type == GL_GEOMETRY_SHADER)
        std::cout << "Compile failure in geometry shader" << std::endl;
    std::cout << error << std::endl;
    
    return false;
}

bool        Shader::checkProgramCompilation(GLuint program) {
    GLint status;
    GLint errorSize;
    char *error;

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_TRUE)
        return true;

    std::cout << "linker failure:" << std::endl;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &errorSize);
    std::cout << errorSize << std::endl;
    error = new char[errorSize + 1];
    glGetProgramInfoLog(program, errorSize, NULL, error);
    std::cout << error << std::endl;
    
    return false;
}

void    Shader::use() const {
    glUseProgram(this->programID);
}

void    Shader::setCamera(glm::mat4 cameraMat) {
    Shader::camera = cameraMat;
}

void    Shader::setView() {
    glUniformMatrix4fv(glGetUniformLocation(this->programID, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(Shader::perspective));
    glUniformMatrix4fv(glGetUniformLocation(this->programID, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(Shader::camera));
}

void    Shader::setView(glm::mat4 view) {
    glUniformMatrix4fv(glGetUniformLocation(this->programID, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(Shader::perspective));
    glUniformMatrix4fv(glGetUniformLocation(this->programID, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
}

glm::mat4       Shader::getViewMatrix() {
    return Shader::camera;
}

void    Shader::setOrthoView(float resX, float resY) {
    glm::mat4 mat = glm::ortho(0.0f, resX, 0.0f, resY);
    glUniformMatrix4fv(glGetUniformLocation(this->programID, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(mat));
}

void    Shader::setModel(glm::mat4 model) const {
    glUniformMatrix4fv(glGetUniformLocation(this->programID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
}

void    Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}

void    Shader::setInt(const std::string &name, float value) const {
    glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}

void    Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(this->programID, name.c_str()), x, y, z);
}

GLuint  Shader::getProgramID() const {
    return this->programID;
}

glm::mat4 Shader::camera = glm::lookAt(
    glm::vec3(10.f, 5.0f, 15.f),
    glm::vec3(0.f, 0.f, 0.f),
    glm::vec3(0.f, 1.f, 0.f)
    );

glm::mat4 Shader::perspective = glm::perspective(45.f, 1.0f, 0.1f, 4000.f);
