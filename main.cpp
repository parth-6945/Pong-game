#include <iostream>
#include <raylib.h>
#include <chrono>
#include <thread>

using namespace std;

const int screenWidth = 1280;
const int screenHeight = 800;
int playerScore{};
int cpuScore{};
bool GameStart = false;
bool ShowIntro = true;
bool ShowPresent = true;

bool ShowMenu = true;
int selectedOption = 0;
const char* difficultyOptions[] = { "Easy", "Medium", "Hard" };
const int totalOptions = 3;


Color Green = Color{30, 148, 123, 255};
Color DarkGreen = Color{16, 128, 106, 255};
Color LightGreen = Color{103, 163, 147, 255};
Color Yellow = Color{243, 213, 91, 255};

class Paddle{
public:
    float x, y;
    float width, height;
    float speed;

    void Draw(){
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update(){
        if (IsKeyDown(KEY_UP) && y > 0) y -= speed;
        else if (IsKeyDown(KEY_DOWN) && y + height < screenHeight) y += speed;
    }
    void Reset(){
        x = screenWidth - width - 10;
        y = (screenHeight - height) / 2;
    }
};

Paddle player;

class Ball{
public: 
    float x, y;
    float speedX, speedY;
    int radius;

    void Draw(){
        DrawCircle(x, y, radius, Yellow);
    }
    Ball(float x1, float y1, int speedx1, int speedy1, int rad)
        : x(x1), y(y1), speedX(speedx1), speedY(speedy1), radius(rad) {}

    void Update(){
        if (x + radius < screenWidth && x - radius > 0) x += speedX;
        if (x + radius >= screenWidth){
            cpuScore++;
            ResetBall();
            player.Reset();
        }
        if (x - radius <= 0){
            playerScore++;
            ResetBall();
            player.Reset();
        }
        if (y + radius < screenHeight && y - radius > 0) y += speedY;
        else{
            speedY = -speedY;
            y += speedY;
        }
    }

    void ResetBall(){
        x = screenWidth / 2;
        y = screenHeight / 2;
        int speedChoice[2] = {-1, 1};
        speedX *= speedChoice[GetRandomValue(0, 1)];
        speedY *= speedChoice[GetRandomValue(0, 1)];
        Draw();
        this_thread::sleep_for(chrono::milliseconds(700));
    }
};


class CPUpaddle : public Paddle{
public:
    void Update(int ballY){
        if ((y + height / 2 > ballY) && y > 0) y -= speed;
        else if ((y + height / 2 < ballY) && y + height < screenHeight) y += speed;
    }
};

Ball ball(screenWidth / 2, screenHeight / 2, 13, 13, 20);
CPUpaddle cpu;

void DrawCountdown(int countdown) {
    ClearBackground(DarkGreen);
    DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, LightGreen);
    DrawText("Get Ready!", screenWidth / 2 - MeasureText("Get Ready!", 80) / 2, screenHeight / 2 - 100, 80, WHITE);
    DrawText(TextFormat("%i", countdown), screenWidth / 2 - MeasureText(TextFormat("%i", countdown), 100) / 2, screenHeight / 2, 100, WHITE);
}

void DrawIntroScreen() {
    ClearBackground(DarkGreen);
    DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, LightGreen);
    DrawText("Pong Game", screenWidth / 2 - MeasureText("Pong Game", 80) / 2, screenHeight / 2 - 100, 80, WHITE);
    DrawText("Press Enter to Start", screenWidth / 2 - MeasureText("Press Enter to Start", 40) / 2, screenHeight / 2, 40, WHITE);
}

void DrawMenuScreen() {
    ClearBackground(DarkGreen);
    DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, LightGreen);

    DrawText("Select Difficulty", screenWidth / 2 - MeasureText("Select Difficulty", 60) / 2, 100, 60, WHITE);

    for (int i = 0; i < totalOptions; i++) {
        Color textColor = (i == selectedOption%totalOptions) ? YELLOW : WHITE;
        DrawText(difficultyOptions[i], screenWidth / 2 - MeasureText(difficultyOptions[i], 50) / 2, 250 + i * 80, 50, textColor);
    }
    DrawText("Use UP/DOWN to navigate, ENTER to select", screenWidth / 2 - MeasureText("Use UP/DOWN to navigate, ENTER to select", 30) / 2, screenHeight - 100, 30, WHITE);
    DrawText("Use the UP/DOWN arrow keys to take your paddle up and down.", screenWidth / 2 - MeasureText("Use the UP/DOWN arrow keys to take your paddle up and down.", 20) / 2, screenHeight - 130, 20, BLACK);
    DrawText("The first to 11 points wins.", screenWidth / 2 - MeasureText("The first to 11 points wins.", 20) / 2, screenHeight - 150, 20, BLACK);
}

