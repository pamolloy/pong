/*
 * main.h - TODO
 */

#ifndef MAIN
#define MAIN
void add_tick( int );
WINDOW *set_table( int height, int width, int y, int x );
WINDOW *set_scoreboard( int height, int width, int y, int x );
WINDOW *set_endzone( int height, int width, int y, int x );
void setup_signal();
void update_board ( WINDOW *board, int lives, int time );
#endif

