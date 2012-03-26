/*
 * main.c - Main control flow of the program
 */

#include <stdlib.h> // exit()
#include <curses.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sys/times.h>
#include <stdint.h>
#include <unistd.h> // getpid()

#include "main.h"
#include "paddle.h"
#include "ball.h"
#include "alarm.h"

#define MARGIN  3
#define LIFE_TOTAL   3

void start();
void end();
int ticks = 0;	// Number of alarm signals

int main()
{
    WINDOW *table;
    WINDOW *board;
    WINDOW *endzone;
    struct bar *paddle;
    struct ball *ppb;
    struct tms st_time;
    int lives = LIFE_TOTAL;

    start();

    /* Create a court and place a ball on it */
    table = set_table(LINES - (MARGIN * 2), COLS - (MARGIN * 2), MARGIN, MARGIN);
    ppb = create_ball(table);

    /* Create an endzone and place a paddle on it */
    endzone = set_endzone(LINES - (MARGIN * 2), 1, MARGIN - 1, COLS - MARGIN - 1);
    paddle = create_bar(endzone, LINES / 4);

    /* Create a scoreboard */
    board = set_scoreboard(1, COLS - (MARGIN * 2), MARGIN - 1, MARGIN);

    setup_signal();

    if ( times(&st_time) == -1 ) {  // Record game start time
        perror("times");
        exit(EXIT_FAILURE);
    }

    int c;
    while( (c = wgetch(table)) != 'Q' ) {
        if (c == ERR && errno != EINTR)
            break;	//TODO This is a real error
        if (c == KEY_UP)
            move_bar(endzone, paddle, 1);
        if (c == KEY_DOWN)
            move_bar(endzone, paddle, -1);
        while ( ticks > 0 ) {
            if ( move_ball(table, ppb, paddle) == 0 ) {
                if ( lives-- > 0 ) {
                    center_ball(table, ppb);
                    ticks = 0;
                }
                if ( lives == 0 ) {
                    end();
                    printf("You lost after %i attempts.\n", LIFE_TOTAL);
                    exit(EXIT_FAILURE);
                }
            }
            update_board(board, lives, st_time.tms_utime);
            ticks--;
        }
    }

    end();

    exit(EXIT_SUCCESS);
}

/*
 * Update the score board with the current number of lives and time
 * elapsed
 */
void update_board ( WINDOW *board, int lives, int time )
//TODO The clock_t values aren't displayed
//TODO Calculate time with sysconf(_SC_CLK_TCK)
{
    struct tms en_time;

    mvwprintw(board, 0, 0, "LIVES: %i", lives);
    times(&en_time);        // Current time
    mvwprintw(board, 0, COLS / 2, "Time: %j",
                (intmax_t)(en_time.tms_utime - time));
    wrefresh(board);
}

/*
 * Create a window for the pong court with a boarder
 */
WINDOW *set_table( int height, int width, int y, int x )
{
    WINDOW *table;
    /* Create window with margin */
    table = newwin(height, width, y, x);
    keypad(table, TRUE);                // Single value for fn keys
    wborder(table, ACS_VLINE, ' ', ACS_HLINE, ACS_HLINE,
            ACS_ULCORNER, ACS_HLINE, ACS_LLCORNER, ACS_HLINE);
    wrefresh(table);
    return table;
}

/*
 * Create a window for the scoreboard
 */
WINDOW *set_scoreboard( int height, int width, int y, int x )
{
    WINDOW *board;
    /* Create window with margin */
    board = newwin(height, width, y, x);
    wrefresh(board);
    return board;
}

/*
 * Create a window for the paddle
 */
WINDOW *set_endzone( int height, int width, int y, int x )
{
    WINDOW *ez;
    /* Create window with margin */
    ez = newwin(height, width, y, x);
    wrefresh(ez);
    return ez;
}

/*
 * Use sigaction to catch the interrupt signal and explicitly disable
 * restart, which will cause the curses get character function to
 * return an error
 */
void setup_signal()
{
    struct sigaction siginfo;
    
    signal(SIGINT, SIG_IGN);	    // Ignore interrupts during game
    siginfo.sa_flags = 0;	        // All off, esp. SA_RESTART
    siginfo.sa_handler = add_tick;
    sigemptyset(&siginfo.sa_mask);
    sigaction(SIGALRM, &siginfo, NULL);
    set_ticker( 1000 / 50 );        // Set ball speed
}

/*
 * Increment a global variable every time a interrupt signal is caught
 */
void add_tick( int signal )
{
	ticks++;
}

/*
 * Initialize the ncurses library
 */
void start()
{
    if ( initscr() == NULL ) {
        fprintf(stderr, "initscr: failed to initialize curses");
        exit(EXIT_FAILURE);
    }
    noecho();               // Disable echo of getch
    cbreak();               // Disable line buffering
    nonl();                 // Disable new line
    intrflush(NULL, FALSE); // Prevent flush on keyboard interrupt
    if ( curs_set(0) == ERR ) {
        fprintf(stderr, "curs_set: failed to set cursor to invisible");
        exit(EXIT_FAILURE);
    }
    srand(getpid());    // Initialize random number generator
}

/*
 * Restore tty modes, move cursor to lower left-hand corner, reset
 * terminal into non-visual mode
 */
void end()
{
    if ( endwin() == ERR ) {
        fprintf(stderr, "endwin: failed to restore terminal settings");
        exit(EXIT_FAILURE);
    }
}