void UpdateMenuInput() {
    if (IsKeyPressed(KEY_DOWN)) {
        selectedOption = (selectedOption + 1) % totalOptions;  
    }
    if (IsKeyPressed(KEY_UP)) {
        selectedOption = (selectedOption - 1 + totalOptions) % totalOptions;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        ShowMenu = false;  
        switch (selectedOption) {
            case 0: ball.speedX = 5; ball.speedY = 5; player.speed = cpu.speed = 4; break;
            case 1: ball.speedX = 7; ball.speedY = 6.5; player.speed = 5; cpu.speed = 5.5; cpu.height = 116; break;
            case 2: ball.speedX = 9; ball.speedY = 8.5; player.speed = 7.5; cpu.speed = 7;cpu.height = 124;break;
        }
        int countdown = 3;
        while (countdown > 0) {
            BeginDrawing();
            DrawCountdown(countdown);
            EndDrawing();
            this_thread::sleep_for(chrono::seconds(1));
            countdown--;
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void PresentScreen(){
    ClearBackground(DarkGreen);
    DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, LightGreen);
    DrawText("Parth Gargate Presents", screenWidth/2 -MeasureText("Parth Gargate Presents",80)/2, screenHeight/2 -40, 80, BLACK);
}

bool win(){
    if(playerScore == 11 || cpuScore == 11) return true;
    return false;
}

int main() {
    cout << "Starting game!!!" << endl;
    InitWindow(screenWidth, screenHeight, "Pong Game");
    SetTargetFPS(120);

    player.width = 25;
    player.height = 120;
    player.x = screenWidth - player.width - 10;
    player.y = (screenHeight - player.height) / 2;
    player.speed = 10;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = (screenHeight - cpu.height) / 2;
    cpu.speed = 11;

    while (!WindowShouldClose() && !win()) {
        if(ShowPresent){
            BeginDrawing();
            PresentScreen();
            EndDrawing();
            this_thread::sleep_for(chrono::seconds(3));
            ShowPresent = false;
        }
        if (ShowIntro) {
            BeginDrawing();
            DrawIntroScreen();
            EndDrawing();
            if (IsKeyPressed(KEY_ENTER)) {
                ShowIntro = false; 
            }
        } 
        else if(ShowMenu){
            BeginDrawing();
            DrawMenuScreen();
            EndDrawing();
            UpdateMenuInput();
        }
        else {
            BeginDrawing();
            // Updation
            ball.Update();
            player.Update();
            cpu.Update(ball.y);

            // Check for collisions
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, 
                Rectangle{player.x, player.y, player.width, player.height})){
                ball.speedX *= -1;
            }
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, 
                Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
                ball.speedX *= -1;
            }
            // Drawing
            ClearBackground(DarkGreen);
            DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, LightGreen);
            player.Draw();
            cpu.Draw();
            ball.Draw();
            DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
            DrawText(TextFormat("%i", cpuScore), screenWidth / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", playerScore), 3 * screenWidth / 4 - 20, 20, 80, WHITE);
            EndDrawing();
        }
    }

    if(playerScore == 11){
        BeginDrawing();
        DrawText("You Win!!!", screenWidth / 2 - MeasureText("You Win!!!", 80) / 2, screenHeight / 2 - 100, 80, WHITE);
        EndDrawing();
    }
    if(cpuScore == 11){
        BeginDrawing();
        DrawText("You Lose!!!", screenWidth / 2 - MeasureText("You Lose!!!", 80) / 2, screenHeight / 2 - 100, 80, WHITE);
        EndDrawing();
    }
    this_thread::sleep_for(chrono::seconds(3));
    CloseWindow();
}
