#include "stella/graphics/texture2.hpp"
#include "stella/graphics/opengl.hpp" // IWYU pragma: export
#include "stella/core/asset.hpp" // IWYU pragma: export

extern "C"
{
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
}

namespace stella
{
namespace graphics
{

  Texture::Texture(const std::string& filepath, const TextureType type)
    : m_type (type)
  {
    load(filepath);
  }

  Texture::~Texture()
  {
    glDeleteTextures (1, &m_id);
  }

  void Texture::load(const std::string& filepath)
  {
    int width;
    int height;
    int channels;
    unsigned char* image_data;

    //image_data = stbi_load (filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    image_data = stbi_load (filepath.c_str(), &width, &height, &channels, 0);
    if (image_data == nullptr)
    {
      throw std::runtime_error("It wasn't possible to load " + filepath);
    }

    GLenum format;
    switch (channels)
    {
      case 1:
        format = GL_RED;
        break;

      case 3:
        format = GL_RGB;
        break;

      case 4:
        format = GL_RGBA;
        break;

      default:
        format = GL_RGBA;
        break;
    }

    m_width = width;
    m_height = height;

    glGenTextures (1, &m_id);
    glBindTexture (GL_TEXTURE_2D, m_id);
    glTexImage2D (GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap (GL_TEXTURE_2D);

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free (image_data);
    glBindTexture (GL_TEXTURE_2D, 0);
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
