#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <cstdlib>
#include <ctime>

using namespace std;

Color green = {131, 197, 157, 255};
Color red = {200, 0, 0, 255};
Color darkGrey = {33, 33, 33, 255};
Color purple = {116, 80, 171, 255};

int cellSize = 24;
int cellCount = 25;
int offset = 75;

bool ElementrInDeque(Vector2 element, deque<Vector2> deque)
{
    for(unsigned int i = 0; i < deque.size(); i++){
        if(Vector2Equals(deque[i], element)){
            return true;
        }
    }
    return false;
}

double lastUpdateTime = 0;

bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval){
    lastUpdateTime = currentTime;
    return true;
    }
    return false;
}

class Snake
{
public:
    deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    Vector2 direction = {1, 0};
    bool addSegment = false;
    Texture2D headTexture;

    Snake()
    {
        Image image = LoadImage("Lucka_Snake.png");
        headTexture = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    void Draw()
    {
    DrawTexture(headTexture, offset + body[0].x * cellSize, offset + body[0].y * cellSize, WHITE);
    for(unsigned int i = 1; i < body.size(); i++)
        {
        float x = body[i].x;
        float y = body[i].y;
        Rectangle segment = Rectangle{offset + x*cellSize, offset + y*cellSize, (float)cellSize, (float)cellSize};
        DrawRectangleRounded(segment, 0.5, 6, purple);
        }
    }

    ~Snake()
    {
        UnloadTexture(headTexture);
    }

    void Update()
    {
        body.push_front(Vector2Add(body[0], direction));
        if (addSegment == true)
        {
            addSegment = false;
        }
        else
        {
            body.pop_back();
        }
    } 
};

class Food
{    

public:
    Vector2 position = {5,6};
    Texture2D texture;
    string foodPictures[4] = {"Kuba_snake.png", "Kuba_snake2.png", "Kuba_snake3.png", "Kuba_snake4.png"};

