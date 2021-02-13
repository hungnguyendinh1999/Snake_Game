#include <iostream>
#include <ncurses.h>
#include <string>
#include <fstream>

#define KEY_UP 0403
#define KEY_DOWN 0402
#define KEY_LEFT 0404
#define KEY_RIGHT 0405

using namespace std;

bool gameOver;         // only global var
const int width = 20;  // horizontal dimension
const int height = 20; // vertical dimension
int x, y, treatX, treatY, score;
int tailX[100], tailY[100];
int nTail;
enum enumDirection
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
enumDirection dir;

/** PRIVATE METHODS **/
void drawGrid()
{
    for (int i = 0; i < width + 2; i++)
        mvprintw(0, i, "#");

    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            if (i == 0 || i == height + 1)
                mvprintw(i, j, "+");
            else if (j == 0 || j == width + 1)
                mvprintw(i, j, "+");
            else if (i == y && j == x) // Head of snake
                mvprintw(i, j, "O");
            else if (i == treatY && j == treatX)
                mvprintw(i, j, "X");
            else
            {
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        mvprintw(i, j, "o");
                    }
                }
            }
        }
    }
    mvprintw(height + 3, 0, "Score %d", score);
    refresh();
}

/** PUBLIC **/
void Setup()
{
    // Make screen look better
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    gameOver = false;
    dir = STOP;
    x = width / 2; // center snake head
    y = height / 2;
    treatX = (rand() % width) + 1;
    treatY = (rand() % height) + 1;
    score = 0;
}

void Draw()
{
    clear();
    /**DRAW GRID**/
    drawGrid();
}

void Input()
{
    keypad(stdscr, TRUE);
    halfdelay(1);

    int c = getch();
    switch (c)
    {
    case KEY_LEFT:
        if (dir != RIGHT)
            dir = LEFT;
        break;
    case KEY_RIGHT:
        if (dir != LEFT)
            dir = RIGHT;
        break;
    case KEY_UP:
        if (dir != DOWN)
            dir = UP;
        break;
    case KEY_DOWN:
        if (dir != UP)
            dir = DOWN;
        break;
    case 113:
        gameOver = true;
        break;
    }
}

void Logic()
{
    // Tail Logic
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Direction Logic
    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    // out of bound
    if (x > width || x < 1 || y < 1 || y > height)
        gameOver = true;

    // Treat encouter
    if (x == treatX && y == treatY)
    {
        score++;
        treatX = (rand() % width) + 1;
        treatY = (rand() % height) + 1;
        nTail++;
    }

    for (int i = 0; i < nTail; i++)
    {
        if (tailX[i] == x && tailY[i] == y)
        {
            gameOver = true;
        }
    }
}

void OpeningScreen()
{
    ifstream file("beginscreen.txt");
    if (!file)
    {

        cout << "Cannot open input file!" << endl;
        return;
    }
    string line;
    int row = 0;
    while (getline(file, line))
    {
        char *char_arr;
        char_arr = &line[0];
        mvprintw(row++, 0, char_arr);
    }

}
int startGame();
void GameOver()
{
    ifstream file("gameover.txt");
    if (!file)
    {

        cout << "Cannot open input file!" << endl;
        return;
    }
    string line;
    int row = 0;
    while (getline(file, line))
    {
        char *char_arr;
        char_arr = &line[0];
        mvprintw(row++, 0, char_arr);
    }

    int c = getch();
    switch(c)
    {
        case KEY_LEFT:
            gameOver = false;
            startGame();
            break;
        case 'q':
            break;
    }
    refresh();
}

int startGame()
{
    
    keypad(stdscr, TRUE);

    int c = getch();
    switch (c)
    {
    case KEY_LEFT:
        while (!gameOver)
        {
            Draw();
            Input();
            Logic();
        }
        GameOver();
        break;
    case 'q':
        break;
    
    }
    

    getch();
    endwin();
    return 0;
}

int main()
{
    Setup();
    OpeningScreen();

    return startGame();
}