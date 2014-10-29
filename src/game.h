#ifndef _GAME_H_
#define _GAME_H_

/*
 * Movement directions
 */
#define NORTH 0
#define SOUTH 1
#define EAST  2
#define WEST  3

/*
 * Main game entry points:
 * - start / stop a game
 * - move all blocks on the map
 * - change level
 * - print some infos
 */
int init_game( int *argc, char *argv[] );
void end_game( void );
void move( int direction );
void print_clump_list( void );
void print_board( void );
int restart_game( void );
int next_level( void );

#endif /* _GAME_H_ */

