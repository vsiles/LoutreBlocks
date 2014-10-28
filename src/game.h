#define NORTH 0
#define SOUTH 1
#define EAST  2
#define WEST  3

int init_game( int *argc, char *argv[] );
void end_game( void );
void move( int direction );
void print_clump_list( void );
void print_board( void );
void end_game( void );
int restart_game( void );
int next_level( void );

