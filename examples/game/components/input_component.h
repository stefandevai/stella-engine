#pragma once

struct InputComponent {
	InputComponent(const bool *keys) : Keys(keys) {}
	const bool *Keys;
};

