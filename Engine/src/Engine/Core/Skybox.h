#pragma once
#include "Core/Base.h"
#include "Core/Vertex.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "pch.h"

namespace Engine {

class Skybox {

public:
  Skybox() {

    std::vector<glm::vec3> vertices = {// positions
                                       {-1.0f, 1.0f, -1.0f},  {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f},
                                       {1.0f, -1.0f, -1.0f},  {1.0f, 1.0f, -1.0f},   {-1.0f, 1.0f, -1.0f},

                                       {-1.0f, -1.0f, 1.0f},  {-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f},
                                       {-1.0f, 1.0f, -1.0f},  {-1.0f, 1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f},

                                       {1.0f, -1.0f, -1.0f},  {1.0f, -1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
                                       {1.0f, 1.0f, 1.0f},    {1.0f, 1.0f, -1.0f},   {1.0f, -1.0f, -1.0f},

                                       {-1.0f, -1.0f, 1.0f},  {-1.0f, 1.0f, 1.0f},   {1.0f, 1.0f, 1.0f},
                                       {1.0f, 1.0f, 1.0f},    {1.0f, -1.0f, 1.0f},   {-1.0f, -1.0f, 1.0f},

                                       {-1.0f, 1.0f, -1.0f},  {1.0f, 1.0f, -1.0f},   {1.0f, 1.0f, 1.0f},
                                       {1.0f, 1.0f, 1.0f},    {-1.0f, 1.0f, 1.0f},   {-1.0f, 1.0f, -1.0f},

                                       {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, -1.0f},
                                       {1.0f, -1.0f, -1.0f},  {-1.0f, -1.0f, 1.0f},  {1.0f, -1.0f, 1.0f}};

    for (unsigned int i{0}; i < 36; i++) {
      Vertex v;
      v.position = vertices[i];
      v.color = {1.0f, 0.0f, 0.0f, 1.0f};

      mVertices.push_back(v);
    }

    mVAO = VertexArray::create();
    mVBO = VertexBuffer::create(mVertices);

    Engine::BufferLayout layout = {
        {Engine::Types::ShaderDataType::float3, "aPos"},
        {Engine::Types::ShaderDataType::float4, "aColor"},
    };

    mVBO->setLayout(layout);
    mVAO->addVertexBuffer(mVBO);
  };

  void loadCubeMap(std::vector<std::string> faces) {
    mFaces = faces;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, nrComponents;
    unsigned char *data;
    for (uint32_t i{0}; i < mFaces.size(); i++) {
      mFaces = faces;
      glGenTextures(1, &textureID);
      glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);  // <--- DEBE SER CUBE_MAP

      int width, height, nrComponents;
      for (uint32_t i = 0; i < mFaces.size(); i++) {
        unsigned char *data = stbi_load(mFaces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data) {

          GLenum format;
          if (nrComponents == 1)
            format = GL_RED;
          else if (nrComponents == 3)
            format = GL_RGB;
          else if (nrComponents == 4)
            format = GL_RGBA;

          glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
          glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
          stbi_image_free(data);
        } else {
          CORE_ERROR("Error loading: {0}", mFaces[i]);
        }
      }

      // Parámetros obligatorios para Cubemaps
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
  }

  void bind() { glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); }

  void draw() {
    mVAO->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  std::vector<Vertex> &getVertices() { return mVertices; }

private:
  uint32_t textureID;
  std::vector<std::string> mFaces;
  std::vector<Vertex> mVertices;

  Ref<VertexBuffer> mVBO;
  Ref<VertexArray> mVAO;
};

}  // namespace Engine
