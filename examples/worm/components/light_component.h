#pragma once

struct LightComponent {
	inline LightComponent(unsigned int type, float intensity) : Type(type), Intensity(intensity) {};
	unsigned int Type;
	float Intensity;
};

