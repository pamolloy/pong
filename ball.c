/*
 * ball.c - TODO
 */

#include <curses.h>
#include <stdlib.h>

#include "ball.h"
#include "paddle.h"

#define MAXNUM 3

/*
 * Create a ping-pong ball structure and place it in the center of the
 * table with a random direction and speed
 */
struct ball *create_ball( WINDOW *table )
{
    struct ball *p;

    p = malloc(sizeof(struct ball));    //TODO Free memory
    p->sym = '0';
    center_ball(table, p);

    return p;
}

/*
 * TODO
 */
void center_ball( WINDOW *table, struct ball *ppb )
{
    ppb->x = COLS / 2;
    ppb->y = LINES / 2;
    ppb->yd = ppb->xd = -1; //TODO Assign random number
    ppb->yc = ppb->yn = 1;  //TODO Assign random number
    ppb->xc = ppb->xn = 3;  //TODO Assign random number
    mvwaddch(table, ppb->y, ppb->x, '0');
    wmove(table, 0, 0);
    wrefresh(table);
}

/*
 * Increment the position of the input ball structure. Return 0 if
 * the ball will pass the paddle, otherwise return 1.
 */
int move_ball( WINDOW *table, struct ball *ppb, struct bar *paddle )
//TODO Separate the logic into functions
{
    mvwaddch(table, ppb->y, ppb->x, ' ');   // Remove ball

    /* Switch direction if at bottom side */
    if ( (ppb->y + ppb->yd) == table->_maxy )
        ppb->yd = -1;
    /* Switch direction if at top side */
    if ( (ppb->y + ppb->yd) == table->_begy )
        ppb->yd = 1;
    /* Move the ball along y */
    if ( ppb->yc > 0 ) {	// If NOT at next y coordinate
        ppb->y += ppb->yd;	// move along y
        ppb->yc--;			// record move
    }

    /* Switch direction if at paddle */
    if ( (ppb->x + ppb->xd) == table->_maxx ) { // Check for paddle
        if ( pong(paddle, ppb->y + ppb->yd) == 0 ) {
            return 0;
        }
        //TODO Assign random number to yc, yn, xc, xn, yd, xd
        ppb->xd = -1;
        }
    /* Switch direction if at left side */
    if ( (ppb->x + ppb->xd) == table->_begx )
        ppb->xd = 1;
    /* Move the ball along x */
    if ( ppb->xc > 0 ) {	// If NOT at next x coordinate
        ppb->x += ppb->xd;	// move along x
        ppb->xc--;			// record move
    }
    /* Keep going if the ball has reached the next point */
    if ( ppb->yc == 0 && ppb->xc == 0 ) {
        ppb->yc = ppb->yn;	// Assign new point
        ppb->xc = ppb->xn;
        move_ball(table, ppb, paddle);
    }
    mvwaddch(table, ppb->y, ppb->x, ppb->sym);
    wrefresh(table);
    return 1;
}

int pong ( struct bar *paddle, int x )
{
    if ( paddle->lower < x && x < paddle->upper )
        return 1;
    else
        return 0;
}
