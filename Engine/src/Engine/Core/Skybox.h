#pragma once
#include "Core/Vertex.h"
#include "pch.h"
#include <cstdint>
#include <vector>

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

    mIndices = {// front
                0, 1, 3, 1, 2, 3,
                // back
                4, 5, 7, 5, 6, 7,
                // right
                0, 1, 4, 1, 4, 5,
                // left
                2, 3, 7, 2, 6, 7,
                // top
                0, 3, 4, 3, 4, 7,
                // bottom
                1, 2, 5, 2, 5, 6};
  };

  void loadCubeMap(std::vector<std::string> faces) {

    mFaces = faces;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    unsigned char *data;
    for (uint32_t i{0}; i < mFaces.size(); i++) {
      data = stbi_load(mFaces[i].c_str(), &width, &height, &nrChannels, 0);

      if (data) {

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      } else {
        CORE_ERROR("Cubemap error loading texture - Path: {0}", faces[i]);
      }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  }

  void bind() { glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); }

  std::vector<Vertex> &getVertices() { return mVertices; }
  std::vector<unsigned int> &getIndices() { return mIndices; }

private:
  uint32_t textureID;
  std::vector<std::string> mFaces;
  std::vector<Vertex> mVertices;
  std::vector<unsigned int> mIndices;
};

}  // namespace Engine
