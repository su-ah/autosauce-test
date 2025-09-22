#ifndef SHADER_JAVA
#define SHADER_JAVA
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "glad/glad.h"
#include "shared/Logger.hpp"

enum SHADER_TYPE {
  UNINITIALIZED,
  FRAGMENT,
  VERTEX,
  GEOMETRY,
  COMPUTE,
  SWAG
};

class Shader {
private:
SHADER_TYPE type = UNINITIALIZED;
bool bound = false;
GLuint shaderProgram = 0;
std::vector<GLuint> shaders; // Store multiple shader objects
std::unordered_map<SHADER_TYPE, GLuint> shaderMap; // Map shader types to their OpenGL IDs
std::unordered_map<std::string, GLint> uniformCache; // Cache uniform locations

// Helper methods
GLenum getGLShaderType(SHADER_TYPE type);
bool compileShader(GLuint& shader, SHADER_TYPE shaderType, const std::string& source);
void checkCompileErrors(GLuint shader, const std::string& type);


public:

inline SHADER_TYPE get_type() {
	return type;
}

inline bool is_bound() {
	return bound;
}

// Shader loading and compilation
bool addShader(SHADER_TYPE shaderType, const std::string& source);
bool replaceShader(SHADER_TYPE shaderType, const std::string& source);
bool removeShader(SHADER_TYPE shaderType);
bool linkProgram();
bool loadFromFiles(const std::unordered_map<SHADER_TYPE, std::string>& shaderFiles);
bool loadFromSources(const std::unordered_map<SHADER_TYPE, std::string>& shaderSources);

// Uniform handling
GLint getUniformLocation(const std::string& name);
void setUniform(const std::string& name, float value);
void setUniform(const std::string& name, int value);
void setUniform(const std::string& name, bool value);
void setUniform(const std::string& name, float x, float y, float z);
void setUniform(const std::string& name, float x, float y, float z, float w);

void bind();
void unbind();

Shader() {
  LOG_DEBUG("Shader object created");
}

~Shader(){
  LOG_DEBUG("Shader object destroyed");
  if (bound) {    
    LOG_WARN("Shader is still bound during destruction. Forcing unbind.");
    unbind();
  }
  
  // Clean up all shader objects
  for (GLuint shader : shaders) {
    if (shader != 0) {
      glDeleteShader(shader);
    }
  }
  shaders.clear();
  shaderMap.clear();
  
  // Clean up shader program
  if (shaderProgram != 0) {
    glDeleteProgram(shaderProgram);
  }
}
};



#endif //SHADER_JAVA
