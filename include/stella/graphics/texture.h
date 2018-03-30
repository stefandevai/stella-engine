#pragma once

#include <glad/glad.h>
#include <string>

namespace stella {
namespace graphics {
class Texture {
public:
  Texture(const std::string name, const char *texPath);
  ~Texture();
  void Bind();
  void Unbind();

  inline const GLuint GetWidth() const { return (GLint)Width; }
  inline const GLuint GetHeight() const { return (GLint)Height; }
  inline const GLuint GetID() const { return ID; }
  inline const GLuint GetCacheID() const { return CacheID; }
  inline const std::string &GetName() const { return Name; }
  inline const bool IsCached() { return this->Cached; }

  inline void SetCached(GLfloat cache_id) {
    this->Cached = true;
    this->CacheID = cache_id;
  }

private:
  GLuint ID, Width, Height;
  GLfloat CacheID;
  std::string Name;
  bool Cached;
  void load(const char *texPath);
};
} // namespace graphics
} // namespace stella
