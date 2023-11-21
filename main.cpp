#include "raylib.h"

// Creating a struct to determine animation data for my character sprites. 
// Code from previous project: https://github.com/trangan13/DapperDash_Project/blob/main/dasher.cpp
struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};



AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
// update running time
data.runningTime += deltaTime;
if (data.runningTime >= data.updateTime) // check if running time has reached or exceded updateTime
{
    data.runningTime = 0.0;
    // update animation frame which will be dependant by the width of one character size in the sprite
    data.rec.x = data.frame * data.rec.width;
    data.frame++;
    if (data.frame > maxFrame) // checks it has reached the limit for a loop to restart
    {
        data.frame = 0;
    }
}
return data; // returns the data to be available outside of the function
}








int main ()
{
   /* Creating constants for the screen size
    * I am opting for the size of a full screen
    * as the game should be played on a phone horizontally */
    const int windowWidth {1920};
    const int windowHeight {1080};
    InitWindow( windowWidth, windowHeight, "All my flappys");


     // Flappy variables, code from same project as Struct, changed all details from scarfy to flappy. 
    Texture2D flappy = LoadTexture("textures/flappy.png");
    AnimData flappyData;
    flappyData.rec.width = flappy.width/4; // changed this to match the current size of my sprite which is only 4 frames
    flappyData.rec.height = flappy.height;
    flappyData.rec.x = 0;
    flappyData.rec.y = 0;
    flappyData.pos.x = windowWidth/3 - flappyData.rec.width/4;
    flappyData.pos.y = windowHeight - flappyData.rec.height;
    flappyData.frame = 0; 
    flappyData.runningTime = 0.0;
    flappyData.updateTime = 1.0/12.0; // how often a frame is changed

    // Need to setup flap action, jumplike, negative in Y axis
    const int flapVel{-600}; // (Pixels/s)/s    
    // Set initial Velocity
    int velocity {0}; //pixels/frame
     // Gravity
    const int gravity {1'000}; // (pixels/s)/s


    // Starting with basic setup, FPS, while loop, begin drawing
    // Code from: https://github.com/naoisecollins/2023MSc-SoftwareEngineering1-Class-Workspace/commit/9fef4e7cde904d2a6832a49adcba3959b9cd7a95
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        
        // Calling Delta time for future reference
        float dt{GetFrameTime()};


        // Begin Drawing
        BeginDrawing();
        ClearBackground(WHITE);


        // Gravity action
        velocity += gravity * dt;
    


        // Button Actions - Flap
         if (IsKeyPressed(KEY_SPACE))
        {
            velocity += flapVel;
        }


        // Update the position of Flappy, I see my current code had called in Delta Time before and assigned to dt.
        flappyData.pos.y += velocity * dt;

        // Update Runnning Time - This works as a timer, everytime running time reaches 12th of a second, 
        // we update the frame but also restart the timer eventurally. 
        // Original code stops animation when in the air, I will do the same if space bar is pressed. 
       if(!IsKeyPressed(KEY_SPACE))
       { 
            flappyData = updateAnimData (flappyData, dt, 5);
       }

        // Draw Flappy
        DrawTextureRec(flappy, flappyData.rec, flappyData.pos, WHITE);


        // End Drawing
        EndDrawing();


    }
    
    //Unload textures
    UnloadTexture(flappy);

    // Close Window and Unload Textures
    CloseWindow();




}