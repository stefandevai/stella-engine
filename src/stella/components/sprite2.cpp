#include "stella/components/sprite2.hpp"
#include "stella/graphics/texture.hpp"

namespace stella
{
namespace component
{
    int SpriteT::width()
    {
        int width = -1;
        if (texture_ptr)
        {
            width = static_cast<int>(texture_ptr->GetWidth()) / hframes;
        }
        return width;
    }

    int SpriteT::height()
    {
        int height = -1;
        if (texture_ptr)
        {
            height = static_cast<int>(texture_ptr->GetHeight()) / vframes;
        }
        return height;
    }

    glm::vec2 SpriteT::uv()
    {
        glm::vec2 uv{-1.0, -1.0};
        if (texture_ptr)
        {
            unsigned texw = texture_ptr->GetWidth();
            unsigned texh = texture_ptr->GetHeight();

            unsigned max_frames = hframes * vframes;
            float framex = static_cast<float>(frame % hframes);
            float framey = static_cast<float>((frame % max_frames) / hframes);
            // Multiply the x coord of the frame in the tile map by the normalized value of the width one frame.
            uv.x = framex * (texw / static_cast<float>(hframes)) / static_cast<float>(texw);
            // Multiply the y coord of the frame in the tile map by the normalized value of the height one frame.
            // Invert the value as the y axis is upwards for OpenGL
            uv.y = 1.0f - framey * (texh / static_cast<float>(vframes)) / static_cast<float>(texh);
        }
        return uv;
    }

    void SpriteT::set_texture(const std::string& texture)
    {
        loaded = false;
        this->texture = texture;
    }
}
}