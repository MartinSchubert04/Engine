#include "Core/Vertex.h"
#include "Renderer/Buffer.h"

namespace Engine {

class OpenGLvertexBuffer : public VertexBuffer {
public:
  OpenGLvertexBuffer(std::vector<Vertex> vertices);
  OpenGLvertexBuffer(std::vector<float> vertices);
  ~OpenGLvertexBuffer();

  void bind() const override;
  void unbind() const override;

  void setLayout(const BufferLayout &layout) override { mLayout = layout; };
  const BufferLayout &getLayout() const override { return mLayout; };

private:
  unsigned int mRendererID;
  BufferLayout mLayout;
};

class OpenGLindexBuffer : public IndexBuffer {
public:
  OpenGLindexBuffer(std::vector<uint32_t> &indices);
  ~OpenGLindexBuffer();

  void bind() const override;
  void unbind() const override;

  uint32_t getCount() override { return mCount; };

private:
  unsigned int mRendererID;
  uint32_t mCount;
};

}  // namespace Engine
