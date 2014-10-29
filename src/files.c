/**********************************************************************
 *                            denki_blocks                            *
 **********************************************************************
 *                                                                    *
 *files.c:                                                            *
 *                                                                    *
 *                                                                    *
 *********************************************************************/


/***        Libraries         ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "denki.h"
#include "files.h"
/***        /Libraries        ***/
/***        Definitions       ***/
#define DEFAULT_LEVEL "level1.lvl"
/***       /Definitions       ***/
/***    Global variables      ***/
#include "variables.h"

char *level_file=NULL;
/***   Global variables       ***/

int bad_file_format( void )
{
    puts("Bad file format.");
    return BAD_FILE_FORMAT;
}

int get_board( char *s )
{
    int i,c;
    FILE *f;

    if( s )
    {
        f = fopen( s, "r" );
        if( !f )
        {
            perror( s );
            return NO_SUCH_FILE;
        }

        if( level_file )
            free(level_file);
        level_file = malloc( strlen( s ) );
        strcpy( level_file, s );
    }
    else
    {
        if( !level_file )
        {
            level_file = malloc( sizeof( DEFAULT_LEVEL ) +1 );
            strcpy( level_file, DEFAULT_LEVEL );
        }
        f = fopen( level_file , "r" );
        if( !f )
        {
            perror( level_file );
            return NO_SUCH_FILE;
        }
        level = 1;
    }


    // FIXME: introduce constants for board_size
    for( i=1 ; i<= 17 ; i++ )
        board[0][i] = board[17][i] = board[i][0] = board[i][17] = '#';

    for( i=1 ; i <= 16 ; i++ )
    {
        if( fscanf( f, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
                    &board[i][1], &board[i][2], &board[i][3], &board[i][4],
                    &board[i][5], &board[i][6], &board[i][7], &board[i][8],
                    &board[i][9], &board[i][10], &board[i][11], &board[i][12],
                    &board[i][13], &board[i][14], &board[i][15], &board[i][16] ) != 16 )
            return bad_file_format();
    }

    // FIXME: introduce constants for #colors
    for( i = 0 ; i < 9 ; i++ )
        gather_color[i] = 0;

    while( (c = fgetc( f )) != '\n' )
        if( c >= '1' && c <= '9' )
            gather_color[c - '1'] = 1;
        else if( c != ' ' )
            return bad_file_format();


    fclose( f );
    return 0;
}
