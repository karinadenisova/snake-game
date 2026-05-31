#include <iostream>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <vector>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #define CLEAR system("cls")
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    char getch() {
        struct termios oldt, newt;
        char ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
    #define CLEAR system("clear")
#endif

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;
deque<pair<int,int>> snake;
pair<int,int> food;
int score = 0;
char dir = 'd';

void setup() {
    snake.push_front({HEIGHT/2, WIDTH/2});
    snake.push_back({HEIGHT/2, WIDTH/2-1});
    snake.push_back({HEIGHT/2, WIDTH/2-2});
    srand(time(0));
    food = {rand() % HEIGHT, rand() % WIDTH};
    score = 0;
    dir = 'd';
}

void draw() {
    CLEAR;
    for (int i = 0; i < HEIGHT+2; i++) cout << '#';
    cout << endl;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) cout << '#';
            bool isSnake = false;
            for (auto &seg : snake) if (seg.first == i && seg.second == j) isSnake = true;
            if (isSnake) cout << 'O';
            else if (food.first == i && food.second == j) cout << 'F';
            else cout << ' ';
            if (j == WIDTH-1) cout << '#';
        }
        cout << endl;
    }
    for (int i = 0; i < HEIGHT+2; i++) cout << '#';
    cout << "\nScore: " << score << "   Use WASD. Press q to quit.\n";
}

void input() {
    if (_kbhit() || true) {
        char c;
        #ifdef _WIN32
            c = tolower(getch());
        #else
            c = tolower(getch());
        #endif
        if (c == 'a' && dir != 'd') dir = 'a';
        else if (c == 'd' && dir != 'a') dir = 'd';
        else if (c == 'w' && dir != 's') dir = 'w';
        else if (c == 's' && dir != 'w') dir = 's';
        else if (c == 'q') exit(0);
    }
}

void logic() {
    pair<int,int> newHead = snake.front();
    if (dir == 'a') newHead.second--;
    else if (dir == 'd') newHead.second++;
    else if (dir == 'w') newHead.first--;
    else if (dir == 's') newHead.first++;

    if (newHead.first < 0 || newHead.first >= HEIGHT || newHead.second < 0 || newHead.second >= WIDTH) {
        cout << "\nGAME OVER!\n";
        exit(0);
    }
    for (auto &seg : snake) if (seg == newHead) { cout << "\nGAME OVER!\n"; exit(0); }

    snake.push_front(newHead);
    if (newHead == food) {
        score++;
        food = {rand() % HEIGHT, rand() % WIDTH};
    } else {
        snake.pop_back();
    }
}

int main() {
    setup();
    while (true) {
        draw();
        input();
        logic();
        #ifdef _WIN32
            Sleep(100);
        #else
            usleep(100000);
        #endif
    }
    return 0;
}
