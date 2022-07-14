#include "Boid.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>

my::Vector2f Vector2Limit(my::Vector2f v, int minLength, int maxLength);
Vector2 ToRayVec(my::Vector2f vec);

Boid::Boid() {
	color    = RAYWHITE;
	position = my::Vector2f();

	relPoints[0] = my::Vector2f(0, 10);
	relPoints[1] = my::Vector2f(10, -20);
	relPoints[2] = my::Vector2f(-10, -20);

	relPoints[0] = relPoints[0] * scale;
	relPoints[1] = relPoints[1] * scale;
	relPoints[2] = relPoints[2] * scale;

	points[0] = position - relPoints[0];
	points[1] = position - relPoints[1];
	points[2] = position - relPoints[2];

	velocity = my::Vector2f();
	acceleration = my::Vector2f();

	angle = velocity.AngleTo(my::Vector2f(0, 0));

	Rotate(90 - angle);
}

Boid::Boid(my::Vector2f position, Color color) {
	this->color = color;
	this->position = position;

	relPoints[0] = my::Vector2f(  0,  10);
	relPoints[1] = my::Vector2f( 10, -20);
	relPoints[2] = my::Vector2f(-10, -20);

	relPoints[0] = relPoints[0] * scale;
	relPoints[1] = relPoints[1] * scale;
	relPoints[2] = relPoints[2] * scale;

	points[0] = position - relPoints[0];
	points[1] = position - relPoints[1];
	points[2] = position - relPoints[2];

	velocity = my::Vector2f(0.0f, -1.0f);
	acceleration = my::Vector2f();

	angle = velocity.AngleTo(my::Vector2f(0, 0));

	Rotate(90 - angle);
}

void Boid::Draw() {
	if (debug) {
		Vector2 endVelo = ToRayVec(position + (velocity * 10));
		Vector2 endAcc =  ToRayVec(position + (acceleration * 10));

		DrawLineEx( ToRayVec(position), endVelo, 2, {255,   0,   0, 100});
		DrawLineEx( ToRayVec(position), endAcc , 2, {255, 100,   0, 100});
		DrawCircleV(ToRayVec(position), visionRad,  {255, 160,   0, 50 });
		DrawCircleV(ToRayVec(position), avoidRad,   {  0, 160, 255, 50 });
		DrawCircleV(ToRayVec(position),		   5,   {255, 255,   0, 255 });
	}

	DrawTriangle(ToRayVec(points[0]), ToRayVec(points[1]), ToRayVec(points[2]), color);
}

void Boid::Update() {
	SetPosition(position + velocity);
	velocity = velocity + acceleration;
	velocity = Vector2Limit(velocity, 1, 5);

	float angleNew = velocity.AngleTo(my::Vector2f(0, 0));;
	if (angleNew != angle) {
		Rotate(angle - angleNew);
	}
	angle = angleNew;
}

void Boid::Debug(bool deb) {
	debug = deb;
}

void Boid::Rotate(float angle) {
	for (int i = 0; i < 3; ++i) {

		float radianAngle = angle * (PI / 180);

		float x = relPoints[i].x * cos(radianAngle) - relPoints[i].y * sin(radianAngle);
		float y = relPoints[i].x * sin(radianAngle) + relPoints[i].y * cos(radianAngle);

		relPoints[i] = my::Vector2f(x, y);
		points[i] = position - my::Vector2f( x, y );
	}
}

void Boid::SetPosition(my::Vector2f newPos) {
	position = newPos;

	points[0] = position - relPoints[0];
	points[1] = position - relPoints[1];
	points[2] = position - relPoints[2];
}

void Boid::SetVelocity(my::Vector2f velocity) {
	if (this->velocity != velocity) {
		this->velocity = velocity;
		float angleNew = velocity.AngleTo(my::Vector2f(0, 0));
		Rotate(angle - angleNew);
		angle = angleNew;
	}
}

void Boid::SetColor(Color color) {
	this -> color = color;
}

void Boid::WarpCoordinates(float width, float height) {

	if (position.x < 0.0f)	 SetPosition({ position.x + width,  position.y });
	if (position.x > width)	 SetPosition({ position.x - width,  position.y });
	if (position.y < 0.0f)	 SetPosition({ position.x, position.y + height });
	if (position.y > height) SetPosition({ position.x, position.y - height });
}


void Boid::Behaviours(Boid *flockMates, size_t n) {
	
	acceleration = my::Vector2f();
	float sepWeight = 0.1;
	float alignWeight = 0.1;
	float coheWeight = 0.1;

	int numBoids = 0;
	my::Vector2f flockHeading;
	my::Vector2f flockCenter;
	my::Vector2f avoidanceHeading;

	for (unsigned int i = 0; i < n; ++i) {
		Boid *mate = &flockMates[i];

		if (mate != this) {
			my::Vector2f offset = mate->position - position;
			
			float sqrDst = offset.x * offset.x + offset.y * offset.y;

			if (sqrDst < visionRad * visionRad) {
				numBoids += 1;
				flockHeading += mate->velocity.Normalize();
				flockCenter += mate->position;

				if (sqrDst < avoidRad * avoidRad) {
					avoidanceHeading -= offset * (1 / avoidRad);
				}
			}
		}
	}

	if (numBoids != 0) {
		
		if (numBoids >= 2) {
			int z = 1;
		}
		
		flockCenter = flockCenter * (1.0f / (float) numBoids);

		my::Vector2f offsetToFMCenter = flockCenter - position;

		if (avoidanceHeading.Length() != 0 && offsetToFMCenter.Length() != 0 && flockHeading.Length() != 0) {
			my::Vector2f alignmentForce  = (SteerTowards(flockHeading)     * alignWeight);
			my::Vector2f cohesionForce   = (SteerTowards(offsetToFMCenter) * coheWeight);
			my::Vector2f separationForce = (SteerTowards(avoidanceHeading) * sepWeight);

			// std::cout << ": " << cohesionForce.x << " " << cohesionForce.y << std::endl;

			acceleration += alignmentForce;
			acceleration += cohesionForce;
			acceleration += separationForce;
		}
	}
}

my::Vector2f Vector2Limit(my::Vector2f v, int minLength, int maxLength) {
	float squaredMag = v.LengthSquared();

	if (squaredMag == 0) {
		return my::Vector2f();
	}

	my::Vector2f resultant = v;

	if (maxLength != 0 && squaredMag > (maxLength*maxLength)) {
		resultant = resultant.Normalize() * maxLength;
	}
	if (minLength != 0 && squaredMag < (minLength*minLength)) {
		resultant = resultant.Normalize() * minLength;
	}

	return resultant;
}
Vector2 ToRayVec(my::Vector2f vec) {
	return { vec.x, vec.y };
}
my::Vector2f Boid::SteerTowards(my::Vector2f vector) {
	my::Vector2f v = (vector.Normalize() * 5) - velocity;
	return Vector2Limit(v, 0, 3);
}
