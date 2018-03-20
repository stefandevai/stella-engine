#include "../components/collectible_component.h"

struct Collect {
  Collect(CollectibleType type)
      : Type(type) {}
  CollectibleType Type;
};

