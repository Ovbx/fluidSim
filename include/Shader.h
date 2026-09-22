#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <filesystem>

class Shader {

public:
    Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
    ~Shader();

    void useProgram();
    void setMat4(const std::string& name, const glm::mat4& value);
    GLuint getId() const;

private:
  GLuint m_ID;
};


