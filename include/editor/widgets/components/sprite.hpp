#pragma once

#include "stella/components/sprite.hpp"
#include "stella/components/layer.hpp"
#include "imgui.h"

namespace stella
{
namespace widget
{
  struct Sprite
  {
    Sprite (const std::vector<std::string>& texture_list) : m_texture_list (texture_list) {}
    void operator() (entt::registry& registry, const entt::entity entity)
    {
      const auto& sprite = registry.get<component::SpriteT> (entity);
      int frame          = sprite.frame;
      std::string new_layer = sprite.layer;
      std::string new_texture = sprite.texture;

      ImGui::PushID ("sprite#inspector");
      if (ImGui::BeginCombo ("Texture", sprite.texture.c_str(), 0))
      {
        for (const auto& tex : m_texture_list)
        {
          const bool is_selected = (sprite.texture == tex);
          if (ImGui::Selectable (tex.c_str(), is_selected))
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
      if (ImGui::BeginCombo ("Render layer", nullptr, 0))
      {
        registry.view<component::LayerT>().each ([&sprite, &new_layer] (auto entity, auto& layer)
        {
          const bool is_selected = (sprite.layer == layer.id);
          if (ImGui::Selectable (layer.id.c_str(), is_selected))
          {
            new_layer = layer.id;
          }

          if (is_selected)
          {
            ImGui::SetItemDefaultFocus();
          }
        });
        ImGui::EndCombo();
      }
      ImGui::PopID();

      if (frame != sprite.frame)
      {
        registry.patch<component::SpriteT> (entity, [&frame, &new_texture] (auto& spr)
        {
          spr.frame       = frame;
          spr.loaded      = false;
        });
      }
      if (new_texture != sprite.texture || new_layer != sprite.layer)
      {
        auto old_frame = sprite.frame;
        auto& spr = registry.replace<component::SpriteT> (entity, new_texture);
        spr.frame = old_frame;
        spr.layer = new_layer;
      }
    }

  private:
    const std::vector<std::string>& m_texture_list;
  };
} // namespace widget
} // namespace stella