#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <curses.h>

// TODO: add command line arguments to the program
    // allow to set fixed screen size
    // otherwise screensize is adaptive

#define MAX_LENGTH 256

typedef struct {
    int x;
    int y;
} vec2d;

int score = 0;
vec2d segments[MAX_LENGTH];

bool collide_snake(vec2d a) {
    for (int i = 0; i < score; i++) {
        if (a.x == segments[i].x && a.y == segments[i].y) {
            return true;
        }
    }
    return false;
}

void draw_border(int width, int height, char c) {
    for (int i = 0; i < width; i++) {
        mvaddch(0, i * 2, c);
    }
    for (int i = 1; i < height; i++) {
        mvaddch(i, 0, c);
        mvaddch(i, (width -1) * 2, c);
    }
    for (int i = 0; i < width; i++) {
        mvaddch(height - 1, i * 2, c);
    }
}

int main(int argc, char *argv[]) {
    WINDOW *win = initscr();

    int screen_width;
    int screen_height;

    getmaxyx(win, screen_height, screen_width);
    if (screen_width % 2 == 0) {
        screen_width = (screen_width / 2) - 2;
    }
    else {
        screen_width = (screen_width / 2) - 1;
    }
    screen_height -= 2;

    // setting up colors
    if (has_colors() == FALSE) {
        endwin();
        fprintf(stderr, "Your terminal does not support color\n");
        exit(1);
    }
    start_color();
    use_default_colors();
    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_GREEN, -1);

    keypad(win, true);
    nodelay(win, true);
    curs_set(0);

    vec2d head = { 0, 0 };
    vec2d dir = { 1, 0 };
    char head_char = '>';

    vec2d food = { rand() % (screen_width + 1), rand() % screen_height };

    char score_message[32];
    sprintf(score_message, "  Score: %d  ", score);

    while(true) {
        // reactive screen
        getmaxyx(win, screen_height, screen_width);
        if (screen_width % 2 == 0) {
            screen_width = (screen_width / 2) - 2;
        }
        else {
            screen_width = (screen_width / 2) - 1;
        }
        screen_height -= 2;

        int pressed = wgetch(win);
        if (pressed == KEY_LEFT) {
            if (dir.x == 1) continue;
            dir.x = -1;
            dir.y = 0;
            head_char = '<';
        }
        if (pressed == KEY_RIGHT) {
            if (dir.x == -1) continue;
            dir.x = 1;
            dir.y = 0;
            head_char = '>';
        }
        if (pressed == KEY_UP) {
            if (dir.y == 1) continue;
            dir.x = 0;
            dir.y = -1;
            head_char = '^';
        }
        if (pressed == KEY_DOWN) {
            if (dir.y == -1) continue;
            dir.x = 0;
            dir.y = 1;
            head_char = 'v';
        }
        if (pressed == '\e') {
            break;
        }

        // update snake segments
        for (int i = 0; i < score; i++) {
            segments[score - i] = segments[score - i - 1];
        }

        segments[0] = head;

        head.x += dir.x;
        head.y += dir.y;

        // snake loops through screen
        if (head.x < 0) {
            head.x = screen_width;
        }
        if (head.x > screen_width) {
            head.x = 0;
        }
        if (head.y < 0) {
            head.y = screen_height - 1;
        }
        if (head.y > screen_height - 1) {
            head.y = 0;
        }

        // keep berry on screen when resizing
        if (food.x > screen_width){
            food.x = screen_width;
        }
        if (food.y > screen_height - 1) {
            food.y = screen_height - 1;
        }

        erase();

        // draw food
        attron(COLOR_PAIR(1));
        mvaddch(food.y + 1, food.x * 2 + 1, '@');
        attroff(COLOR_PAIR(1));

        // draw snake
        attron(COLOR_PAIR(2));
        for (int i = 0; i < score; i++) {
            mvaddch(segments[i].y + 1, segments[i].x * 2 + 1, 'o');
        }
        mvaddch(head.y + 1, head.x * 2 + 1, head_char);
        attroff(COLOR_PAIR(2));

        draw_border(screen_width + 2, screen_height + 2, '#');

        mvaddstr(0, screen_width - 5, score_message);

        if (collide_snake(head)) {
            while(true) {
                int pressed = wgetch(win);
                if (pressed == ' ') {
                    score = 0;
                    head.x = 0;
                    head.y = 0;
                    dir.x = 1;
                    dir.y = 0;
                    head_char = '>';
                    sprintf(score_message, "  Score: %d  ", score);
                    break;
                }
                if (pressed == '\e') {
                    getch();
                    endwin();
                    exit(0);
                }
                attron(COLOR_PAIR(1));
                mvaddstr(screen_height - 2, screen_width - 3, "GAME OVER");
                mvaddstr(screen_height - 1, screen_width - 15, "[SPACE] = RESTART | [ESC] = EXIT");
                attroff(COLOR_PAIR(1));
            }
        }

        if (food.x == head.x && food.y == head.y) {
            if (score < MAX_LENGTH) {
                score += 1;
            }
            else {
                // WIN!
            }
            sprintf(score_message, "  Score: %d  ", score);

            while(collide_snake(food) || (food.x == head.x && food.y == head.y)) {
                food.x = rand() % (screen_width + 1);
                food.y = rand() % screen_height;
            }
        }

        usleep(100000);

    }

    getch();
    endwin();

    return 0;
}
