#include "Model.h"
#include "common.h"
#include "renderer/Render.h"

Model::Model(string path) {
  loadModel(path);
}

void Model::draw(Shader &shader) {
  for (unsigned int i{0}; i < meshes.size(); i++) {
    meshes[i]->draw(shader);
  }
}

void Model::loadModel(string path) {
  Assimp::Importer import;
  const aiScene *scene =
      import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
    return;
  }

  // Clear previous meshes and textures
  meshes.clear();
  texturesLoaded.clear();

  directory = path.substr(0, path.find_last_of('/'));

  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  // process all the node's meshes (if any)
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }
  // then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

unique_ptr<Mesh> Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<shared_ptr<Texture>> textures;

  for (unsigned int i{0}; i < mesh->mNumVertices; i++) {
    Vertex vertex;

    glm::vec3 vector;
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.position = vector;

    vector.x = mesh->mNormals[i].x;
    vector.y = mesh->mNormals[i].y;
    vector.z = mesh->mNormals[i].z;
    vertex.normal = vector;

    if (mesh->mTextureCoords[0])  // does the mesh contain texture coordinates?
    {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.texCoords = vec;
    } else
      vertex.texCoords = glm::vec2(0.0f, 0.0f);

    if (scene->mNumMaterials > mesh->mMaterialIndex) {
      const auto &mat = scene->mMaterials[mesh->mMaterialIndex];
      aiColor4D diffuse;
      if (AI_SUCCESS ==
          aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
        vertex.color = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
      }

      if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        vertex.useDiffuseTexture = 1.f;
      } else {
        vertex.useDiffuseTexture = 0.f;
      }
    }

    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE,
                                            "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR,
                                             "texture_specular");

    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  return make_unique<Mesh>(vertices, indices, textures);
}

vector<shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial *mat,
                                                        aiTextureType type,
                                                        string typeName) {

  vector<shared_ptr<Texture>> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString filename;
    mat->GetTexture(type, i, &filename);

    string path = directory + "/" + string(filename.C_Str());

    bool skip = false;
    for (unsigned int j = 0; j < texturesLoaded.size(); j++) {
      if (std::strcmp(texturesLoaded[j]->getPath().c_str(), path.c_str()) ==
          0) {
        textures.push_back(texturesLoaded[j]);
        skip = true;
        break;
      }
    }
    if (!skip) {  // if texture hasn't been loaded already, load it

      auto texture = make_shared<Texture>(path);
      texture->setType(typeName);

      if (texture->isValid()) {
        textures.push_back(texture);
        texturesLoaded.push_back(texture);
      } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
      }
    }
  }

  return textures;
}
