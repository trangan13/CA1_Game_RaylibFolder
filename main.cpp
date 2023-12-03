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

// This is to check if the character touches the ground 
bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

// Creating another function to check when flappies are off screen (lost)
bool isLost(AnimData data, int windowWidth)
{
    return data.pos.x <= windowWidth*0;
}

// Moving here as they were too far low
// Set this to finish the game when touching the ground or an obstacle
bool gameOver = false;
// winning the game
bool gameWon = false;


// This is a function for updating animation frames for all objects https://www.udemy.com/course/cpp-fundamentals/learn/lecture/26799168#overview
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
    data.rec.y = data.flappyCount * data.rec.height; // I think this will let me change the animation when Flappy catches children, still need to test changing flappy count

}
return data; // returns the data to be available outside of the function
}


// Define Timer https://github.com/raysan5/raylib/wiki/Frequently-Asked-Questions#how-do-i-make-a-timer 
// https://youtube.com/watch?v=vGlvTWUctTQ 
// define a timer: other timer: https://github.com/raylib-extras/examples-c/discussions/2
typedef struct
{
    float Lifetime;
}Timer;

// start or restart a timer with a specific lifetime
void StartTimer(Timer* timer, float lifetime)
{
    if (timer != nullptr)
        timer->Lifetime = lifetime;
}

void UpdateTimer(Timer* timer)
{
    // subtract this frame from the timer if it's not allready expired
    if (timer != nullptr && timer->Lifetime > 0)
        timer->Lifetime -= GetFrameTime();
}

// check if a timer is done.
bool TimerDone(Timer* timer)
{
    if (timer != nullptr)
        return timer->Lifetime <= 0;

	return false;
}





