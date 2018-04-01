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

  inline GLuint GetWidth() const { return (GLint)Width; }
  inline GLuint GetHeight() const { return (GLint)Height; }
  inline GLuint GetID() const { return ID; }
  inline GLuint GetCacheID() const { return CacheID; }
  inline const std::string &GetName() const { return Name; }
  inline bool IsCached() const { return this->Cached; }

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
