#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace stella
{
namespace component
{
  struct AnimationData
  {
      std::vector<unsigned int> frames;
      float step = 0.1;
  };

  struct AnimationPlayer
  {
    enum State
    {
        STOP,
        PLAY
    };
    std::unordered_map<std::string, AnimationData> animations;
    State state = STOP;
    std::string current = "";
    std::string last = "";
    int index = 0;
    float elapsed = 0.0f;

    inline void add (const std::string& name, const AnimationData& data)
    {
        if (current.empty())
        {
            current = name;
            last = name;
        }
        animations[name] = data;
    }
  };
} // namespace component
} // namespace stella
