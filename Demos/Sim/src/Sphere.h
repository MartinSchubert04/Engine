#pragma once
#include "glm/trigonometric.hpp"
#include <Engine.h>
#include <cstdint>
#include <vector>

struct PlanetProperties {
  float size;
  float rotationSpeed;
  float angleTilt;
  float orbitSpeed;
};

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

        // u: angulo horizontal
        float u = 0.5f + (atan2(pointOnUnitSphere.z, pointOnUnitSphere.x) / (2 * glm::pi<float>()));

        // v: altura
        float v = 0.5f - (asin(pointOnUnitSphere.y) / glm::pi<float>());

        uvs.push_back({u, v});

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

  void setTexture(Ref<Texture> texture) { mMesh.textures.push_back(texture); }

  void draw(Ref<Shader> shader, PlanetProperties planetProps) {
    Engine::Transform transform;
    transform.rotate(glm::radians(planetProps.angleTilt), {0, 0, 1});
    transform.rotate(planetProps.rotationSpeed, {0, 1, 0});
    transform.scale(planetProps.size);
    shader->setVec4("modelColor", {.8, .8, .8, 1});

    transform.setModel(shader);
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
  Sphere(int resolution, PlanetProperties props = {1.f, 1.f, 0.f, 0.f}) :
      mFaceResolution(resolution), mProperties(props) {
    for (int i = 0; i < 6; i++)
      mFaces.push_back(TerrainFace(mFaceResolution, mDirections[i]));
  }

  void draw(Ref<Shader> shader) {
    for (auto &f : mFaces) {
      f.draw(shader, mProperties);
    }
  }

  void onUpdate(Engine::DeltaTime dt) { mProperties.rotationSpeed += 20.0f * dt.getSeconds(); }

  void setTextures(std::vector<Ref<Texture>> textures) {
    for (uint32_t i{0}; i < mFaces.size(); i++) {
      for (uint32_t j{0}; j < textures.size(); j++) {
        mFaces[i].setTexture(textures[j]);
      }
    }
  }

  void setProperties(PlanetProperties props) { mProperties = props; }

private:
  PlanetProperties mProperties;
  int mFaceResolution;
  std::vector<TerrainFace> mFaces;
  std::vector<glm::vec3> mDirections = {{0, 1, 0}, {0, -1, 0}, {-1, 0, 0}, {1, 0, 0}, {0, 0, 1}, {0, 0, -1}};
};
