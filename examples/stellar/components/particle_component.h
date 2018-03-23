#pragma once

struct ParticleComponent {
	ParticleComponent(unsigned int max_life, double size, double speedx, double speedy) : MaxLife(max_life), W(size), SpeedX(speedx), SpeedY(speedy) {
		Life = 0;
		Alive = true;

		H = W;
	}

	unsigned int Life, MaxLife;
	double W, H;
	double SpeedX, SpeedY;
	bool Alive;
};

