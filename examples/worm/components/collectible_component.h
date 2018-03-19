#pragma once

struct CollectibleComponent {
  inline CollectibleComponent(unsigned int type, float effect)
      : Type(type), Effect(effect){};
  unsigned int Type;
  float Effect;
};
