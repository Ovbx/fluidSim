#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {

public: 
    Shader(const char* vertexSource, const char* fragmentSource);
    ~Shader();

    void useProgram();
    void setMat4(const std::string& name, const glm::mat4& value);
    GLuint getId() const;

private:
    GLuint m_ID;

};


