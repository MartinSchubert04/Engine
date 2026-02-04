#pragma once
#include "pch.h"
#include "Types.h"
#include "Core/Vertex.h"

namespace Engine {

struct BufferElement {

  std::string name;
  Types::ShaderDataType type;
  uint32_t size;
  uint32_t offset;
  bool normalized;

  BufferElement() {};
  BufferElement(Types::ShaderDataType type, const std::string &name, bool normalized = false) :
      name(name), type(type), size(Types::ShaderDataTypeSize(type)), offset(0), normalized(normalized) {};

  uint32_t getElementCount() const {
    switch (type) {
    case Types::ShaderDataType::float1:
      return 1;
    case Types::ShaderDataType::float2:
      return 2;
    case Types::ShaderDataType::float3:
      return 3;
    case Types::ShaderDataType::float4:
      return 4;
    case Types::ShaderDataType::int1:
      return 1;
    case Types::ShaderDataType::int2:
      return 2;
    case Types::ShaderDataType::int3:
      return 3;
    case Types::ShaderDataType::int4:
      return 4;
    case Types::ShaderDataType::boolean:
      return 1;
    case Types::ShaderDataType::mat3:
      return 3 * 3;
    case Types::ShaderDataType::mat4:
      return 4 * 4;
    }
    CORE_ASSERT(false, "Unkown shader data type");
    return 0;
  }

  GLenum getElementType() const {
    switch (type) {
    case Types::ShaderDataType::float1:
      return GL_FLOAT;
    case Types::ShaderDataType::float2:
      return GL_FLOAT;
    case Types::ShaderDataType::float3:
      return GL_FLOAT;
    case Types::ShaderDataType::float4:
      return GL_FLOAT;
    case Types::ShaderDataType::int1:
      return GL_INT;
    case Types::ShaderDataType::int2:
      return GL_INT;
    case Types::ShaderDataType::int3:
      return GL_INT;
    case Types::ShaderDataType::int4:
      return GL_INT;
    case Types::ShaderDataType::boolean:
      return GL_BOOL;
    case Types::ShaderDataType::mat3:
      return GL_FLOAT;
    case Types::ShaderDataType::mat4:
      return GL_FLOAT;
    }
    CORE_ASSERT(false, "Unkown shader data type");
    return 0;
  }
};

class BufferLayout {
public:
  BufferLayout() {};
  BufferLayout(const std::initializer_list<BufferElement> &elements) : mElements(elements) {
    calculateOffsetsAndStride();
  }

  inline const std::vector<BufferElement> &getElements() const { return mElements; }
  inline const uint32_t &getStride() const { return mStride; }

  inline std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
  inline std::vector<BufferElement>::iterator end() { return mElements.end(); }

  inline std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
  inline std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }

private:
  void calculateOffsetsAndStride() {
    uint32_t offset = 0;
    mStride = 0;

    for (auto &e : mElements) {
      e.offset = offset;  // assigns 0 to the first, and the size of data marks the initial byte of the current element
      offset += e.size;
      mStride += e.size;
    }
  }

private:
  std::vector<BufferElement> mElements;
  uint32_t mStride;
};

class VertexBuffer {
public:
  virtual ~VertexBuffer() {};

  virtual void bind() const = 0;
  virtual void unbind() const = 0;
  virtual void setLayout(const BufferLayout &layout) = 0;
  virtual const BufferLayout &getLayout() const = 0;

  static Ref<VertexBuffer> create(std::vector<Vertex> vertices);
  static Ref<VertexBuffer> create(std::vector<float> vertices);
};

class IndexBuffer {
public:
  virtual ~IndexBuffer() {};

  virtual void bind() const = 0;
  virtual void unbind() const = 0;

  virtual uint32_t getCount() = 0;

  static Scope<IndexBuffer> create(std::vector<uint32_t> &indices);
};

}  // namespace Engine
