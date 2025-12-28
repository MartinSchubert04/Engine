#pragma once

#include "Shader.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>

using namespace std;

class Model {
public:
  Model(string path);

  void draw(Shader &shader);

private:
  vector<unique_ptr<Mesh>> meshes;
  string directory;
  vector<Texture> texturesLoaded;

  void loadModel(string path);
  void processNode(aiNode *node, const aiScene *scene);
  unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
  vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                       string typeName);
};
