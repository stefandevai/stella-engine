#pragma once

struct InputComponent {
  InputComponent(const std::array<bool, 1024> &keys) : Keys(keys) {}
  const std::array<bool, 1024> &Keys;
};
