/*
 * paddle.c - A user controlled paddle to block the ball
 */

#include <curses.h>
#include <stdlib.h>

#include "paddle.h"

#define PCOL 0 // Paddle column

/*
 * Create a bar structure
 */
struct bar *create_bar( WINDOW *table, int height )
{
    struct bar *p;

    p = malloc(sizeof(struct bar));     //TODO Free memory?
    p->lower = 2;
    p->upper = 2 + height;
    place_bar(table, p);

    return p;
}

/*
 * Place the bar structure in the center of the appropriate column
 */
void place_bar( WINDOW *table, struct bar *paddle )
{
    int i;
    for (i = paddle->lower; i <= paddle->upper; i++) {
        mvwaddch(table, i, PCOL, ACS_BLOCK);
    }
    wrefresh(table);
}

/*
 * Increment the column position of the input bar structure in the
 * input direction
 */
int move_bar( WINDOW *table, struct bar *paddle, int direct )
{
    if ( direct == -1 ) {
        if ( (paddle->upper + 1) > table->_maxy ) {
            return 0;
        }
        else {
            mvwaddch(table, paddle->lower, PCOL, ' ');
            paddle->upper++;
            paddle->lower++;
            mvwaddch(table, paddle->upper, PCOL, ACS_BLOCK);
            wrefresh(table);
            return -1;
        }
    }
    if ( direct == 1 ) {
        if ( (paddle->lower - 1) < (table->_begy + table->_yoffset) ) {
            return 0;
        }
        else {
            mvwaddch(table, paddle->upper, PCOL, ' ');
            paddle->upper--;
            paddle->lower--;
            mvwaddch(table, paddle->lower, PCOL, ACS_BLOCK);
            wrefresh(table);
            return 1;
        }
    }
    return 0;
}

