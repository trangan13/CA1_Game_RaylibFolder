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
    int flappyCount; // Adding this to be able to change the animation on the y axis
};

// This is to check if the character touches the ground - I might add aswell if crosses the sky limit
bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}





AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
// update running time
data.runningTime += deltaTime;
data.rec.y = data.flappyCount * data.rec.height/4; // I think this will let me change the animation when Flappy catches children, still need to test changing flappy count
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
    const int windowWidth {1360}; // Reduce dimensions that match background ration
    const int windowHeight {800};
    InitWindow( windowWidth, windowHeight, "All my flappys");

    // Trees on fire Obstacles variables
    Texture2D tree = LoadTexture("textures/fire.png"); // They should be trees on fire but for now just fire

    const int sizeOfTrees{5};
    // Array of trees / fire
    AnimData trees[sizeOfTrees]{};
 
    // 5 trees will come up with 300 pixels separation
    for (int i = 0; i < sizeOfTrees; i++)
    {
        trees[i].rec.x = 0.0;
        trees[i].rec.y = 0.0;
        trees[i].rec.width = tree.width/4;
        trees[i].rec.height = tree.height/5;
        trees[i].pos.y = windowHeight - 1000;    // tree.height/4; // Trees appear at ground level
        trees[i].frame = 0;
        trees[i].runningTime = 0.0;
        trees[i].updateTime = 1.0/16.0;
        trees[i].pos.x = windowWidth + i * 300;
    }


    // Nebula animation variables
    int treeVel {-200};

 


     // Flappy variables, code from same project as Struct, changed all details from scarfy to flappy. 
    Texture2D flappy = LoadTexture("textures/flappy.png");
    AnimData flappyData;
    flappyData.rec.width = flappy.width/4; // changed this to match the current size of my sprite which is only 4 frames
    flappyData.rec.height = flappy.height/4; // Updated to match the new sprite with the birds
    flappyData.rec.x = 0;
    flappyData.rec.y = 0;
    flappyData.pos.x = windowWidth/3 - flappyData.rec.width/4; // 
    flappyData.pos.y = windowHeight/2 - flappyData.rec.height; // change this for flappy to appear in the middle
    flappyData.frame = 0; 
    flappyData.runningTime = 0.0;
    flappyData.updateTime = 1.0/12.0; // how often a frame is changed

    // Need to setup flap action, jumplike, negative in Y axis
    const int flapVel{-400}; // (Pixels/s)/s    
    // Set initial Velocity
    int velocity {0}; //pixels/frame
     // Gravity
    const int gravity {400}; // (pixels/s)/s - Lowering gravity


    // Background image loading
    Texture2D background = LoadTexture("Textures/parallax-forest-back-trees.png");
    // Creating a float to help the background move with a variable
    float bgX{};
    Texture2D midground = LoadTexture("Textures/parallax-forest-middle-trees.png");
    float mgX{};
    Texture2D foreground = LoadTexture("Textures/parallax-forest-front-trees.png");
    float fgX{};

    // Set this to finish the game when touching the ground or an obstacle
    bool gameOver = false;

    // Starting with basic setup, FPS, while loop, begin drawing
    // Code from: https://github.com/naoisecollins/2023MSc-SoftwareEngineering1-Class-Workspace/commit/9fef4e7cde904d2a6832a49adcba3959b9cd7a95
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        
        // Calling Delta time for future reference
        float dt{GetFrameTime()};


        // Begin Drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // Movement for the background, conditional to bring it back again. 
        bgX -= 20 * dt;
        if (bgX <= -background.width*5)
        {
            bgX = 0;
        }

        mgX -= 50 * dt; // Same for midground
        if (mgX <= -midground.width*5)
        {
            mgX = 0;
        }
        
        fgX -= 100 * dt; // Same for background
        if (fgX <= -foreground.width*5)
        {
            fgX = 0;
        }

        //Draw background, drawtextureex adds a scaling parameter
        // Since the background images are quite small, I am using x5 the size
        // Also using two images so we can loop them
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 5.0, WHITE);
        Vector2 bg2Pos{bgX + background.width*5, 0.0}; // this duplicates the background, *5 because that is the lenght of the scaled texture
        DrawTextureEx(background, bg2Pos, 0.0, 5.0, WHITE);

        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 5.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width*5, 0.0}; // this duplicates the midground
        DrawTextureEx(midground, mg2Pos, 0.0, 5.0, WHITE);

        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 5.0, WHITE);
        Vector2 fg2Pos{fgX + foreground.width*5, 0.0}; // this duplicates the midground
        DrawTextureEx(foreground, fg2Pos, 0.0, 5.0, WHITE);
        





        
        // Gravity action
        velocity += gravity * dt;
    


        // Button Actions - Flap
         if (IsKeyPressed(KEY_SPACE))
        {
            velocity += flapVel;
        }


        // Update the position of Flappy, I see my current code had called in Delta Time before and assigned to dt.
        flappyData.pos.y += velocity * dt;

        // Update position of Trees/Fire
        for (int i = 0; i < sizeOfTrees; i++)
        {
            // Update the position of each tree
            trees[i].pos.x += treeVel * dt;
        }


        // Update Runnning Time - This works as a timer, everytime running time reaches 12th of a second, 
        // we update the frame but also restart the timer eventurally. 
        // Original code stops animation when in the air, I will do the same if space bar is pressed. 
       if(!IsKeyPressed(KEY_SPACE))
       { 
            flappyData = updateAnimData (flappyData, dt, 5);
       }

        // Draw Flappy
        DrawTextureRec(flappy, flappyData.rec, flappyData.pos, WHITE);

        // Draw all trees
        for (int i = 0; i < sizeOfTrees; i++)
        {
           trees[i] = updateAnimData (trees[i], dt, 5); 
           
            // Draw each tree
            DrawTextureRec(tree, trees[i].rec, trees[i].pos, WHITE);
        }

        // Check for game over conditions
        if (isOnGround(flappyData, windowHeight)) {
            gameOver = true;
        }

        if (gameOver) {
    DrawText("Who will save your flappys now", 100, 100, 20, BLACK);
        }




        // End Drawing
        EndDrawing();


    }
    
    //Unload textures
    UnloadTexture(flappy);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    UnloadTexture(tree);

    // Close Window and Unload Textures
    CloseWindow();




}