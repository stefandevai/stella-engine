#pragma once

#include "../core/resource.hpp"
#include <string>

namespace stella
{
namespace graphics
{
  class Texture : public core::Resource
  {
  public:
    // Horizontal and vertical frames
    unsigned hframes = 1, vframes = 1;

  public:
    // Texture(const std::string &name, const char *texPath);
    Texture (const std::string& path, const unsigned hframes = 1, const unsigned vframes = 1);
    Texture (const unsigned w, const unsigned h);
    Texture();
    ~Texture();
    void LoadChar (unsigned w, unsigned h, unsigned char* data);
    void Bind();
    static void Unbind();

    inline unsigned int GetWidth() const { return (int) Width; }
    inline unsigned int GetHeight() const { return (int) Height; }
    inline unsigned int GetID() const { return ID; }
    inline unsigned int GetCacheID() const { return CacheID; }
    // inline const std::string &GetName() const { return Name; }
    inline bool IsCached() const { return this->Cached; }

    inline void SetCached (float cache_id)
    {
      this->Cached  = true;
      this->CacheID = cache_id;
    }

  private:
    unsigned int ID, Width, Height;
    float CacheID;
    // std::string Name;
    bool Cached;
    void load (const char* texPath);
  };

} // namespace graphics
} // namespace stella
