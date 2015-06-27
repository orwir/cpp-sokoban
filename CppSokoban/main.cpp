#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <ctype.h>

//////////////////////////////
//Game settings
const int ROWS_COUNT = 10;
const int COLS_COUNT = 15;
const unsigned char SYMBOL_HERO = 2;
const unsigned char SYMBOL_WALL = 177;
const unsigned char SYMBOL_BOX = 254;
const unsigned char SYMBOL_EXIT = 176;

const unsigned char LEVEL_DATA_0[ROWS_COUNT][COLS_COUNT+1] =
{
    "#####2#########",
    "#  X   #   X ##",
    "# X ### X  #  #",
    "#X X  X  ## X #",
    "# X    ##  #  #",
    "#######    # X#",
    "#   X   XX #X #",
    "#XXX # # X   ##",
    "#1 X #   X X  #",
    "###############",
};

//logic variables
HANDLE consoleHandle = 0;
bool isRunning = true;
unsigned char levelData[ROWS_COUNT][COLS_COUNT];
int heroRow = 0;
int heroCol = 0;

void SetupSystem();
void Initialize();
void Render();
void Update();
void Shutdown();
void MoveHeroTo(int, int);

void main()
{
    SetupSystem();
    Initialize();

    do
    {
        Render();
        Update();

    } while (isRunning);

    printf("\n\n\tCongratulations! You win. Press any key to close game...");
    _getch();
}

void Render()
{
    COORD cursor;
    cursor.X = 0;
    cursor.Y = 0;
    SetConsoleCursorPosition(consoleHandle, cursor);

    printf("\n\n\t");
    for (int r = 0; r < ROWS_COUNT; r++)
    {
        for (int c = 0; c < COLS_COUNT; c++)
        {
            unsigned char symbol = levelData[r][c];
            switch (symbol)
            {
            case SYMBOL_WALL:
                SetConsoleTextAttribute(consoleHandle, 15);
                break;
            case SYMBOL_HERO:
                SetConsoleTextAttribute(consoleHandle, 10);
                break;
            case SYMBOL_BOX:
                SetConsoleTextAttribute(consoleHandle, 14);
                break;
            case SYMBOL_EXIT:
                SetConsoleTextAttribute(consoleHandle, 12);
                break;
            }
            printf("%c", symbol);
        }
        printf("\n\t");
    }
    SetConsoleTextAttribute(consoleHandle, 7);
    printf("\n\n\tUse WASD to move ");
    SetConsoleTextAttribute(consoleHandle, 10);
    printf("Hero");
    SetConsoleTextAttribute(consoleHandle, 7);
    printf(". R - restart level");
}

void Update()
{
    unsigned char inputChar = _getch();
    inputChar = tolower(inputChar);
    switch (inputChar)
    {
    case 'w':
        MoveHeroTo(heroRow - 1, heroCol);
        break;
    case 'a':
        MoveHeroTo(heroRow, heroCol - 1);
        break;
    case 's':
        MoveHeroTo(heroRow + 1, heroCol);
        break;
    case 'd':
        MoveHeroTo(heroRow, heroCol + 1);
        break;
    case 'r':
        Initialize();
        break;
    }

}

void MoveHeroTo(int row, int col)
{
    unsigned char destination = levelData[row][col];
    bool canMove = false;
    switch (destination)
    {
    case ' ':
        canMove = true;
        break;
    case SYMBOL_EXIT:
        isRunning = false;
        break;
    case SYMBOL_BOX:
        int heroDirectionRow = row - heroRow;
        int heroDirectionCol = col - heroCol;
        if (levelData[row + heroDirectionRow][col + heroDirectionCol] == ' ')
        {
            canMove = true;
            levelData[row][col] = ' ';
            levelData[row + heroDirectionRow][col + heroDirectionCol] = SYMBOL_BOX;
        }
        break;
    }
    if (canMove) {
        levelData[heroRow][heroCol] = ' ';
        heroRow = row;
        heroCol = col;
        levelData[heroRow][heroCol] = SYMBOL_HERO;
    }
}

void SetupSystem()
{
    srand(time(0));
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Initialize()
{
    //load
    for (int r = 0; r < ROWS_COUNT; r++)
    {
        for (int c = 0; c < COLS_COUNT; c++)
        {
            unsigned char symbol = LEVEL_DATA_0[r][c];
            switch (symbol)
            {
            case '#':
                levelData[r][c] = SYMBOL_WALL;
                break;
            case '1':
                levelData[r][c] = SYMBOL_HERO;
                heroRow = r;
                heroCol = c;
                break;
            case '2':
                levelData[r][c] = SYMBOL_EXIT;
                break;
            case 'X':
                levelData[r][c] = SYMBOL_BOX;
                break;
            default:
                levelData[r][c] = symbol;
                break;
            }
        }
    }
}

void Shutdown()
{
    system("cls");
    printf("Press any key to continue...");
    _getch();
}
