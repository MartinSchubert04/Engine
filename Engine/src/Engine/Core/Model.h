#pragma once

#include "Shader.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>
#include "Texture.h"

class Model {
public:
  glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
  float roughness = 0.2f;
  float metallic = 0.1f;
  glm::vec3 size;

  Model(std::string path);

  void draw(Shader &shader);
  void update(Shader *shader);

private:
  std::vector<std::unique_ptr<Mesh>> meshes;
  std::string directory;
  std::vector<std::shared_ptr<Texture>> texturesLoaded;

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  std::unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);

  std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

  glm::vec3 calculateSize(const aiScene *scene);
};
