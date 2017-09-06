#pragma once

#include <entityx/entityx.h>

class TileviewSystem : public entityx::System<TileviewSystem> {
	public:
		TileviewSystem(const int &boundx);
		~TileviewSystem();
		void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;
	private:
		const int BoundX;
};

