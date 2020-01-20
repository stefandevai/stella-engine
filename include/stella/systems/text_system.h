#pragma once

#include "stella/systems/system.h"
#include "stella/components/text_component.h"
#include "stella/components/position_component.h"
#include "stella/components/dimension_component.h"
#include "stella/components/sprite_component.h"
#include "stella/components/color_component.h"
#include "stella/graphics/font.h"

namespace stella
{
namespace systems
{
class TextSystem : public System
{
  private:
    //std::map<std::string, std::shared_ptr<graphics::Font>> m_fonts;
    core::ResourceManager<graphics::Font, const std::string, unsigned> &m_fonts;

  public:
    TextSystem(entt::registry &registry, core::ResourceManager<graphics::Font, const std::string, unsigned> &fonts)
      : m_fonts(fonts)
    {
      //m_fonts.insert(std::pair<std::string, std::shared_ptr<graphics::Font>>("1980", std::make_shared<graphics::Font>("assets/fonts/1980.ttf")));
      registry.on_construct<components::TextComponent>().connect<&TextSystem::initialize_text>(this);
      registry.on_destroy<components::TextComponent>().connect<&TextSystem::delete_text>(this);
    }

    ~TextSystem() override { }

    void update(entt::registry &registry, const double dt) override
    {
    //   registry.group<components::TextComponent>(entt::get<components::PositionComponent, components::DimensionComponent>).each([&registry](auto entity, auto &text, auto &pos, auto &dim)
    //   {
    //     if (!text.IsStatic)
    //     {
      
    //     }
    //   });
    }
  private:
    TextSystem() = delete;
    
    void initialize_text(entt::registry &registry, entt::entity entity, components::TextComponent &text)
    {
      auto pos = registry.get<components::PositionComponent>(entity);
      //auto dim = registry.get<components::DimensionComponent>(entity);
      float scale = 1.f;
      float char_posx = (float)pos.x;
      auto font = m_fonts.load(text.FontName);

      for (wchar_t c : text.Text)
      {
          auto ch = font->get_char_data(c);
          GLfloat xpos = char_posx + ch.bl * scale;
          GLfloat ypos = pos.y - ch.bt * scale;
          GLfloat w = ch.bw * scale;
          GLfloat h = ch.bh * scale;
          
          if (w > 0.f && h > 0.f)
          {
            auto char_entity = registry.create();
            registry.assign<components::SpriteComponent>(char_entity, glm::vec3(xpos, ypos, 0.f), glm::vec2(w, h), glm::vec2(ch.tx, 0.f), *font->get_atlas(), "text");
            registry.assign<components::PositionComponent>(char_entity, xpos, ypos);
            registry.assign<components::DimensionComponent>(char_entity, w, h);
            registry.assign<components::ColorComponent>(char_entity, text.color);
            text.char_entities.push_back(char_entity);
          }

          char_posx += (ch.ax >> 6) * scale;
     }
    }

    void delete_text(entt::registry &registry, entt::entity entity)
    {
      auto& text = registry.get<components::TextComponent>(entity);
      //std::cout << "here\n";
      for (auto &ch : text.char_entities)
      {
        registry.destroy(ch);
      }
    }
};
} // namespace systems
} // namespace stella

