/*
 * paddle.h - Paddle structure and function declarations
 */

#ifndef PADDLE
#define PADDLE
struct bar { int upper; int lower; int len; };
struct bar *create_bar( WINDOW *, int );
void place_bar( WINDOW *, struct bar * );
int move_bar( WINDOW *, struct bar *, int );
#endif

