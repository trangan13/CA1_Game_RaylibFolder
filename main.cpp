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


// This is a function for updating animation frames for all objects https://www.udemy.com/course/cpp-fundamentals/learn/lecture/26799168#overview
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

    // Trees on fire Obstacles variables, I have opten for 2 different trees and 1 branch
    Texture2D tree1 = LoadTexture("textures/tree1.png"); // They should be trees on fire but for now just fire
    Texture2D tree2 = LoadTexture("textures/tree2.png");
    Texture2D branch = LoadTexture("textures/branch.png");

    // Setting up the array to 5 for now, will experiment. 
    const int sizeOfTree1{5};
    const int sizeOfTree2{5};
    const int sizeOfBranch{5};
    
    // Array of tree1
    AnimData trees1[sizeOfTree1]{};

    for (int i = 0; i < sizeOfTree1; i++)
    {
        trees1[i].rec.x = 0.0;
        trees1[i].rec.y = 0.0;
        trees1[i].rec.width = tree1.width/4;
        trees1[i].rec.height = tree1.height;
        trees1[i].pos.y = windowHeight - tree1.height; // Trees appear at ground level
        trees1[i].frame = 0;
        trees1[i].runningTime = 0.0;
        trees1[i].updateTime = 1.0/16.0;
        trees1[i].pos.x = windowWidth + i * 2000;
        trees1[i].flappyCount = 0; // null for trees
    }

        // Array of tree2
    AnimData trees2[sizeOfTree2]{};

    for (int i = 0; i < sizeOfTree2; i++)
    {
        trees2[i].rec.x = 0.0;
        trees2[i].rec.y = 0.0;
        trees2[i].rec.width = tree2.width/4;
        trees2[i].rec.height = tree2.height;
        trees2[i].pos.y = windowHeight - tree2.height; // Trees appear at ground level
        trees2[i].frame = 0;
        trees2[i].runningTime = 0.0;
        trees2[i].updateTime = 1.0/16.0;
        trees2[i].pos.x = windowWidth + i * 1000;
        trees2[i].flappyCount = 0; // null for trees
        
    }

            // Array of branches
    AnimData branches[sizeOfBranch]{};

    for (int i = 0; i < sizeOfBranch; i++)
    {
        branches[i].rec.x = 0.0;
        branches[i].rec.y = 0.0;
        branches[i].rec.width = branch.width/4;
        branches[i].rec.height = branch.height;
        branches[i].pos.y = 0; // Branches appear at the top right
        branches[i].frame = 0;
        branches[i].runningTime = 0.0;
        branches[i].updateTime = 1.0/16.0;
        branches[i].pos.x = windowWidth + i * 1500;
        branches[i].flappyCount = 0; // null for trees
        
    }





    // Tree movement animation variables
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
    flappyData.flappyCount = 0; // Will set as 0 for normal, I was getting some errors. 

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

        // Update position of Trees1
        for (int i = 0; i < sizeOfTree1; i++)
        {
            // Update the position of each tree
            trees1[i].pos.x += treeVel * dt;
        }
        // Update position of Trees2
        for (int i = 0; i < sizeOfTree2; i++)
        {
            // Update the position of each tree
            trees2[i].pos.x += treeVel * dt;
        }
        // Update position of Branch
        for (int i = 0; i < sizeOfBranch; i++)
        {
            // Update the position of each tree
            branches[i].pos.x += treeVel * dt;
        }

        // Update Runnning Time - This works as a timer, everytime running time reaches 12th of a second, 
        // we update the frame but also restart the timer eventually. 
        // Original code stops animation when in the air, I will do the same if space bar is pressed. 
       if(!IsKeyPressed(KEY_SPACE))
       { 
            flappyData = updateAnimData (flappyData, dt, 4);
       }

        // Draw Flappy
        DrawTextureRec(flappy, flappyData.rec, flappyData.pos, WHITE);

        // Draw all trees1 - using the same function as Flappy
        for (int i = 0; i < sizeOfTree1; i++)
        {
           trees1[i] = updateAnimData (trees1[i], dt, 4); 
           
            // Draw each tree
            DrawTextureRec(tree1, trees1[i].rec, trees1[i].pos, WHITE);
        }
        // Draw all trees2 
        for (int i = 0; i < sizeOfTree2; i++)
        {
           trees2[i] = updateAnimData (trees2[i], dt, 4); 
           
            // Draw each tree
            DrawTextureRec(tree2, trees2[i].rec, trees2[i].pos, WHITE);
        }
        // Draw all branches
        for (int i = 0; i < sizeOfBranch; i++)
        {
           branches[i] = updateAnimData (branches[i], dt, 4); 
           
            // Draw each branch
            DrawTextureRec(branch, branches[i].rec, branches[i].pos, WHITE);
        }      
        
        
        
        
        
        
        
        // Check for game over conditions
        if (isOnGround(flappyData, windowHeight)) {
            gameOver = true;
        }

        if (gameOver) {
     DrawText("Who will save your flappys now", windowWidth/2-352, windowHeight/2-48,  50, BLACK); // Added a second one for visibility
     DrawText("Who will save your flappys now", windowWidth/2-350, windowHeight/2-50,  50, RED);
        }




        // End Drawing
        EndDrawing();


    }
    
    //Unload textures
    UnloadTexture(flappy);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    UnloadTexture(tree1);
    UnloadTexture(tree2);
    UnloadTexture(branch);

    // Close Window and Unload Textures
    CloseWindow();




}