#include "Shader.h"
#include <iostream>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    int success;
    int linkSuccess;
    char infoLog[512];

    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error::Shader::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error::Shader::FRAGMENT::COMPILATION_FAILED" << infoLog << std::endl;
    }
    m_ID = glCreateProgram();

    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, fragmentShader);
    glLinkProgram(m_ID);
    glGetProgramiv(m_ID, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        char infoLog[512];
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        std::cout << "Error::Shader::PROGRAM::LINKING_FAILED" << infoLog << std::endl; 
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

};
void Shader::useProgram() {
    glUseProgram(m_ID);
}
//sends mat4 floating point numbers from cpu program to a uniform variable in gpu shader progarm
void Shader::setMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
GLuint Shader::getId() const {
    return m_ID;
}
Shader::~Shader() {
    glDeleteProgram(m_ID);
}