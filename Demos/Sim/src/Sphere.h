#pragma once
#include "Core/Mesh.h"
#include "glm/trigonometric.hpp"
#include <Engine.h>
#include <cstdint>
#include <vector>

struct PlanetProperties {
  float size;
  float rotationSpeed;
  float angleTilt;
  float orbitSpeed;
  float acceleration;
  float mass;
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

  void createMesh(Ref<Engine::Texture> texture = nullptr);

  void setTexture(Ref<Texture> texture) { mMesh.textures.push_back(texture); }

  void draw(Ref<Shader> shader, PlanetProperties planetProps);

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
  Sphere(int resolution, PlanetProperties props = {1.f, 1.f, 0.f, 0.f, 0.f, 0.f});
  Sphere(float radius, glm::vec2 segments, glm::vec3 pos, PlanetProperties props);

  void draw(Ref<Shader> shader);
  void drawCubeSphere(Ref<Shader> shader);

  void onUpdate(Engine::DeltaTime dt);

  void setTexturesCubeSphere(std::vector<Ref<Texture>> textures);
  void setTextures(std::vector<Ref<Texture>> textures);
  void setTexture(Ref<Texture> texture);
  void setProperties(PlanetProperties props) { _properties = props; }
  void setColor(glm::vec4 color) { _color = color; }

private:
  float _radius;
  Engine::Scope<Engine::Mesh> _mesh;

  glm::vec4 _color;
  PlanetProperties _properties;
  int _faceResolution;
  std::vector<TerrainFace> _faces;
  std::vector<glm::vec3> _directions = {{0, 1, 0}, {0, -1, 0}, {-1, 0, 0}, {1, 0, 0}, {0, 0, 1}, {0, 0, -1}};
};
