# Boids-Simulation
My crude implimentation of the Boids Algorithm, written in C++ using raylib.

## Overview
Simulating flocking behaviour seen in birds or fish is surprising only based on 3 simple rules. <br>
Each boid has a position, velocity, acceleration and a radius in which it can see nearby boids.
### 1. Separation
When this rule is applied, boids try not to crash into one another.
### 2. Cohesion
When this rule is applied, boids steer towards the center of mass of all nearby boids. 
### 3. Alignment
The rule helps the boids stay aligned with one another, i.e. match velocities (speed and direction) of nearby boids.

Inspired by [Sebastian Lague's video](https://youtu.be/bqtqltqcQhw "Coding Adventure: Boids") on this algorithm.

## Screenshot
![image](https://user-images.githubusercontent.com/75276178/178956353-09de228a-b5dd-43f4-b239-5d2ba2743cfa.png)
