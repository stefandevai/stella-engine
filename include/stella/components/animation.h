#pragma once

#include <string>
#include <tuple>
#include <vector>

#include <glm/glm.hpp>

namespace stella
{
namespace components
{
  struct Animation
  {
    inline Animation (std::vector<std::tuple<std::string, std::vector<unsigned int>, unsigned int>> frames,
                                glm::vec2 frame_dimensions)
      : Frames{std::move (frames)}, FrameDimensions{std::move (frame_dimensions)}
    {
      this->Initialized = false;
    }
    bool Initialized;
    std::vector<std::tuple<std::string, std::vector<unsigned int>, unsigned int>> Frames;
    glm::vec2 FrameDimensions;
    std::string current_animation = std::string();
    std::string old_animation     = std::string();
  };
} // namespace components
} // namespace stella