int main ()
{
   // Creating constants for the screen size // changing out of constants so I can clear them out when winning
    const int windowWidth {1360}; // Reduce dimensions that match background ratio
    const int windowHeight {800};
    InitWindow( windowWidth, windowHeight, "All my flappys");

    // Custom Font: https://github.com/naoisecollins/2023MSc-SoftwareEngineering1-Class-Workspace/commit/1518ca81d2727b80735651599ee26f5905ebc22d
    Font customFont = LoadFont("fonts/blueStyle.otf");

    // Audio / For background music and sprite effects: https://github.com/naoisecollins/2023MSc-SoftwareEngineering1-Class-Workspace/commit/14e8669b1f2ca643c209f1ca952611b726b73f81
    InitAudioDevice();

    // Loading all sounds
    Music musicTense = LoadMusicStream("Sounds/Audiio_NicolasGurrero_OrchestralWorks_TheAntechamber.wav"); // Changing them to music stream isntead of audio
    Music musicRelief = LoadMusicStream("Sounds/Audiio_Ghosthood_SideEffects_SideEffects_Inst.wav"); 
    Sound soundFlap = LoadSound("sounds/flap.wav");
    Sound soundMama = LoadSound("sounds/mama.wav");

    // Seems I need to play music outside the while loop https://www.raylib.com/examples/audio/loader.html?name=audio_music_stream
    PlayMusicStream(musicTense);


    // Sound settings
    int volumeSounds = 40;
    int volumeMusic = 30;
    SetMusicVolume(musicTense, volumeMusic/100.0f);
    SetMusicVolume(musicRelief, volumeMusic/100.0f); 
    SetSoundVolume(soundFlap, volumeSounds/100.0f);
    SetSoundVolume(soundMama, volumeSounds/100.0f);


    // Trees on fire Obstacles variables, I have opten for 2 different trees and 1 branch
    Texture2D tree1 = LoadTexture("textures/tree1.png"); // They should be trees on fire but for now just fire
    Texture2D tree2 = LoadTexture("textures/tree2.png");
    Texture2D branch = LoadTexture("textures/branch.png");

    // Setting up the array to 5 for now, will experiment. 
    const int sizeOfTree1{8};
    const int sizeOfTree2{4};
    const int sizeOfBranch{5};
    
    // Array of tree1
    AnimData trees1[sizeOfTree1]{};

    // Random value did not work very well for the logic, as multiplying by i makes distances longer everytime, I need to generate a value and add it to a variable
    // https://community.gamedev.tv/t/can-i-improve-dapper-dasher-how/206459/6 
    int trees1Total {0};

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

        // I will add to trees total 
        trees1Total += GetRandomValue(500,3000);
        trees1[i].pos.x = windowWidth + trees1Total; // https://www.raylib.com/examples/core/loader.html?name=core_random_values
        trees1[i].flappyCount = 0; // null for trees
    }

        // Array of tree2
    AnimData trees2[sizeOfTree2]{};
    int trees2Total {0};
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

        trees2Total += GetRandomValue(3000,5000);
        trees2[i].pos.x = windowWidth + trees2Total;
        trees2[i].flappyCount = 0; // null for trees
        
    }

            // Array of branches
    AnimData branches[sizeOfBranch]{};
    int branchTotal {0};
    for (int i = 0; i < sizeOfBranch; i++)
    {
        branches[i].rec.x = 0.0;
        branches[i].rec.y = 0.0;
        branches[i].rec.width = branch.width/4;
        branches[i].rec.height = branch.height;
        branches[i].pos.y = 0; // Branches appear at the top right // 
        branches[i].frame = 0;
        branches[i].runningTime = 0.0;
        branches[i].updateTime = 1.0/16.0;

        branchTotal += GetRandomValue(1000,3500);
        branches[i].pos.x = windowWidth + branchTotal;
        branches[i].flappyCount = 0; // null for trees
        
    }
    // Tree movement animation variables
    int treeVel {-200};

    // FlappyBabies animation
    Texture2D flappyB = LoadTexture("textures/FlappyB.png"); 
    Texture2D flappyG = LoadTexture("textures/FlappyG.png");
    Texture2D flappyY = LoadTexture("textures/FlappyY.png");
     // Copied the same elements as the main flappy, only correcting the height
    int flappiesTotal {0}; // Using the same strategy as the trees to randomize
    // Flappy G
    AnimData flappyGData;
    flappyGData.rec.width = flappyG.width/4; 
    flappyGData.rec.height = flappyG.height; // Updated to all small bird sprites
    flappyGData.rec.x = 0;
    flappyGData.rec.y = 0;

    flappiesTotal += GetRandomValue(1000,3000); // In my sprite green goes first
    flappyGData.pos.x = windowWidth + flappiesTotal;
    flappyGData.pos.y = windowHeight/2 - flappyGData.rec.height; 
    flappyGData.frame = 0; 
    flappyGData.runningTime = 0.0;
    flappyGData.updateTime = 1.0/24.0; // faster flapping than mom
    flappyGData.flappyCount = 0; // 
     
    AnimData flappyBData;
    flappyBData.rec.width = flappyB.width/4; 
    flappyBData.rec.height = flappyB.height; // Updated to all small bird sprites
    flappyBData.rec.x = 0;
    flappyBData.rec.y = 0;
    flappiesTotal += GetRandomValue(1500,2500);
    flappyBData.pos.x = windowWidth + flappiesTotal; // So they start outside the screen a bit randomly
    flappyBData.pos.y = windowHeight/2 - flappyBData.rec.height; 
    flappyBData.frame = 0; 
    flappyBData.runningTime = 0.0;
    flappyBData.updateTime = 1.0/24.0; // faster flapping than mom
    flappyBData.flappyCount = 0; // 
   
    // Flappy Y
     AnimData flappyYData;
    flappyYData.rec.width = flappyY.width/4; 
    flappyYData.rec.height = flappyY.height; // Updated to all small bird sprites
    flappyYData.rec.x = 0;
    flappyYData.rec.y = 0;
    
    flappiesTotal += GetRandomValue(4000,6000);
    flappyYData.pos.x = windowWidth + flappiesTotal; 
    flappyYData.pos.y = windowHeight/2 - flappyYData.rec.height; 
    flappyYData.frame = 0; 
    flappyYData.runningTime = 0.0;
    flappyYData.updateTime = 1.0/24.0; // faster flapping than mom
    flappyYData.flappyCount = 0; // 
    
    
    // Flappies movement animation
    int flappiesVel {-150};



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
    flappyData.flappyCount = 0; // Will set as 0 for normal, I was getting some errors. 
    // I want flappy to flap much quicker when falling(gameover)
    if(!gameOver){
        flappyData.updateTime = 1.0/12.0;
    } else {
        flappyData.updateTime = 1.0/48.0;
    }


    // Need to setup flap movement
    int flapVel{-600}; // (Pixels/s)/s    
    // Set initial Velocity
    int velocity {0}; //pixels/frame
     // Gravity
    int gravity {800}; // (pixels/s)/s


    // Background image loading
    Texture2D background = LoadTexture("Textures/parallax-forest-back-trees.png");
    // Creating a float to help the background move with a variable
    float bgX{};
    Texture2D midground = LoadTexture("Textures/parallax-forest-middle-trees.png");
    float mgX{};
    Texture2D foreground = LoadTexture("Textures/parallax-forest-front-trees.png");
    float fgX{};

    // Setting up collision booleans for interaction with obstacles and flappies // Moving them outside the while loop
    bool collisionTree{};
    bool collisionFlappyB{};
    bool collisionFlappyG{};
    bool collisionFlappyY{};

    // Starting with basic setup, FPS, while loop, begin drawing
    // Code from: https://github.com/naoisecollins/2023MSc-SoftwareEngineering1-Class-Workspace/commit/9fef4e7cde904d2a6832a49adcba3959b9cd7a95
    SetTargetFPS(60);

    // My timer for loading
    float loadLife = 10.0f;
    float messageLife = 7.0f; // making this longer
    Timer loadTimer = {0};
    Timer messageTimer = {0};
    Timer finishTimer = {0};
    StartTimer(&loadTimer, loadLife); // timer should start outside the loop
    StartTimer (&messageTimer, messageLife);

    
    // This condition loop was not working when checking for the music to be ready, used the timer only instead
    while(!WindowShouldClose() && !TimerDone(&loadTimer)){ // Ryan Bissett helped me with this condition through the Discord Channel for class

        UpdateMusicStream(musicTense); 
        // Begin Drawing
        BeginDrawing();
        ClearBackground(WHITE);
        UpdateTimer (&loadTimer);
        UpdateTimer(&messageTimer);
        DrawTextureEx(background, (Vector2){0, 0}, 0.0, 5.0, WHITE);

        if (!TimerDone(&messageTimer)){
        DrawTextEx(customFont, "The forest is on FIRE!", (Vector2){windowWidth/2-474, windowHeight/2-48}, 100, 3, BLACK); 
        DrawTextEx(customFont, "The forest is on FIRE!", (Vector2){windowWidth/2-470, windowHeight/2-50},  100, 3, RED);
        } else {
            DrawTextEx(customFont, "SAVE YOUR FLAPPYS!", (Vector2){windowWidth/2-494, windowHeight/2-48}, 120, 4, BLACK); 
            DrawTextEx(customFont, "SAVE YOUR FLAPPYS!", (Vector2){windowWidth/2-490, windowHeight/2-50},  120, 4, RED);
            
        }

        EndDrawing();
        
    } 
      while (!WindowShouldClose()){
        
        // Calling Delta time for future reference
        float dt{GetFrameTime()};

        UpdateMusicStream(musicTense);   // Update music buffer with new stream data
        UpdateMusicStream(musicRelief);

    

        // Begin Drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // Movement for the background, conditional to bring it back again. 
        bgX -= 50 * dt;
        if (bgX <= -background.width*5)
        {
            bgX = 0;
        }

        mgX -= 100 * dt; // Same for midground
        if (mgX <= -midground.width*5)
        {
            mgX = 0;
        }
        
        fgX -= 160 * dt; // Same for background
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
        // I am changing the conditional for game over here for flappy to fall instead of dissapearing
        if (!gameOver){        
            if (IsKeyPressed(KEY_SPACE))
            {
                velocity += flapVel;
                PlaySound(soundFlap); // Adding sound effect to the flapp
            }
         }

        // Update the position of Flappy, I see my current code had called in Delta Time before and assigned to dt.
        flappyData.pos.y += velocity * dt;

        // Update position of flappies
        flappyBData.pos.x += flappiesVel * dt;
        flappyGData.pos.x += flappiesVel * dt;
        flappyYData.pos.x += flappiesVel * dt;

        

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
        flappyBData = updateAnimData(flappyBData, dt, 4);
        flappyGData = updateAnimData(flappyGData, dt, 4);
        flappyYData = updateAnimData(flappyYData, dt, 4);

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
        
        
        
        for (AnimData tree1 : trees1) // Collision with trees1
        {
            float pad{70}; // Adding a pad to reduce the area of collision in the corners. 
            Rectangle tree1Rec{ // We need to locate the tree on the screen, not on the sprite
                tree1.pos.x + pad, 
                tree1.pos.y + pad,
                tree1.rec.width - 2*pad,
                tree1.rec.height - 2*pad 
            };
            Rectangle flappyRec {
                flappyData.pos.x,
                flappyData.pos.y,
                flappyData.rec.width,
                flappyData.rec.height

            };
            if (CheckCollisionRecs(tree1Rec, flappyRec))
            {
                collisionTree = true;
            }
        }

        for (AnimData tree2 : trees2) // Collision with trees2
        {
            float pad2{150}; // Adding a pad to reduce the area of collision in the corners. 
            Rectangle tree2Rec{ // We need to locate the tree on the screen, not on the sprite
                tree2.pos.x + pad2, 
                tree2.pos.y + pad2,
                tree2.rec.width - 2*pad2,
                tree2.rec.height - 2*pad2 
            };
            Rectangle flappyRec {
                flappyData.pos.x,
                flappyData.pos.y,
                flappyData.rec.width,
                flappyData.rec.height

            };
            if (CheckCollisionRecs(tree2Rec, flappyRec))
            {
                collisionTree = true;
            }
        }

        for (AnimData branch : branches) // Collision with branches
        {
            float pad3{80}; // Adding a pad to reduce the area of collision in the corners. 
            Rectangle tree2Rec{ // We need to locate the tree on the screen, not on the sprite
                branch.pos.x + pad3, 
                branch.pos.y + pad3,
                branch.rec.width - 2*pad3,
                branch.rec.height - 2*pad3 
            };
            Rectangle flappyRec {
                flappyData.pos.x,
                flappyData.pos.y,
                flappyData.rec.width,
                flappyData.rec.height
            };
            if (CheckCollisionRecs(tree2Rec, flappyRec))
            {
                collisionTree = true;
            }
        }

        
        // Collision with Flappies, since they are not arrays I think I can just create the rectangles to check for collision
        Rectangle flappyRec = {flappyData.pos.x, flappyData.pos.y, flappyData.rec.width, flappyData.rec.height};
        Rectangle flappyBRec = {flappyBData.pos.x, flappyBData.pos.y, flappyBData.rec.width, flappyBData.rec.height};
        Rectangle flappyGRec = {flappyGData.pos.x, flappyGData.pos.y, flappyGData.rec.width, flappyGData.rec.height};
        Rectangle flappyYRec = {flappyYData.pos.x, flappyYData.pos.y, flappyYData.rec.width, flappyYData.rec.height};

        // Added another conditional as the changes were being looped and going crazy
        if (!collisionFlappyB && CheckCollisionRecs(flappyRec, flappyBRec))
            {
                collisionFlappyB = true;
                PlaySound(soundMama); // Adding a mama cry when saved
                flappyData.flappyCount = 2; // Updating the Y position to change the sprite and show a child
                flapVel = flapVel - flapVel*.3; // I will reduce a percentage 10% // changed to 30% to feel the weight
                treeVel = treeVel + treeVel *.2; // increase velocity of obstacles
                flappiesVel = flappiesVel + flappiesVel*.1; // increase volocity of next flappy
            }
        if (!collisionFlappyG && CheckCollisionRecs(flappyRec, flappyGRec))
            {
                collisionFlappyG = true;
                PlaySound(soundMama); // Adding a mama cry when saved
                flappyData.flappyCount = 1; // Correcting again with the position of my sprite
                 flapVel = flapVel - flapVel*.2; // I will reduce a percentage 10%
                treeVel = treeVel + treeVel *.3; // increase velocity of obstacles
                flappiesVel = flappiesVel + flappiesVel*.2; // increase volocity of next flappy
            }
        if (!collisionFlappyY && CheckCollisionRecs(flappyRec, flappyYRec))
            {
                collisionFlappyY = true;
                PlaySound(soundMama); // Adding a mama cry when saved
                flappyData.flappyCount = 3;
                flapVel = flapVel - flapVel*.2; // I will reduce a percentage 10%
                treeVel = treeVel + treeVel *.3; // increase velocity of obstacles
            }


        // Draw Flappies // Adding a condition to collision so they dissapear
         
        if(!collisionFlappyG){
            DrawTextureRec(flappyG, flappyGData.rec, flappyGData.pos, WHITE);
        }
        if(!collisionFlappyB){
            DrawTextureRec(flappyB, flappyBData.rec, flappyBData.pos, WHITE);
        }
        if(!collisionFlappyY){
            DrawTextureRec(flappyY, flappyYData.rec, flappyYData.pos, WHITE);
        }
        
        // Starting winning condition from third flappy collision. Start time for final countdown
        if(collisionFlappyY && finishTimer.Lifetime == 0 && !gameOver){
            StartTimer(&finishTimer, 10.0f);
        }
        if (collisionFlappyY && finishTimer.Lifetime > 0) {
            UpdateTimer(&finishTimer);
        }
            // After 5 seconds show message // added third condition so it dissapears when the second message is shown
        if (collisionFlappyY && finishTimer.Lifetime <= 5 && !TimerDone(&finishTimer)) {
            DrawTextEx(customFont, "You are almost there", (Vector2){windowWidth/2.0f-354, windowHeight/1.5f}, 60, 3, BLACK); // making the floats to prevent a vector error
            DrawTextEx(customFont, "You are almost there", (Vector2){windowWidth/2.0f-350, windowHeight/1.5f-2},  60, 3, ORANGE);          
        }

        if (collisionFlappyY && TimerDone(&finishTimer)) // winning condition
        {
            gameWon = true;
            PauseMusicStream(musicTense);
            PlayMusicStream(musicRelief); // change music
            velocity = 0; // Setup a nice flight no more button action
            gravity = 0; // no more falling
            treeVel = -1000; // clearing obstacles
            DrawTextEx(customFont, "You saved your flappys!", (Vector2){windowWidth/2.0f-354, windowHeight/1.5f}, 80, 3, BLACK); // making the floats to prevent a vector error
            DrawTextEx(customFont, "You saved your flappys!", (Vector2){windowWidth/2.0f-350, windowHeight/1.5f-2},  80, 3, YELLOW);          

        }
        



        // Wrapping this up so we cannot lose once the game is over (in case there are an lingering trees)
        if(!gameWon){
            // Check for game over conditions - Added losing a Flappy and unified any tree collision
            if (isOnGround(flappyData, windowHeight) or collisionTree) {
                gameOver = true;
            } else if (!collisionFlappyB and isLost(flappyBData, windowWidth)) // Updated the conditions only to check if we don't rescue them first
            {
                gameOver = true;
            } else if (!collisionFlappyG and isLost(flappyGData, windowWidth))
            {
                gameOver = true;
            } else if (!collisionFlappyY and isLost(flappyYData, windowWidth))
            {
                gameOver = true;
            }
        }

        // Changed to DrawTextEx for custom font
        if (gameOver) {
        flapVel = 300; // falling hard
     DrawTextEx(customFont, "Who will save your flappys now?", (Vector2){windowWidth/2-353, windowHeight/2-49}, 55, 3, BLACK); // Added a second one for visibility
     DrawTextEx(customFont, "Who will save your flappys now?", (Vector2){windowWidth/2-350, windowHeight/2-50},  55, 3, RED);
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
    
    UnloadFont(customFont);

    UnloadMusicStream(musicRelief);
    UnloadMusicStream(musicTense); 
    UnloadSound(soundFlap);
    UnloadSound(soundMama);

    CloseAudioDevice();

    // Close Window and Unload Textures
    CloseWindow();




}