#include "Model.h"
#include "assimp/postprocess.h"
#include "pch.h"
#include "renderer/Render.h"

Model::Model(std::string path) {
  loadModel(path);
}

void Model::draw(Shader &shader) {
  for (unsigned int i{0}; i < meshes.size(); i++) {
    meshes[i]->draw(shader);
  }
}

void Model::update(Shader *shader) {
  shader->setVec4("modelColor", color);

  // shader->setFloat("roughness", roughness);
  // shader->setFloat("metallic", metallic);

  glm::mat4 model{1.0f};

  model = glm::scale(model, size);

  shader->setMat4("model", model);
}

void Model::loadModel(std::string path) {
  Assimp::Importer import;

  const uint32_t cMeshImportFlags = aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_SortByPType |
                                    aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_OptimizeMeshes |
                                    aiProcess_ValidateDataStructure
      /* | aiProcess_GlobalScale*/
      /* | aiProcess_FlipUVs */;

  const aiScene *scene = import.ReadFile(path, cMeshImportFlags);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return;
  }

  // Clear previous meshes and textures
  meshes.clear();
  texturesLoaded.clear();

  directory = path.substr(0, path.find_last_of('/'));

  processNode(scene->mRootNode, scene);
  size = calculateSize(scene);
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

std::unique_ptr<Mesh> Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<Texture>> textures;

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
      if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
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

    auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");

    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  return make_unique<Mesh>(vertices, indices, textures);
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                                                  std::string typeName) {

  std::vector<std::shared_ptr<Texture>> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString filename;
    mat->GetTexture(type, i, &filename);

    std::string path = directory + "/" + std::string(filename.C_Str());

    bool skip = false;
    for (unsigned int j = 0; j < texturesLoaded.size(); j++) {
      if (std::strcmp(texturesLoaded[j]->getPath().c_str(), path.c_str()) == 0) {
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

glm::vec3 Model::calculateSize(const aiScene *scene) {
  float minY = std::numeric_limits<float>::max();
  float maxY = std::numeric_limits<float>::lowest();
  float minX = std::numeric_limits<float>::max();
  float maxX = std::numeric_limits<float>::lowest();
  float minZ = std::numeric_limits<float>::max();
  float maxZ = std::numeric_limits<float>::lowest();

  for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[i];
    for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
      float y = mesh->mVertices[j].y;
      float x = mesh->mVertices[j].x;
      float z = mesh->mVertices[j].z;

      if (y < minY)
        minY = y;
      if (y > maxY)
        maxY = y;

      if (x < minX)
        minX = x;
      if (x > maxX)
        maxX = x;

      if (z < minZ)
        minZ = z;
      if (z > maxZ)
        maxZ = z;
    }
  }

  return {maxY - minY, maxX - minX, maxZ - minZ};
}
