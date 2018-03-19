#pragma once

#include <string>

struct AnimationComponent {
  inline AnimationComponent(std::string initial_animation)
      : current_animation(initial_animation) {
    initialized = false;
  }
  bool initialized;
  std::string current_animation;
};
