#pragma once

#include "stella/core/input/input_events.hpp"
#include <initializer_list>
#include <unordered_map>

union SDL_Event;

namespace stella::core
{
class SDLInputMapper
{
public:
  SDLInputMapper();
  void update (SDL_Event& event);
  const bool query (const InputEvent event);
  const bool query (const std::initializer_list<InputEvent> events);

private:
  std::unordered_map<InputEvent, bool> m_input_map;
};

} // namespace stella::core
