#pragma once

#include "stella/components/sprite.hpp"
#include "imgui.h"

namespace stella
{
namespace widget
{
    struct Sprite
    {
        Sprite (const std::vector<std::string>& texture_list) : m_texture_list (texture_list) {}
        void operator () (entt::registry& registry, const entt::entity entity)
        {
            const auto& sprite = registry.get<component::SpriteT>(entity);
            int frame = sprite.frame;
            ImGui::PushID ("sprite#inspector");

            std::string new_texture = sprite.texture;
            if (ImGui::BeginCombo("Texture", sprite.texture.c_str(), 0))
            {
                for (const auto& tex : m_texture_list)
                {
                    const bool is_selected = (sprite.texture == tex);
                    if(ImGui::Selectable(tex.c_str(), is_selected))
                    {
                        new_texture = tex;
                    }

                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::InputInt ("Frame", &frame);
            ImGui::PopID();

            if (frame != sprite.frame || new_texture != sprite.texture)
            {
                registry.patch<component::SpriteT>(entity, [&frame, &new_texture] (auto& spr)
                {
                    spr.frame = frame;
                    spr.texture = new_texture;
                    spr.texture_ptr = nullptr;
                    spr.loaded = false;
                });
            }
        }

    private:
        const std::vector<std::string>& m_texture_list;
    };
}
}