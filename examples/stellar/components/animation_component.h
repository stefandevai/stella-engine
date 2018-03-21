#pragma once

#include <string>
#include <vector>
#include <tuple>

struct AnimationsComponent {
  inline AnimationsComponent(std::vector<std::tuple<std::string, std::vector<unsigned int>, unsigned int>> frames)
      : Frames{std::move(frames)} {
     this->Initialized = false;
  }
	bool Initialized;
	std::vector<std::tuple<std::string, std::vector<unsigned int>, unsigned int>> Frames;
};
