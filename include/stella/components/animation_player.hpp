#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "component.hpp"

namespace stella
{
namespace component
{
  struct AnimationData
  {
    std::vector<unsigned int> frames;
    float step = 0.1;
  };

  struct AnimationPlayer : public Component
  {
    AnimationPlayer() : Component ("Animation Player") {}
    enum State
    {
      STOP,
      PLAY,
      PAUSE
    };
    std::unordered_map<std::string, AnimationData> animations;
    bool loop           = false;
    State state         = STOP;
    State last_state    = STOP;
    std::string current = "";
    std::string last    = "";
    unsigned int index  = 0;
    float elapsed       = 0.0f;

    inline void add (const std::string& name, const AnimationData& data)
    {
      if (current.empty())
      {
        current = name;
        last    = name;
        state   = PLAY;
      }
      animations[name] = data;
    }
  };
} // namespace component
} // namespace stella
