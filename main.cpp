#include <iomanip>
#include <iostream>
#include <vector>
#include <random>
#include <conio.h>

using namespace std;

struct Random
{
    Random(int min, int max)
        : mUniformDistribution(min, max)
    {}
    int operator()()
    {
        return mUniformDistribution(mEngine);
    }
    default_random_engine mEngine{ random_device()() };
    uniform_int_distribution<int> mUniformDistribution;
};

vector<vector<int>> stage(22, vector<int>(13, 0));
vector<vector<int>> block =
{
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};

vector<vector<int>> field(22, vector<int>(13, 0));
int y = 0;
int x = 4;
bool gameover = false;
size_t GAMESPEED = 20000;

Random getRandom{ 0, 6 };

vector<vector<vector<int>>> block_list =
{
    {
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 }
    },
    {
        { 0, 0, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 1, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 0, 0, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 1, 0 }
    }
};

void gameLoop();
void display();
bool makeBlocks();
void initGame();
void moveBlock(int, int);
void collidable();
bool isCollide(int, int);
void userInput();
bool rotateBolck();
void spawnBlock();

void gameLoop()
{
    size_t time = 0;
    initGame();
    while (!gameover)
    {
        if (kbhit())
        {
            userInput();
        }

        if (time < GAMESPEED)
        {
            time++;
        }
        else
        {
            spawnBlock();
            time = 0;
        }
    }
}

void display()
{
    system("cls");
    for (size_t i = 0; i < 21; i++)
    {
        for (size_t j = 0; j < 12; j++)
        {
            switch (field[i][j])
            {
            case 0:
                cout << " " << flush;
                break;
            case 9:
                cout << "@" << flush;
                break;
            default:
                cout << "#" << flush;
                break;
            }
        }
        cout << endl;
    }
    if (gameover)
    {
        system("cls");
    }
}

void initGame()
{
    for (size_t i = 0; i <= 20; i++)
    {
        for (size_t j = 0; j <= 11; j++)
        {
            if ((j == 0) || (j == 11) || (i == 20))
            {
                field[i][j] = stage[i][j] = 9;
            }
            else
            {
                field[i][j] = stage[i][j] = 0;
            }
        }
    }
    makeBlocks();
    display();
}

bool makeBlocks()
{
    x = 4;
    y = 0;
    int blockType = getRandom();
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            block[i][j] = 0;
            block[i][j] = block_list[blockType][i][j];
        }
    }
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            field[i][j + 4] = stage[i][j + 4] + block[i][j];

            if (field[i][j + 4] > 1)
            {
                gameover = true;
                return true;
            }
        }
    }
    return false;
}

void moveBlock(int x2, int y2)
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            field[y + i][x + j] -= block[i][j];
        }
    }
    x = x2;
    y = y2;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            field[y + i][x + j] += block[i][j];
        }
    }
    display();
}

void collidable()
{
    for (size_t i = 0; i<21; i++)
    {
        for (size_t j = 0; j<12; j++)
        {
            stage[i][j] = field[i][j];
        }
    }
}

bool isCollide(int x2, int y2)
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            if (block[i][j] && stage[y2 + i][x2 + j] != 0)
            {
                return true;
            }
        }
    }
    return false;
}

void userInput()
{
    char key;
    key = getch();
    switch (key)
    {
    case 'd':
        if (!isCollide(x + 1, y))
        {
            moveBlock(x + 1, y);
        }
        break;
    case 'a':
        if (!isCollide(x - 1, y))
        {
            moveBlock(x - 1, y);
        }
        break;
    case 's':
        if (!isCollide(x, y + 1))
        {
            moveBlock(x, y + 1);
        }
        break;
    case ' ':
        rotateBolck();
    }
}

bool rotateBolck()
{
    vector<vector<int>> tmp(4, vector<int>(4, 0));
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            tmp[i][j] = block[i][j];
        }
    }
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            block[i][j] = tmp[3 - j][i];
        }
    }
    if (isCollide(x, y))
    {
        for (size_t i = 0; i < 4; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                block[i][j] = tmp[i][j];
            }
        }
        return true;
    }
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            field[y + i][x + j] -= tmp[i][j];
            field[y + i][x + j] += block[i][j];
        }
    }
    display();
    return false;
}

void spawnBlock()
{
    if (!isCollide(x, y + 1))
    {
        moveBlock(x, y + 1);
    }
    else
    {
        collidable();
        makeBlocks();
        display();
    }
}

int main()
{
    gameLoop();
    return 0;
}