    Food(deque<Vector2> snakeBody)
    { 
        LoadRandomPicture();
        position = GenerateRandomPos(snakeBody);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void LoadRandomPicture()
    {
        srand(time(0));
        int randomNumber = rand() % 4;
        Image image = LoadImage(foodPictures[randomNumber].c_str());
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }

    void Draw()
    {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(1, cellCount - 2);
        float y = GetRandomValue(1, cellCount - 2);
        return Vector2{x, y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
    {  
        Vector2 position = GenerateRandomCell();
        while(ElementrInDeque(position, snakeBody))
        {
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Game
{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0; 
    Sound Kuba_Chcipneme;
    Sound Panik_hokejka;
    Sound Panik_pingpong;
    Sound JaksecitisKubo;
    Sound Hirsch_Kacer;
    Sound Panik_zdravim;
    Sound KubaNatyAlko;
    Sound NatyUsmev;
    Sound VojtaVulgarni;
    Sound Panik_helma;

    Game()
    {
        InitAudioDevice();
        Kuba_Chcipneme = LoadSound("Kuba_Chcipneme.mp3");
        Panik_hokejka = LoadSound("PanikHratSHokejkou.mp3");
        Panik_pingpong = LoadSound("Panik_PingPong.mp3");
        JaksecitisKubo = LoadSound("JaksecitisKubo.mp3");
        Hirsch_Kacer = LoadSound("HirKacer.mp3");
        KubaNatyAlko = LoadSound("KubaNatyJontak.mp3");
        NatyUsmev = LoadSound("NatyUsmevBanan.mp3");
        VojtaVulgarni = LoadSound("VojtaSeOmlouvaVulgarni.mp3");
        Panik_helma = LoadSound("PanikChybiHelma.mp3");
    }

    ~Game()
    {
        UnloadSound(Kuba_Chcipneme);
        UnloadSound(Panik_hokejka);
        UnloadSound(Panik_pingpong);
        UnloadSound(JaksecitisKubo);
        UnloadSound(Hirsch_Kacer);
        UnloadSound(KubaNatyAlko);
        UnloadSound(NatyUsmev);
        UnloadSound(VojtaVulgarni);
        UnloadSound(Panik_helma);
        
        CloseAudioDevice();
    }

    void random_sound()
    {
        srand(time(0));
        int randomNumber = rand() % 9 + 1;
        switch (randomNumber)
        {
        case 1:
            PlaySound(Panik_hokejka);
            break;
        case 2:
            PlaySound(VojtaVulgarni);
            break;
        case 3:
            PlaySound(Panik_pingpong);
            break;
        case 4:
            PlaySound(JaksecitisKubo);
            break;
        case 5:
            PlaySound(Hirsch_Kacer);
            break;
        case 6:
            PlaySound(Panik_helma);
            break;
        case 7:
            PlaySound(KubaNatyAlko);
            break;
        case 8:
            PlaySound(NatyUsmev);
            break;
        case 9:
            PlaySound(VojtaVulgarni);
            break;
        default:
            break;
        }
    }

    void Draw()
    {
        food.Draw();
        snake.Draw();
    }

    void Update()
    {
        if(running)
        {
        snake.Update(); 
        CheckCollisionWithFood();
        CheckCollisionWithEdges();
        CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood()
    {
        if(Vector2Equals(snake.body[0], food.position))
        {
            food.LoadRandomPicture();
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            score ++;
            random_sound();
        }
    }

    void CheckCollisionWithEdges()
    {
        if(snake.body[0].x == cellCount || snake.body[0].x == -1)
        {
            GameOver();
        }
        if(snake.body[0].y == cellCount || snake.body[0].y == -1)
        {
            GameOver();
        }
    }

    bool gameOverScreen = false;

    void GameOver()
    {
        cout << "Game over"<<endl;
        running = false;
        gameOverScreen = true;
    }

    void CheckCollisionWithTail()
    {
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if(ElementrInDeque(snake.body[0], headlessBody)){
            GameOver();
        }
    }
};

int main()
{

    cout << "Starting the game..." << endl;

    InitWindow(2*offset + cellSize*cellCount, 2*offset + cellSize*cellCount, "Eat_Jackob_Game");
    SetTargetFPS(60);
    
    Game game = Game();

    PlaySound(game.Kuba_Chcipneme);

    bool gameStarted = false;

    while (gameStarted == false)
    {
        BeginDrawing();

        if (!gameStarted)
        {
            DrawText("Press ENTER to start the game", GetScreenWidth() / 2 - MeasureText("Press ENTER to start the game", 20) / 2, GetScreenHeight() / 2 - 20, 20, WHITE);
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            gameStarted = true;
        }

        EndDrawing();
    }

    double lastDirectionChangeTime = 0;

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        float time_of_step = 0.25;

        if(eventTriggered(time_of_step))
        {
            game.Update();
        }
        if (GetTime() - lastDirectionChangeTime >= time_of_step)
        {
            if((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && game.snake.direction.y != 1)
            {
                game.snake.direction = {0, -1};
                lastDirectionChangeTime = GetTime();
            }
            if((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && game.snake.direction.y != -1)
            {
                game.snake.direction = {0, 1};
                lastDirectionChangeTime = GetTime();
            }
            if((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && game.snake.direction.x != 1)
            {
                game.snake.direction = {-1, 0};
                lastDirectionChangeTime = GetTime();
            }
            if((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && game.snake.direction.x != -1)
            {
                game.snake.direction = {1, 0};
                lastDirectionChangeTime = GetTime();
            }
        }
        ClearBackground(green);
        DrawRectangleLinesEx( Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize*cellCount+10, (float)cellSize*cellCount+10}, 5, darkGrey);
        game.Draw();
        DrawText("Eat Jackob!", offset - 5, 20, 40, darkGrey);
        DrawText(TextFormat("Score: %i", game.score), offset + cellCount*cellSize - 2*offset, 20, 40, darkGrey);

        if (game.gameOverScreen)
        {
            if(game.score < 5)
            {
                DrawText(TextFormat("U did bad brother, ur mum is not proud, ur score is: %i", game.score), GetScreenWidth() / 2 - MeasureText("U did bad brother, ur mum is not proud, ur score is: %i", 20) / 2, GetScreenHeight() / 2 - 20, 20, red);
            }
            else if(game.score < 20)
            {
                DrawText(TextFormat("You did well, your mum is now proud, your score is: %i", game.score), GetScreenWidth() / 2 - MeasureText("You did well, your mum is now proud, your score is: %i", 20) / 2, GetScreenHeight() / 2 - 20, 20, red);
            }    
            else
            {
                DrawText(TextFormat("You did AMAZING, your score is: %i", game.score), GetScreenWidth() / 2 - MeasureText("You did AMAZING, your score is: %i", 20) / 2, GetScreenHeight() / 2 - 20, 20, red);
            }            
            DrawText("Press ENTER to restart, press ESC to end the game", GetScreenWidth() / 2 - MeasureText("Press ENTER to restart, press ESC to end the game", 20) / 2, GetScreenHeight() / 2 + 20, 20, red);
            if (IsKeyPressed(KEY_ENTER))
            {
                game.gameOverScreen = false;
                game.running = true;
                game.score = 0;
                game.snake.body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
                game.snake.direction = {1, 0};
                game.food.position = game.food.GenerateRandomPos(game.snake.body);
            }
        }

        EndDrawing();    
    }
   
    CloseWindow();

    return 0;
} 
