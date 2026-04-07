#include "Sphere.h"
#include "Constants.h"

void TerrainFace::createMesh(Ref<Engine::Texture> texture) {
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

      float u = 0.5f + (atan2(pointOnUnitSphere.z, pointOnUnitSphere.x) / (2 * glm::pi<float>()));
      float v = 0.5f - (asin(pointOnUnitSphere.y) / glm::pi<float>());

      if (u < 0.0f)
        u += 1.0f;
      if (u > 1.0f)
        u -= 1.0f;

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

void TerrainFace::draw(Ref<Shader> shader, PlanetProperties planetProps) {
  Engine::Transform transform;
  transform.rotate(glm::radians(planetProps.angleTilt), {0, 0, 1});
  transform.rotate(planetProps.rotationSpeed, {0, 1, 0});
  transform.scale(planetProps.size);
  shader->setVec4("modelColor", {.8, .8, .8, 1});

  transform.setModel(shader);
  mMesh.draw(shader);
}

//  ── Sphere ───────────────────────────────────────────

Sphere::Sphere(int resolution, PlanetProperties props) : mFaceResolution(resolution), mProperties(props) {
  for (int i = 0; i < 6; i++)
    mFaces.push_back(TerrainFace(mFaceResolution, mDirections[i]));
}

Sphere::Sphere(float radius, glm::vec2 segments, glm::vec3 pos, PlanetProperties props) {

  this->_color = glm::vec4(.8, .8, .8, 1);

  std::vector<Engine::Vertex> vertices;
  std::vector<unsigned int> indices;

  for (int y{0}; y <= segments.y; y++) {
    for (int x{0}; x <= segments.x; x++) {
      Vertex vertex;

      float xSeg = float(x) / segments.x;
      float ySeg = float(y) / segments.y;

      float xPos = std::cos(xSeg * 2.0f * PI) * std::sin(ySeg * PI);
      float yPos = std::cos(ySeg * PI);
      float zPos = std::sin(xSeg * 2.0f * PI) * std::sin(ySeg * PI);

      vertex.position = glm::vec3(xPos, yPos, zPos);

      // calculate normal
      vertex.normal = glm::normalize(glm::vec3(xPos, yPos, zPos));
      vertex.texCoords = glm::vec2(xSeg, ySeg);
      vertex.color = _color;
      vertex.useDiffuseTexture = 1.f;

      vertices.push_back(vertex);
    }
  }

  for (int y = 0; y < segments.y; y++) {
    for (int x = 0; x < segments.x; x++) {
      unsigned int curRow = y * (segments.x + 1);
      unsigned int nextRow = (y + 1) * (segments.x + 1);

      // Triángulo 1
      indices.push_back(curRow + x);
      indices.push_back(nextRow + x);
      indices.push_back(curRow + x + 1);

      // Triángulo 2
      indices.push_back(nextRow + x);
      indices.push_back(nextRow + x + 1);
      indices.push_back(curRow + x + 1);
    }
  }

  _mesh = Engine::createScope<Mesh>(vertices, indices);
}

void Sphere::draw(Ref<Shader> shader) {
  Engine::Transform transform;

  transform.rotate(glm::radians(mProperties.angleTilt), {0, 0, 1});
  transform.rotate(glm::radians(mProperties.rotationSpeed), {0, 1, 0});
  transform.scale(mProperties.size);

  transform.setModel(shader);

  _mesh->draw(shader);
}

void Sphere::drawCubeSphere(Ref<Shader> shader) {
  for (auto &f : mFaces) {
    f.draw(shader, mProperties);
  }
}

void Sphere::onUpdate(Engine::DeltaTime dt) {
  mProperties.rotationSpeed += 20.0f * dt.getSeconds();
}

void Sphere::setTexturesCubeSphere(std::vector<Ref<Texture>> textures) {
  for (uint32_t i{0}; i < mFaces.size(); i++) {
    for (uint32_t j{0}; j < textures.size(); j++) {
      mFaces[i].setTexture(textures[j]);
    }
  }
}

void Sphere::setTextures(std::vector<Ref<Texture>> textures) {
  for (auto &t : textures) {
    _mesh->textures.push_back(t);
  }
}
