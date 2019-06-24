#pragma once

struct ParticleComponent {
	ParticleComponent(unsigned int max_life, double size, double speedx, double speedy, int id = -1) : MaxLife(max_life), W(size), SpeedX(speedx), SpeedY(speedy), ID(id) {
		Life = 0;
		Alive = true;

		H = W;
	}

	unsigned int Life, MaxLife;
	double W, H, SpeedX, SpeedY;
	int ID;
	bool Alive;
};

