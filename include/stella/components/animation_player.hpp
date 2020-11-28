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
    AnimationData(const std::vector<unsigned int>& frames = {}, const float step = 0.1f, const bool loop = false) : frames(frames), step(step), loop(loop) {}
    std::vector<unsigned int> frames;
    float step = 0.1;
    bool loop = false;
  };

  struct AnimationPlayer : public Component
  {
    AnimationPlayer() : Component ("AnimationPlayer") {}
    enum State
    {
      STOP,
      PLAY,
      PAUSE
    };
    std::unordered_map<std::string, AnimationData> animations;
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
      animations.emplace(name, data);
    }
  };
} // namespace component
} // namespace stella
