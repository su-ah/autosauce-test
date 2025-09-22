#include "shared/Shader.hpp"
#include "shared/Logger.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

// Helper method to convert SHADER_TYPE to OpenGL enum
GLenum Shader::getGLShaderType(SHADER_TYPE type) {
    switch (type) {
        case VERTEX: return GL_VERTEX_SHADER;
        case FRAGMENT: return GL_FRAGMENT_SHADER;
        case GEOMETRY: return GL_GEOMETRY_SHADER;
        case COMPUTE: return GL_COMPUTE_SHADER;
        default: 
            LOG_ERROR("Unknown shader type");
            return 0;
    }
}

bool Shader::compileShader(GLuint& shader, SHADER_TYPE shaderType, const std::string& source) {
    GLenum glType = getGLShaderType(shaderType);
    if (glType == 0) return false;
    
    shader = glCreateShader(glType);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    
    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        LOG_ERROR("Shader compilation failed: " + std::string(infoLog));
        glDeleteShader(shader);
        return false;
    }
    
    return true;
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "Error: " << type << " shader compilation failed: " << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "Error: Program linking failed: " << infoLog << std::endl;
        }
    }
}

bool Shader::addShader(SHADER_TYPE shaderType, const std::string& source) {
    // Check if this shader type already exists
    if (shaderMap.find(shaderType) != shaderMap.end()) {
        LOG_WARN("Shader type already exists. Use replaceShader() to replace it.");
        return false;
    }
    
    GLuint shader;
    if (compileShader(shader, shaderType, source)) {
        shaders.push_back(shader);
        shaderMap[shaderType] = shader;
                LOG_DEBUG("Shader type added successfully");
        return true;
    }
    return false;
}

bool Shader::replaceShader(SHADER_TYPE shaderType, const std::string& source) {
    // Remove existing shader of this type if it exists
    removeShader(shaderType);
    
    // Add the new shader
    GLuint shader;
    if (compileShader(shader, shaderType, source)) {
        shaders.push_back(shader);
        shaderMap[shaderType] = shader;
        std::cout << "Shader type " << shaderType << " replaced successfully" << std::endl;
        
        // Clear uniform cache since the program will need to be relinked
        uniformCache.clear();
        
        return true;
    }
    return false;
}

bool Shader::removeShader(SHADER_TYPE shaderType) {
    auto it = shaderMap.find(shaderType);
    if (it != shaderMap.end()) {
        GLuint shaderToRemove = it->second;
        
        // Remove from shaders vector
        auto vectorIt = std::find(shaders.begin(), shaders.end(), shaderToRemove);
        if (vectorIt != shaders.end()) {
            shaders.erase(vectorIt);
        }
        
        // Delete the OpenGL shader object
        glDeleteShader(shaderToRemove);
        
        // Remove from map
        shaderMap.erase(it);
        
        std::cout << "Shader type " << shaderType << " removed successfully" << std::endl;
        
        // Clear uniform cache since the program will need to be relinked
        uniformCache.clear();
        
        return true;
    }
    
    std::cout << "Warning: Shader type " << shaderType << " not found" << std::endl;
    return false;
}

bool Shader::linkProgram() {
    if (shaders.empty()) {
        std::cout << "Error: No shaders to link" << std::endl;
        return false;
    }
    
    shaderProgram = glCreateProgram();
    
    // Attach all shaders
    for (GLuint shader : shaders) {
        glAttachShader(shaderProgram, shader);
    }
    
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");
    
    // Check linking success
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        return false;
    }
    
    // Detach shaders after linking
    for (GLuint shader : shaders) {
        glDetachShader(shaderProgram, shader);
    }
    
    return true;
}

bool Shader::loadFromFiles(const std::unordered_map<SHADER_TYPE, std::string>& shaderFiles) {
    std::unordered_map<SHADER_TYPE, std::string> sources;
    
    for (const auto& pair : shaderFiles) {
        std::ifstream file(pair.second);
        if (!file.is_open()) {
            std::cout << "Error: Failed to open shader file: " << pair.second << std::endl;
            return false;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        sources[pair.first] = buffer.str();
    }
    
    return loadFromSources(sources);
}

bool Shader::loadFromSources(const std::unordered_map<SHADER_TYPE, std::string>& shaderSources) {
    // Clear existing shaders
    for (GLuint shader : shaders) {
        glDeleteShader(shader);
    }
    shaders.clear();
    shaderMap.clear();
    
    // Add all provided shaders
    for (const auto& pair : shaderSources) {
        GLuint shader;
        if (compileShader(shader, pair.first, pair.second)) {
            shaders.push_back(shader);
            shaderMap[pair.first] = shader;
        } else {
            return false;
        }
    }
    
    return linkProgram();
}

GLint Shader::getUniformLocation(const std::string& name) {
    auto it = uniformCache.find(name);
    if (it != uniformCache.end()) {
        return it->second;
    }
    
    GLint location = glGetUniformLocation(shaderProgram, name.c_str());
    uniformCache[name] = location;
    
    if (location == -1) {
        std::cout << "Warning: Uniform '" << name << "' not found" << std::endl;
    }
    
    return location;
}

void Shader::setUniform(const std::string& name, float value) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void Shader::setUniform(const std::string& name, int value) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void Shader::setUniform(const std::string& name, bool value) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, value ? 1 : 0);
    }
}

void Shader::setUniform(const std::string& name, float x, float y, float z) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform3f(location, x, y, z);
    }
}

void Shader::setUniform(const std::string& name, float x, float y, float z, float w) {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform4f(location, x, y, z, w);
    }
}

void Shader::bind() {
    if (shaderProgram == 0) {
        std::cout << "Error: Cannot bind shader - no program created" << std::endl;
        return;
    }
    
    if (!bound) {
        glUseProgram(shaderProgram);
        bound = true;
        LOG_INFO("Shader bound and activated successfully");
    } else {
        LOG_WARN("Shader is already bound");
    }
}

void Shader::unbind() {
    if (bound) {
        glUseProgram(0);
        bound = false;
        LOG_INFO("Shader unbound successfully");
    } else {
        LOG_WARN("Shader is already unbound");
    }
}
