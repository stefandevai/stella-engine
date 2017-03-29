#pragma once

#include <string>
#include <vector>
#include <map>

struct AnimationComponent {
	inline AnimationComponent(std::string initial_animation, const std::map<std::string, std::vector<unsigned int>> &anims) : current_animation(initial_animation), animations(anims) { initialized = false; }
	bool initialized;
	std::string current_animation;
	const std::map<std::string, std::vector<unsigned int>> &animations;
};
