#!/usr/bin/env python3
import curses
import random
import time

def main(stdscr):
    curses.curs_set(0)
    stdscr.nodelay(1)
    stdscr.timeout(100)
    sh, sw = stdscr.getmaxyx()
    w = curses.newwin(sh, sw, 0, 0)
    w.keypad(1)

    snake_x = sw // 4
    snake_y = sh // 2
    snake = [[snake_y, snake_x], [snake_y, snake_x-1], [snake_y, snake_x-2]]
    direction = curses.KEY_RIGHT
    food = [sh//2, sw//2]
    w.addch(food[0], food[1], curses.ACS_PI)
    score = 0

    while True:
        key = w.getch()
        if key in [curses.KEY_LEFT, curses.KEY_RIGHT, curses.KEY_UP, curses.KEY_DOWN]:
            direction = key

        head = snake[0]
        new_head = [head[0], head[1]]
        if direction == curses.KEY_DOWN:
            new_head[0] += 1
        if direction == curses.KEY_UP:
            new_head[0] -= 1
        if direction == curses.KEY_LEFT:
            new_head[1] -= 1
        if direction == curses.KEY_RIGHT:
            new_head[1] += 1

        if (new_head[0] in [0, sh-1] or new_head[1] in [0, sw-1] or new_head in snake):
            w.addstr(sh//2, sw//2 - 5, f" GAME OVER! Score: {score} ")
            w.refresh()
            time.sleep(2)
            break

        snake.insert(0, new_head)
        if new_head == food:
            score += 1
            food = None
            while food is None:
                nf = [random.randint(1, sh-2), random.randint(1, sw-2)]
                if nf not in snake:
                    food = nf
            w.addch(food[0], food[1], curses.ACS_PI)
        else:
            tail = snake.pop()
            w.addch(tail[0], tail[1], ' ')

        w.addch(snake[0][0], snake[0][1], curses.ACS_CKBOARD)
        w.addstr(0, sw//2 - 4, f" Score: {score} ")

if __name__ == "__main__":
    curses.wrapper(main)
