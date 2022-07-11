#include "raylib.h"
#include "Boid.h"
#include <random>
#include <iostream>

int randInt(int, int);
std::mt19937 randomness((unsigned)std::time(nullptr));

const int FLOCKSIZE = 25;
Boid flock[FLOCKSIZE];

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Boid Sim");

    SetTargetFPS(60);
    
    

    for (unsigned int i = 0; i < FLOCKSIZE; ++i) {
        flock[i].SetColor(DARKGRAY);
        flock[i].SetPosition({ (float) randInt(0, screenWidth), (float) randInt(0, screenHeight) });
        float randAngle = randInt(0, 360) * (PI / 180);
        flock[i].SetVelocity({ cos(randAngle), sin(randAngle) });
        // flock[i].Debug(1);
    }

    Boid testBoid({ screenWidth / 2, screenHeight / 2 }, DARKGREEN);
    // testBoid.Rotate(90);
    testBoid.Debug(1);


    while (!WindowShouldClose())
    {
        testBoid.Update();
        testBoid.WarpCoordinates(screenWidth, screenHeight);


        BeginDrawing();

        ClearBackground(RAYWHITE);

        // testBoid.Draw();
        for (unsigned int i = 0; i < FLOCKSIZE; ++i) {
            flock[i].Update();
            flock[i].WarpCoordinates(screenWidth, screenHeight);
            flock[i].Draw();
            flock[i].Behaviours(flock, FLOCKSIZE);
        }
        
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
