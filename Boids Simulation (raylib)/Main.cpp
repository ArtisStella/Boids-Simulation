#include "raylib.h"
#include "MVector.h"
#include "Boid.h"
#include <random>
#include <iostream>
#include <chrono>

std::mt19937 randomness((unsigned)std::time(nullptr));

const int screenWidth = 800;
const int screenHeight = 450;

const int FLOCKSIZE = 40;
Boid flock[FLOCKSIZE];

const int TESTSIZE = 3;
Boid testFlock[TESTSIZE];

int randInt(int, int);
void TestBoids();
void RunBoids();

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Boid Sim");

    SetTargetFPS(60);
    
    

    for (unsigned int i = 0; i < FLOCKSIZE; ++i) {
        flock[i].SetColor(DARKGRAY);
        flock[i].SetPosition( my::Vector2f( (float) randInt(0, screenWidth), (float) randInt(0, screenHeight)));
        float randAngle = (float) randInt(0, 360) * (PI / 180);
        my::Vector2f vel = my::Vector2f((float) cos(randAngle), (float) sin(randAngle)) * 2;
        flock[i].SetVelocity(vel);
        // flock[i].Debug(1);
    }

    Boid testBoid1( my::Vector2f(screenWidth / 4 * 3, screenHeight / 2), DARKGREEN);
    Boid testBoid2( my::Vector2f(screenWidth / 4, screenHeight / 2), DARKGREEN);
    Boid testBoid3( my::Vector2f(screenWidth / 2, screenHeight / 4), DARKGREEN);

    // testBoid.Rotate(90);
    testBoid1.SetVelocity(my::Vector2f(-2, 0));
    testBoid2.SetVelocity(my::Vector2f( 2, 0));
    testBoid3.SetVelocity(my::Vector2f( 0, 2));

    testBoid1.Debug(1);
    testBoid2.Debug(1);
    testBoid3.Debug(1);

    testFlock[0] = testBoid1;
    testFlock[1] = testBoid2;
    testFlock[2] = testBoid3;

    

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        auto start = std::chrono::steady_clock::now();

        // TestBoids();
        
        RunBoids();
        

        auto end = std::chrono::steady_clock::now();

        // std::cout   << "Elapsed time in nanoseconds: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns" << std::endl;

        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

int randInt(int lowerBound, int upperBound) {
    std::uniform_int_distribution<int> dist(lowerBound, upperBound);
    return dist(randomness);
}

void TestBoids() {
    for (unsigned int i = 0; i < TESTSIZE; ++i) {
        testFlock[i].Update();
        testFlock[i].WarpCoordinates(screenWidth, screenHeight);
        testFlock[i].Draw();
        testFlock[i].Behaviours(testFlock, TESTSIZE);
    }
}

void RunBoids() {
    for (unsigned int i = 0; i < FLOCKSIZE; ++i) {
        flock[i].Update();
        flock[i].WarpCoordinates(screenWidth, screenHeight);
        flock[i].Draw();
        flock[i].Behaviours(flock, FLOCKSIZE);
    }
}