#include <Arduino.h>
#include <main.cpp>

void updateSnake();
void setGrid();

/*
    grass = x
    snakehead = s
    snakebody = b
    food = +
    wall = #
    num = n
    blank = 0
*/

HslColor grass = green_l;
HslColor snakehead = blue;
HslColor snakebody = blue_l;
HslColor food = red;
HslColor wall = violet;
HslColor num = blue;
HslColor blank = black;

char gridSnake[rows][rowLength] =
{
  //  0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}, // 0
    {'#', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', '#'}, // 1
    {'#', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', '#'}, // 2
    {'#', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', '#'}, // 3
    {'#', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', '#'}, // 4
    {'#', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', '#'}, // 5
    {'#', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', '#'}, // 6
    {'#', 'x', 'x', 'x', 'x', 'x', '+', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 's', 'b', 'b', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', '#'}, // 7
    {'#', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', '#'}, // 8
    {'#', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', '#'}, // 9
    {'#', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', '#'}, // 10
    {'#', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', '#'}, // 11
    {'#', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', '#'}, // 12
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}, // 13
};

bool gameover;

int snake_X, snake_Y, fruit_X, fruit_Y, score;

int tail_X[400], tail_Y[400]; //snake coordinates
int nTail;

enum eDirecton {
    STOP = 0, 
    LEFT,
    RIGHT, 
    UP, 
    DOWN
};
eDirecton dir;

void initGame()
{
    gameover = false;
    dir = STOP;
    score = 0;

    snake_X = 7;
    snake_Y = 18;
    fruit_X = 7;
    fruit_Y = 6;

    tail_X[0] = 7;
    tail_X[1] = 7;
    tail_Y[0] = 19;
    tail_Y[1] = 20;
    nTail = 2;

    gameupdate = &updateSnake;
}

void updateSnake()
{
    if(gameover) return;
    
    int temp_X = snake_X;
    int temp_Y = snake_Y;

    switch(dir) {
        case LEFT:
            temp_Y--;
            break;
        case RIGHT:
            temp_Y++;
            break;
        case UP:
            temp_X++;
            break;
        case DOWN:
            temp_X--;
            break;
        default:
            break;
    }

    if(gridSnake[temp_X][temp_Y] == 'b' || gridSnake[temp_X][temp_Y] == '#') {
        gameover = true;
        // Gameovercallback!!
        return;
    }
    if(gridSnake[temp_X][temp_Y] == '+') {
        score++;
        bool isFound = false;
        while(!isFound) {
            int x = rand() % rows;
            int y = rand() % rowLength;
            if(gridSnake[x][y] == 'x')
            {
                fruit_X = x;
                fruit_Y = y;
                isFound = true;
            }
        }
    }

    nTail = 2+score;

    for(int i=nTail; i>0; i--)
    {
        tail_X[i] = tail_X[i-1];
        tail_Y[i] = tail_Y[i-1];
    }
    tail_X[0] = snake_X;
    tail_Y[0] = snake_Y;

    snake_X = temp_X;
    snake_Y = temp_Y;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < rowLength; j++)
        {
            if(gridSnake[i][j] != '#')
            {
                gridSnake[i][j] = 'x';
            }
        }
    }

    gridSnake[snake_X][snake_Y] = 's';
    gridSnake[fruit_X][fruit_Y] = 'x';

    for(int i=0; i<nTail; i++)
    {
        gridSnake[tail_X[i]][tail_Y[i]] = 'b';
    }

    setGrid();
}

void setGrid()
{
    HslColor canvas[pixelCount];
    HslColor color;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < rowLength; j++)
        {
            switch (gridSnake[i][j])
            {
                case 'x':
                    color = grass;
                    break;
                case '#':
                    color = wall;
                    break;
                case 's':
                    color = snakehead;
                    break;
                case 'b':
                    color = snakebody;
                    break;
                case '+':
                    color = food;
                    break;
                case 'n':
                    color = num;
                    break;
                default:
                    color = blank;
                    break;
            }
            if (i % 2)
            {
                canvas[i * rowLength + (rowLength - 1 - j)] = color;
            }
            else
            {
                canvas[i * rowLength + j] = color;
            }
        }
    }
    draw(canvas);
}