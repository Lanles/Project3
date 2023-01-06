#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height/1.3;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    // Update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // Update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    // Drawing window
    int windowDimention[2];
    windowDimention[0] = 512;
    windowDimention[1] = 380;
    InitWindow(windowDimention[0], windowDimention[1], "Project 2");

    // Acceleration due to gravity (pixel/s)/s & jump velocity (pixels/s)
    const int gravity = 1500;
    const int jumpVel = -600;
    // X velocety for enemy sprite
    const int enemyVel = -350;

    // Randomley generated amount of enemies (from 8 to 20)
    int randValue = GetRandomValue(8, 20);
    
    // const for the amount of enemies we have
    const int enemyAmount = randValue;

    // Declare sprite location, sprite dimentions and its position
    Texture2D bob = LoadTexture("textures/bob.png");
    AnimData bobData;
    bobData.rec.width = bob.width/8;
    bobData.rec.height = bob.height;
    bobData.rec.x = 0;
    bobData.rec.y = 0;
    bobData.pos.x = windowDimention[0]/2 - bobData.rec.width/2;
    bobData.pos.y = windowDimention[1] - bobData.rec.height/1.3;
    // Animation frame & amount of time before we update the animation frame for player
    bobData.frame = 0;
    bobData.updateTime = 1.0 / 16.0;
    bobData.runningTime = 0.0;

    // Declare sprite location, sprite dimentions and its position (but its the enemy sprite)
    Texture2D enemy = LoadTexture("textures/enemy_sprites.png");

    AnimData enemyae[enemyAmount]{};

    for (int i = 0; i < enemyAmount; i++)
    {
        enemyae[i].rec.x = 0.0; // Rectagle rec
        enemyae[i].rec.y = 0.0; // Rectagle rec
        enemyae[i].rec.width = enemy.width/8; // Rectagle rec
        enemyae[i].rec.height = enemy.height/8; // Rectagle rec 
        enemyae[i].pos.x = windowDimention[0] + (350 * i); // Vector2 pos
        enemyae[i].pos.y = windowDimention[1] - enemyae[i].rec.height; // Vector2 pos
        enemyae[i].frame = 0; // int frame
        enemyae[i].updateTime = 1.0 / 16.0; // float updateTime
        enemyae[i].runningTime = 0.0; // float runningTime
    }

    // Value for reching the end of the game
    float finisLine = {enemyae[enemyAmount - 1].pos.x};

    // Check if the player is in the air
    bool isInAir = false;

    int velocity = 0;

    // Load Background textures
    Texture2D background = LoadTexture("textures/background3.png");
    Texture2D midground = LoadTexture("textures/background2.png");
    Texture2D foreground = LoadTexture("textures/background1.png");
    float bgX = 0.0;
    float mgX = 0.0;
    float fgX = 0.0;

    bool collision = false;

    SetTargetFPS(60);
    // Keeping window open and adding peramiters
    while (!WindowShouldClose())
    {

        // Delta time (this is time since last frame)
        const float dT = GetFrameTime();

        // Start drawing
        BeginDrawing();
        ClearBackground(WHITE);
        
        // bgX, mgX, fgX is used so that the background moves
        bgX -= 20 * dT;
        if (bgX <= -background.width*2)
        {
            bgX = 0.0;
        }
        mgX -= 40 * dT;
        if (mgX <= -midground.width*2)
        {
            mgX = 0.0;
        }
        fgX -= 80 * dT;
        if (fgX <= -foreground.width*2)
        {
            fgX = 0.0;
        }

        // Draw background
        Vector2 bg1Pos = {bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos = {bgX + background.width*2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        // Draw midground
        Vector2 mg1Pos = {mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos = {mgX + midground.width*2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);
        
        // Draw foreground
        Vector2 fg1Pos = {fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos = {fgX + foreground.width*2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);


        // Check if player is on ground (side note if it looks weird divede bobRec.height/1.3 to make him lower //// do in line 25 as well)
        if (isOnGround(bobData, windowDimention[1]))
        {
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // Apply gravity if in air
            velocity += gravity * dT;
            isInAir = true;
        }

        // Check if space is pressed to jump
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }
        
        // Update player position
        bobData.pos.y += velocity * dT;

        // Update enemy position
        for (int i = 0; i < enemyAmount; i++)
        {
            enemyae[i].pos.x += enemyVel * dT;
        }

        // Update finish line position
        finisLine += enemyVel * dT;

        // Check if player is in air and update animatios
        if (!isInAir)
        {
            bobData = updateAnimData(bobData, dT, 7);
        }

        // Update enemy animations
        for (int i = 0; i < enemyAmount; i++)
        {
            enemyae[i] = updateAnimData(enemyae[i], dT, 7);
        }

        // Setup colision and check if enemy is hit
        for (AnimData enemy : enemyae)
        {   
            float pad = 100;
            Rectangle enemyRec{
                enemy.pos.x + pad,
                enemy.pos.y + pad,
                enemy.rec.width - 2 * pad,
                enemy.rec.height - 2 * pad
            };
            Rectangle bobRec{
                bobData.pos.x,
                bobData.pos.y,
                bobData.rec.width,
                bobData.rec.height
            };
            if (CheckCollisionRecs(enemyRec, bobRec))
            {
                collision = true;
            }
        }

        if (collision)
        {
            // Game lost
            DrawText("Game Over!", windowDimention[0] / 4 + 30, windowDimention[1] / 2, 40, BLUE);
        }
        else if (bobData.pos.x >= finisLine)
        {
            // Crossed the finish line
            DrawText("You Win!", windowDimention[0] / 4 + 70, windowDimention[1] / 2, 40, PURPLE);
        }
        else
        {
            // Draw enemy sprite
            for (int i = 0; i < enemyAmount; i++)
            {
                DrawTextureRec(enemy, enemyae[i].rec, enemyae[i].pos, WHITE);
            }

            // Draw player sprite
            DrawTextureRec(bob, bobData.rec, bobData.pos, WHITE);
        }

        // End drawing
        EndDrawing();
    }
    UnloadTexture(bob);
    UnloadTexture(enemy);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}