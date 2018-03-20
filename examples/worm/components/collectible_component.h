#pragma once

enum CollectibleType {
	None,
	Seed,
	FireSeed,
	PoisonSeed
};

struct CollectibleComponent {
  inline CollectibleComponent(CollectibleType type) : Type(type) { };
  CollectibleType Type;
};
