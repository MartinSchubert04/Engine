#pragma once
#include "Core/Base.h"
#include "Core/Log.h"
#include "Core/Vertex.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Renderer.h"
#include "pch.h"

namespace Engine {

class Skybox {

public:
  Skybox() {

    mVertices = {// positions
                 -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
                 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

                 -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
                 -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

                 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

                 -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

                 -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

                 -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
                 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

    // for (unsigned int i{0}; i < 36; i++) {
    //   Vertex v;
    //   v.position = vertices[i];
    //   v.color = {1.0f, 0.0f, 0.0f, 1.0f};

    //   v.normal = glm::normalize(glm::vec3(0, 0, 0));
    //   v.texCoords = glm::vec2(0.0f, 0.0f);

    //   mVertices.push_back(v);
    // }

    vertexArray = VertexArray::create();
    vertexBuffer = VertexBuffer::create(mVertices);

    Engine::BufferLayout layout = {
        {Engine::Types::ShaderDataType::float3, "aPos"},
    };

    vertexBuffer->setLayout(layout);
    vertexArray->addVertexBuffer(vertexBuffer);
  };

  void loadCubeMap(std::vector<std::string> faces) {
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
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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

  void bind() { glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); }

  void draw() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // only not wireframe the skybox, bad solution needs refactor

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    Renderer::submitArrays(vertexArray, 36);
  }

  std::vector<float> &getVertices() { return mVertices; }

private:
  uint32_t textureID;
  std::vector<std::string> mFaces;
  std::vector<float> mVertices;

  Ref<VertexBuffer> vertexBuffer;
  Ref<VertexArray> vertexArray;
};

}  // namespace Engine
