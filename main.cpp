#include "raylib.h"
int main ()
{
   /* Creating constants for the screen size
    * I am opting for the size of a full screen
    * as the game should be played on a phone horizontally */
    const int width {1920};
    const int height {1080};
    InitWindow( width, height, "All my flappys");

    // Starting with basic setup, FPS, while loop, begin drawing
    // Code from: https://github.com/naoisecollins/2023MSc-SoftwareEngineering1-Class-Workspace/commit/9fef4e7cde904d2a6832a49adcba3959b9cd7a95
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        
        // Begin Drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // End Drawing
        EndDrawing();


    }
    

    // Close Window and Unload Textures
    CloseWindow();




}