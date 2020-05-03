#include "stella/components/sprite2.hpp"
#include "stella/graphics/texture.hpp"

namespace stella
{
namespace component
{
    int SpriteT::get_width()
    {
        int width = -1;
        if (texture_ptr)
        {
            // If there's an area defined
            if (bottom_right.x != 0.f || bottom_right.y != 0.f)
            {
                width = bottom_right.x - top_left.x;
            }
            // Otherwise it's a tilemap
            else
            {
                width = static_cast<int>(texture_ptr->GetWidth()) / hframes;
            }
        }
        return width;
    }

    int SpriteT::get_height()
    {
        int height = -1;
        
        if (texture_ptr)
        {
            // If there's an area defined
            if (bottom_right.x != 0.f || bottom_right.y != 0.f)
            {
                height = bottom_right.y - top_left.y;
            }
            // Otherwise it's a tilemap
            else
            {
                height = static_cast<int>(texture_ptr->GetHeight()) / vframes;
            }
        }
        return height;
    }

    glm::vec2 SpriteT::get_uv()
    {
        // If there's a texture and we didn't already set an UV value
        if (texture_ptr && !user_has_set_uv)
        {
            float texw = static_cast<float>(texture_ptr->GetWidth());
            float texh = static_cast<float>(texture_ptr->GetHeight());

            // If there's an area defined
            if (bottom_right.x != 0.f || bottom_right.y != 0.f)
            {
                uv.x = top_left.x / texw;
                uv.y = 1.0 - top_left.y / texh;
                //std::cout << uv.x << " " << uv.y << '\n';
            }
            // Otherwise it's a tilemap
            else
            {
                unsigned max_frames = hframes * vframes;
                float framex = static_cast<float>(frame % hframes);
                float framey = static_cast<float>((frame % max_frames) / hframes);
                // Multiply the x coord of the frame in the tile map by the normalized value of the width one frame.
                uv.x = framex * (texw / static_cast<float>(hframes)) / texw;
                // Multiply the y coord of the frame in the tile map by the normalized value of the height one frame.
                // Invert the value as the y axis is upwards for OpenGL
                uv.y = 1.0f - framey * (texh / static_cast<float>(vframes)) / texh;
            }
        }
        return uv;
    }

    void SpriteT::set_texture(const std::string& texture)
    {
        loaded = false;
        this->texture = texture;
    }

    void SpriteT::set_uv(const glm::vec2& uv)
    {
        user_has_set_uv = true;
        this->uv.x = uv.x;
        this->uv.y = uv.y;
    }
}
}