#include <ncurses.h>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

#define WIDTH 80
#define HEIGHT 25

#define BORDER "#"

class Point {
    public: short x, y;
    public:
    Point() {
        x = 0;
        y = 0;
    }
    Point(short x, short y) {
        this->x = x;
        this->y = y;
    }
};

enum Go{up, left, down, right};

void get_food(Point* food) {
    srand(time(NULL));
    if (food[0].x == 0 && food[0].y == 0) {
        food[0].x = rand() % WIDTH;
        food[0].y = rand() % HEIGHT;
    }
}

void draw(Point* snake, Point* food) {
    for (int y = 0; y < HEIGHT + 2; ++y) {
        printw(BORDER);
        for (int x = 0; x < WIDTH + 2; ++x) {
            if (y == 0 || y == HEIGHT + 1)
                printw(BORDER);
            else if (x == food[0].x && y == food[0].y)
                mvaddch(y, x + 1, '.');
            else if (x == snake[0].x && y == snake[0].y)
                mvaddch(y, x + 1, 'o');
            else
                printw(" ");
        }
        printw(BORDER);
        printw("\n");
    }
}

void change_move(Go* go) {
    int key = getch();

    if (key == KEY_UP && *go != Go::down) *go = Go::up;
    else if (key == KEY_DOWN && *go != Go::up) *go = Go::down;
    else if (key == KEY_LEFT && *go != Go::right) *go = Go::left;
    else if (key == KEY_RIGHT && *go != Go::left) *go = Go::right;
}

void move_snake(Point *snake, Go go) {
    if (go == Go::up) snake[0].y--; 
    else if (go == Go::down) snake[0].y++;
    else if (go == Go::left) snake[0].x--;
    else if (go == Go::right) snake[0].x++;
}

bool is_game_over(Point* head) {
    bool check = false;
    if (head->x <= 0 || head->x > WIDTH || head->y <= 0 || head->y > HEIGHT) check = true;

    return check;
}

int main() {
    Point snake[HEIGHT * WIDTH] = {{-1, -1}};
    Point *head = snake;
    snake[0] = {WIDTH / 2, HEIGHT / 2 + 1};
    Point food[1] = {{0, 0}};

    Go go = up;
    initscr();
    curs_set(0);
    keypad (stdscr, 1);
    halfdelay(1);

    bool game_over = false;
    while (!game_over) {
        erase();
        get_food(food);

        draw(head, food);
        change_move(&go);
        //nodelay(stdscr, 1);
        move_snake(head, go);
        game_over = is_game_over(head);
    }
    clear();
    // printw("%-30s\n", "Game over!");
    endwin();
    
    return 0;
}
