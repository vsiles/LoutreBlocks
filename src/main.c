/**********************************************************************
 *                            denki_blocks                            *
 **********************************************************************
 *                                                                    *
 *main.c:                                                             *
 *           Entrée du programme.                                     *
 *                                                                    *
 *                                                                    *
 *********************************************************************/


/***        Librairies        ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gui.h"
#include "game.h"
#include "files.h"
/***        /Librairies       ***/
/***        Définitions       ***/
#define _MAIN_
/***       /Définitions       ***/
/***    Variables globales    ***/
#include "variables.h"
/***   /Variables globales    ***/

void print_usage( char *name )
{
  printf("Usage :\n   %s [-l level.lvl]\n",name);
}

int parse_args( int *argc, char *argv[] )
{
  int i,j,c;

  for( i = 1 ; i < *argc ; i++ )
    if( strcmp(argv[i],"-l") == 0 && i < *argc -1 )
      {
	level = -1;
	c = get_board( argv[++i] );

	for( j = i +1 ; j < *argc ; j++ )
	  argv[j-2] = argv[j];

	*argc -= 2;
      }

  if( i == *argc )
    c = get_board( NULL );

  if( c < 0 )
    return -1;

  return 0;
}

void end( void )
{
  end_game();
  end_gui();

  exit(0);
}

int main( int argc, char *argv[] )
{
  atexit(end);

  running = 0;
  level = 0;

  init_gui( &argc, argv );

  if( parse_args( &argc, argv ) < 0 )
    exit(0);

  if( init_game( &argc, argv ) < 0 )
    exit(0);

  if( argc > 1 )
    {
      print_usage(argv[0]);
      exit(0);
    }

  print_board();
  start_gui();

  end();
  exit(0);/*Jules : on ne devrait jamais arriver ici mais bon... */
  return 0; /*Vinz : ici encore moins mais restons standards */
}
