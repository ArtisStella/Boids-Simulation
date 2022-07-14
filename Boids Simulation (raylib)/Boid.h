#pragma once
#include "MVector.h"
#include "raylib.h"

class Boid
{
public:
	Boid();
	Boid(my::Vector2f, Color);

	void Draw();
	void Update();

	void WarpCoordinates(float width, float height);
	void Rotate(float angle);

	void SetPosition(my::Vector2f position);
	void SetVelocity(my::Vector2f velocity);
	void SetColor(Color color);
	void Debug(bool deb);

	void Behaviours(Boid *flockMates, size_t n);


private:
	my::Vector2f points[3];
	my::Vector2f relPoints[3];
	my::Vector2f position;
	my::Vector2f velocity;
	my::Vector2f acceleration;
	float visionRad = 60.0f;
	float avoidRad = 25.0f;
	float angle;
	float scale = 0.5;
	Color color;
	bool debug = 0;

	my::Vector2f SteerTowards(my::Vector2f vector);
};
