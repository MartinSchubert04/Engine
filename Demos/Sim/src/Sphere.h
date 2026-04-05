#pragma once
#include "Core/Base.h"
#include "Core/Mesh.h"
#include "Core/Texture.h"
#include "Renderer/Shader.h"
#include "Renderer/Transform.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include <Engine.h>
#include <vector>

class TerrainFace {
public:
  TerrainFace() = default;
  TerrainFace(int resolution, glm::vec3 localUp, Ref<Engine::Texture> texture = nullptr) :
      mTexture(texture), mResolution(resolution), mLocalUp(localUp) {
    mAxisA = glm::vec3(mLocalUp.y, mLocalUp.z, mLocalUp.x);
    mAxisB = glm::cross(mLocalUp, mAxisA);
    createMesh(texture);
  }

  void createMesh(Ref<Engine::Texture> texture = nullptr) {
    std::vector<uint32_t> indices;
    std::vector<glm::vec2> uvs;
    // res - 1 = the amount of squares in a row (this is squeared given this is a face)
    // 6 represents: 2 (triangles per squeare) * 3 (vertices per triangle)
    indices.resize((mResolution - 1) * (mResolution - 1) * 6);
    std::vector<glm::vec3> vertices;

    int triIndex = 0;

    for (uint32_t y{0}; y < mResolution; y++) {
      for (uint32_t x{0}; x < mResolution; x++) {
        int i = x + y * mResolution;
        glm::vec2 percent = glm::vec2(x, y) / (float)(mResolution - 1);

        // percent needs to be in a range of [-1, 1] for that is offseted by .5f a then mult by 2
        // then is transformed along the A axis and B axis
        glm::vec3 pointOnUnitCube = mLocalUp + (percent.x - .5f) * 2 * mAxisA + (percent.y - .5f) * 2 * mAxisB;

        // normalize the point so all vertices are at the same distance from the center
        glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);

        vertices.push_back(pointOnUnitSphere);
        uvs.push_back(percent);

        if (x != mResolution - 1 && y != mResolution - 1) {
          indices[triIndex] = i;
          indices[triIndex + 1] = i + mResolution;
          indices[triIndex + 2] = i + mResolution + 1;

          indices[triIndex + 3] = i;
          indices[triIndex + 4] = i + mResolution + 1;
          indices[triIndex + 5] = i + 1;

          triIndex += 6;
        }
      }
    }

    std::vector<Vertex> meshVertices;

    for (int i = 0; i < vertices.size(); i++) {
      Vertex v;
      v.position = vertices[i];
      v.normal = vertices[i];
      v.color = {0.8, 0.8, 0.8, 1};
      v.texCoords = uvs[i];
      v.useDiffuseTexture = 1;
      meshVertices.push_back(v);
    }

    std::vector<Ref<Engine::Texture>> textures;
    if (texture)
      textures.push_back(texture);

    mMesh = Mesh(meshVertices, indices, textures);
  }

  void draw(Ref<Shader> shader) {
    Engine::Transform transform;
    transform.setModel(shader);

    shader->setVec4("modelColor", {.8, .8, .8, 1});

    mMesh.draw(*shader);
  }

private:
  Engine::Mesh mMesh;
  int mResolution;
  glm::vec3 mLocalUp;
  glm::vec3 mAxisA;
  glm::vec3 mAxisB;
  Ref<Texture> mTexture;
};

class Sphere {
public:
  Sphere() = default;
  Sphere(int resolution, Ref<Engine::Texture> cubemap = nullptr) : mFaceResolution(resolution) {
    for (int i = 0; i < 6; i++)
      mFaces.push_back(TerrainFace(mFaceResolution, mDirections[i], cubemap));
  }

  void draw(Ref<Shader> shader) {
    for (auto &f : mFaces) {
      f.draw(shader);
    }
  }

private:
  int mFaceResolution;
  std::vector<TerrainFace> mFaces;
  std::vector<glm::vec3> mDirections = {{0, 1, 0}, {0, -1, 0}, {-1, 0, 0}, {1, 0, 0}, {0, 0, 1}, {0, 0, -1}};
};
