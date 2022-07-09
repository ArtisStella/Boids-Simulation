#pragma once
#include "raylib.h"

class Boid
{
public:
	Boid();
	Boid(Vector2, Color);

	void Draw();
	void Update();

	void WarpCoordinates(float width, float height);
	void Rotate(float angle);

	void SetPosition(Vector2 position);
	void SetVelocity(Vector2 velocity);
	void SetColor(Color color);
	void Debug(bool deb);

	void Behaviours(Boid *flockMates, size_t n);


private:
	Vector2 points[3];
	Vector2 relPoints[3];
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	float radiusOfVision = 60.0f;
	float angle;
	Color color;
	bool debug = 0;

	Vector2 Separation(Boid *flockMates, size_t n);
};
