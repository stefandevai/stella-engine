#pragma once

struct BodyComponent
{
  BodyComponent(bool is_static = true) : IsStatic(is_static) {}
  bool IsStatic;
};

