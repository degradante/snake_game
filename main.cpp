#include <ncurses.h>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

#define WIDTH 80
#define HEIGHT 25

#define BORDER "#"

class Point {
public: 
    short x, y;
    Point() {
        x = 0;
        y = 0;
    }
    Point(short x, short y) {
        this->x = x;
        this->y = y;
    }

    friend bool operator==(const Point &p1, const Point &p2) {
        return (p1.x == p2.x && p1.y == p2.y);
    }
};

enum Go{up, left, down, right};

void add_food(vector<Point> &food) {
    srand(time(NULL));
    for (size_t i = 0; i < food.size(); ++i) {
        if (food[i] == Point(0, 0)) {
            food[i].x = rand() % WIDTH + 1;
            food[i].y = rand() % HEIGHT + 1;
        }
    }
}

void draw(vector<Point> &snake, vector<Point> &food, Go go) {
    for (short y = 0; y < HEIGHT + 2; ++y) {
        printw(BORDER);
        for (short x = 0; x < WIDTH + 2; ++x) {
            Point p = {x, y};
            bool check = false;
            if (y == 0 || y == HEIGHT + 1) {
                printw(BORDER);
                check = true;
            }
            if (snake[0] == p) {
                switch (go) {
                    case Go::up:
                        mvaddch(y, x + 1, '^');
                        break;
                    case Go::left:
                        mvaddch(y, x + 1, '<');
                        break;
                    case Go::down:
                        mvaddch(y, x + 1, 'v');
                        break;
                    case Go::right:
                        mvaddch(y, x + 1, '>');
                        break;
                }
                check = true;
            }
            // i = 1 because head drawing upper
            for (size_t i = 1; i < snake.size(); ++i) {
                if (snake[i] == p) {
                    mvaddch(y, x + 1, 'o');
                    check = true;
                }
            }
            for (size_t i = 0; i < food.size(); ++i) {
                if (food[i] == p) {
                    if (food[i] == snake[0]) {
                        snake.resize(snake.size() + 1);
                        food[i] = {0, 0};
                    } else mvaddch(y, x + 1, '.');
                    check = true;
                }
            }
            if (!check)
                printw(" ");
        }
        printw(BORDER);
        printw("\n");
    }
}

void change_move(Go* go) {
    int key = getch();

    if ((key == KEY_UP || key == 'w') && *go != Go::down) *go = Go::up;
    else if ((key == KEY_DOWN || key == 's') && *go != Go::up) *go = Go::down;
    else if ((key == KEY_LEFT || key == 'a') && *go != Go::right) *go = Go::left;
    else if ((key == KEY_RIGHT || key == 'd') && *go != Go::left) *go = Go::right;
}

void move_snake(vector<Point> &snake, Go go) {
    for (size_t i = snake.size() - 1; i >= 1; --i) snake[i] = snake[i - 1];      
    if (go == Go::up) snake[0].y--; 
    else if (go == Go::down) snake[0].y++;
    else if (go == Go::left) snake[0].x--;
    else if (go == Go::right) snake[0].x++;
}

bool is_game_over(vector<Point> &snake) {
    bool check = false;
    Point head = snake[0];
    if (head.x <= 0 || head.x > WIDTH || head.y <= 0 || head.y > HEIGHT) check = true;
    else {
        for (size_t i = 1; i < snake.size() && !check; ++i)
            if (head == snake[i])
                check = true;
    }

    return check;
}

int main() {
    initscr();
    curs_set(0);
    keypad (stdscr, 1);
    halfdelay(1);

    vector<Point> snake = {1, {WIDTH / 2, HEIGHT / 2 + 1}};
    vector<Point> food = {2, {0, 0}};
    Go go = up;

    bool game_lose = false;
    bool game_win = false;
    while (!game_lose && !game_win) {
        erase();
        add_food(food);
        draw(snake, food, go);
        change_move(&go);
        //nodelay(stdscr, 1);
        move_snake(snake, go);
        game_lose = is_game_over(snake);
        if (snake.size() == WIDTH * HEIGHT) game_win = true;
    }
    clear();
    endwin();

    if (game_lose) cout << "\n\tВы проиграли\n\n";
    if (game_win) cout << "\n\tПоздравляем победителя!\n\n";

    return 0;
}
