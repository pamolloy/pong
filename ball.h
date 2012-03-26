/*
 * ball.h - TODO
 */

#include "paddle.h"

#ifndef BALL
#define BALL
struct ball {
	int		y,	x,
			yd, xd,	// Increment direction
			yc, xc,	// Count to position
			yn,	xn;	// Increment angle
	char	sym;
};
struct ball *create_ball( WINDOW * );
void center_ball( WINDOW *, struct ball * );
int move_ball( WINDOW *, struct ball *, struct bar * );
int pong ( struct bar *, int );
#endif

