#include "Shader.h"
#include "Core/Assert.h"
#include "Core/Base.h"
#include "Core/Log.h"
#include "pch.h"

namespace Engine {

Shader::Shader(const std::string &name, const std::string &vertexPath, const std::string &fragPath) : mName(name) {
  compile(vertexPath, fragPath);
}
Shader::Shader(const std::string &vertexPath, const std::string &fragPath) {

  compile(vertexPath, fragPath);

  auto lastSlash = vertexPath.find_last_of("/\\");
  lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
  auto lastDot = vertexPath.rfind(".");
  auto count = lastDot == std::string::npos ? vertexPath.size() - lastSlash : lastDot - lastSlash;
  mName = vertexPath.substr(lastSlash, count);
}

std::string Shader::ReadFromFile(const std::string &filepath) {
  // 1. retrieve the vertex/fragment source code from filePath
  std::string filepathCode;
  std::ifstream filepathFile;

  // ensure ifstream objects can throw exceptions:
  filepathFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    // open files
    filepathFile.open(filepath);
    std::stringstream fileStream;

    // read file's buffer contents into streams
    fileStream << filepathFile.rdbuf();

    // close file handlers
    filepathFile.close();

    // convert stream into string
    filepathCode = fileStream.str();

  } catch (std::ifstream::failure &e) {

    CORE_ERROR("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {0} \n shader: [{1}] ", e.what(), filepath);
  }

  return filepathCode;
}

void Shader::compile(const std::string &vertexPath, const std::string &fragPath) {

  std::string vertexSrc = ReadFromFile(vertexPath);
  std::string fragmentSrc = ReadFromFile(fragPath);

  const char *vShaderCode = vertexSrc.c_str();
  const char *fShaderCode = fragmentSrc.c_str();

  unsigned int vertex, fragment;
  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");
  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");
  // shader Program
  mRendererID = glCreateProgram();
  glAttachShader(mRendererID, vertex);
  glAttachShader(mRendererID, fragment);
  glLinkProgram(mRendererID);
  checkCompileErrors(mRendererID, "PROGRAM");
  // delete the shaders as they're linked into our program now and no longer
  // necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::~Shader() {
  glDeleteShader(mRendererID);
}

void Shader::bind() {
  glUseProgram(mRendererID);
}

void Shader::unbind() {
  glUseProgram(0);
}

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(mRendererID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(mRendererID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(mRendererID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(mRendererID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const {
  glUniform2f(glGetUniformLocation(mRendererID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(mRendererID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(mRendererID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(mRendererID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
  glUniform4f(glGetUniformLocation(mRendererID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
  glUniformMatrix2fv(glGetUniformLocation(mRendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
  glUniformMatrix3fv(glGetUniformLocation(mRendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(mRendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
  GLint success;
  GLchar infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);

      CORE_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {0}\n{1}", type, infoLog);
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);

      CORE_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {0}\n{1}", type, infoLog);
    }
  }
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

void ShaderLibrary::add(const Ref<Shader> &shader) {
  auto &name = shader->getName();
  CORE_ASSERT(mShaders.find(name) == mShaders.end(), "Shader already exists!");
  mShaders[name] = shader;
}

Ref<Shader> ShaderLibrary::load(const std::string &vertexPath, const std::string &fragxPath) {
  auto shader = createRef<Shader>(vertexPath, fragxPath);
  add(shader);
  return shader;
}

Ref<Shader> ShaderLibrary::load(const std::string &name, const std::string &vertexPath, const std::string &fragxPath) {
  auto shader = createRef<Shader>(name, vertexPath, fragxPath);
  add(shader);
  return shader;
}

Ref<Shader> ShaderLibrary::get(const Ref<Shader> shader) {
  auto &name = shader->getName();
  CORE_ASSERT(mShaders.find(name) != mShaders.end(), "Shader does not exist in shader library");

  return mShaders[name];
}

}  // namespace Engine
