#include "Boid.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>

Vector2 Vector2Limit(Vector2 v, int maxLength);

Boid::Boid() {
	color    = RAYWHITE;
	position = Vector2Zero();

	relPoints[0] = {   0,  10 };
	relPoints[1] = {  10, -20 };
	relPoints[2] = { -10, -20 };

	relPoints[0] = Vector2Scale(relPoints[0], scale);
	relPoints[1] = Vector2Scale(relPoints[1], scale);
	relPoints[2] = Vector2Scale(relPoints[2], scale);

	points[0] = Vector2Subtract(position, relPoints[0]);
	points[1] = Vector2Subtract(position, relPoints[1]);
	points[2] = Vector2Subtract(position, relPoints[2]);

	velocity = Vector2Zero();
	acceleration = Vector2Zero();

	angle = Vector2Angle(velocity, Vector2Zero());
	Rotate(angle - 90);
}

Boid::Boid(Vector2 position, Color color) {
	this->color = color;
	this->position = position;

	relPoints[0] = {   0,  10 };
	relPoints[1] = {  10, -20 };
	relPoints[2] = { -10, -20 };

	points[0] = Vector2Subtract( position, relPoints[0]);
	points[1] = Vector2Subtract( position, relPoints[1]);
	points[2] = Vector2Subtract( position, relPoints[2]);

	velocity = { 0.0f, -1.0f };
	acceleration = Vector2Zero();

	angle = Vector2Angle(velocity, Vector2Zero());
	Rotate(angle - 90);
}

void Boid::Draw() {
	if (debug) {
		Vector2 scaledVelo = Vector2Scale(velocity, 50);
		Vector2 scaledAccel = Vector2Scale(acceleration, 500);

		DrawLineEx(Vector2Add(position, scaledVelo), position, 2, {255, 0, 0, 100});
		DrawLineEx(Vector2Add(position, scaledAccel), position, 2, {255, 100, 0, 100});
		DrawCircleV(position, radiusOfVision, { 255, 160, 0, 100 });
	}

	DrawTriangle(points[0], points[1], points[2], color);
}

void Boid::Update() {
	SetPosition(Vector2Add(position, velocity));
	velocity = Vector2Add(velocity, acceleration);

	if (Vector2Length(velocity) < 0.75) {
		velocity = Vector2Scale(velocity, 2);
	}

	float angleNew = Vector2Angle(velocity, { 0, 0 });
	if (angleNew != angle) {
		Rotate(angleNew - angle);
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

		relPoints[i] = { x, y };
		points[i] = Vector2Subtract(position, { x, y });
	}
}

void Boid::SetPosition(Vector2 newPos) {
	position = newPos;

	points[0] = Vector2Subtract(position, relPoints[0]);
	points[1] = Vector2Subtract(position, relPoints[1]);
	points[2] = Vector2Subtract(position, relPoints[2]);
}

void Boid::SetVelocity(Vector2 velocity) {
	if (this->velocity.x != velocity.x && this->velocity.y != velocity.y) {
		this->velocity = velocity;
		float angleNew = Vector2Angle(velocity, { 0, 0 });
		Rotate(angleNew - angle);
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
	
	acceleration = Vector2Zero();

	Vector2 sep = Separation(flockMates, n);
	sep = Vector2Scale(sep, 0.1);
	acceleration = Vector2Add(acceleration, sep);

	Vector2 align = Alignment(flockMates, n);
	align = Vector2Scale(align, 0.1);
	acceleration = Vector2Add(acceleration, align);
}


Vector2 Boid::Separation(Boid *flockMates, size_t n) {
	int total = 0;

	Vector2 steering = Vector2Zero();

	for (unsigned int i = 0; i < n; ++i) {
		float dist = Vector2Distance(position, flockMates[i].position);

		if (&flockMates[i] != this && dist < radiusOfVision) {
			Vector2 temp = Vector2Subtract(position, flockMates[i].position);
			temp = Vector2Scale(temp, 1 / (dist * dist));
			steering = Vector2Add(steering, temp);
			total += 1;
		}

		if (total > 0) {
			steering = Vector2Scale(steering , 1 / total);
			steering = Vector2Normalize(steering);
			steering = Vector2Scale(steering, 5);
			steering = Vector2Subtract(steering, velocity);
			steering = Vector2Limit(steering, 1);
		}
	}

	
	return steering;
}

Vector2 Boid::Alignment(Boid* flockMates, size_t n) {
	int total = 0;

	Vector2 steering = Vector2Zero();

	for (unsigned int i = 0; i < n; ++i) {
		float dist = Vector2Distance(position, flockMates[i].position);

		if (&flockMates[i] != this && dist < radiusOfVision) {
			Vector2 temp = flockMates[i].velocity;
			temp = Vector2Scale(temp, 1 / (dist * dist));
			steering = Vector2Add(steering, temp);
			total += 1;
		}

		if (total > 0) {
			steering = Vector2Scale(steering, 1 / total);
			steering = Vector2Normalize(steering);
			steering = Vector2Scale(steering, 5);
			// steering = Vector2Subtract(steering, velocity);
			steering = Vector2Limit(steering, 1);
		}
	}


	return steering;
}

Vector2 Vector2Limit(Vector2 v, int maxLength) {
	float squared_mag = pow(Vector2Length(v), 2);
	Vector2 resultant = v;

	if (squared_mag > (maxLength * maxLength)) {
		resultant.x = resultant.x / sqrt(squared_mag);
		resultant.y = resultant.y / sqrt(squared_mag);
		resultant.x = resultant.x * maxLength;
		resultant.y = resultant.y * maxLength;
	}

	return resultant;
}