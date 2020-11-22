#include "stella/graphics/texture2.hpp"
#include "stella/graphics/opengl.hpp" // IWYU pragma: export
#include "stella/core/asset.hpp" // IWYU pragma: export

extern "C"
{
#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
}

namespace stella
{
namespace graphics
{

  Texture::Texture(const std::string& filepath)
  {
    load(filepath);
  }

  Texture::~Texture()
  {
    if (m_has_loaded)
    {
      glDeleteTextures (1, &m_id);
    }
  }

  void Texture::load(const std::string& filepath)
  {
    int width;
    int height;
    int channels;
    unsigned char* image_data;

    image_data = stbi_load (filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (image_data == nullptr)
    {
      throw std::runtime_error("It wasn't possible to load " + filepath);
    }

    m_width = width;
    m_height = height;

    glGenTextures (1, &m_id);
    glBindTexture (GL_TEXTURE_2D, m_id);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap (GL_TEXTURE_2D);

    stbi_image_free (image_data);
    glBindTexture (GL_TEXTURE_2D, 0);
    m_has_loaded = true;
  }

  void Texture::bind()
  {
    glBindTexture (GL_TEXTURE_2D, m_id);
  }

  void Texture::unbind()
  {
    glBindTexture (GL_TEXTURE_2D, 0);
  }

}
}
