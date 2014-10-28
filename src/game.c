/**********************************************************************
 *                            denki_blocks                            *
 **********************************************************************
 *                                                                    *
 *game.c:                                                             *
 *           Gestion du jeu proprement dit.                           *
 *                                                                    *
 *                                                                    *
 *********************************************************************/


#define _GAME_

/***        Librairies        ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "denki.h"
#include "main.h"
#include "game.h"
#include "files.h"
#include "gui.h"
/***        /Librairies       ***/
/***        Définitions       ***/
struct clump {
  int type;          /* Voir le fichier denki.h                                       */

  int w,h;           /* Largeur et hauteur du plus petit rectangle englobant le clump */
  short int *shape;  /* Matrice décrivant le clump                                    */

  int x,y;           /* Position du coin supérieur gauche du clump sur le plateau     */

  struct clump *next;
  struct clump *prev;

  short int moved;
};

#define LEVELS 6
/***       /Définitions       ***/
/***    Variables globales    ***/
#include "variables.h"

struct clump *whom[18][18];

struct clump *clump_list=NULL;
int clumps[9];
/***   /Variables globales    ***/

void print_board( void )
{
  /*  int i,j;

  for(i=1 ; i <= 16 ; i++)
    {
      for(j=1 ; j <= 16 ; j++)
	printf("%c", board[i][j] );

      printf("\n");
    }
  */
}

void print_clump( struct clump *c )
{
  int i,j;
  int w,h;
  short int *shape;

  w = c->w;
  h = c->h;
  shape = c->shape;

  printf("y0 = %d ; x0 = %d\n   type = %c\n ", c->y, c->x, c->type );
  for( i = 0 ; i < h ; i++ )
    {
      for( j = 0 ; j < w ; j++ )
	printf("%d", shape[ i*w + j ]);

      printf("\n ");
    }

  printf("\r");
}

void free_clump( struct clump *c )
{
  free( c->shape );
  free( c );
}

void fusionne( struct clump *c, struct clump *d )
{
  int i,j;
  int x,y,w,h;
  int cx,cy,cw,ch;
  int dx,dy,dw,dh;
  short int *shape,*cshape,*dshape;

  cx = c->x; cy = c->y; cw = c->w; ch = c->h; cshape = c->shape;
  dx = d->x; dy = d->y; dw = d->w; dh = d->h; dshape = d->shape;

  x = MIN( cx, dx );
  y = MIN( cy, dy );
  w = MAX( cx + cw - x, dx + dw - x );
  h = MAX( cy + ch - y, dy + dh - y );

  shape = (short int *)calloc( h, w*(sizeof( short int )));

  for( i = 0 ; i < h ; i++ )
    for( j = 0 ; j < w ; j++ )
      shape[i*w + j] = 0;

  for( i = 0 ; i < ch ; i++ )
    for( j = 0 ; j < cw ; j++ )
      shape[(i+cy-y)*w + j+cx-x ] = cshape[i*cw + j];

  for(  i = 0 ; i < dh ; i++ )
    for( j = 0 ; j < dw ; j++ )
      if( dshape[i*dw + j] )
	{
	  shape[(i+dy-y)*w + j+dx-x] = 1;
	  whom[i+dy+1][j+dx+1] = c;
	}

  free( cshape );
  c->shape = shape;
  c->x = x; c->y = y;
  c->w = w; c->h = h;

  if( d->prev )
    d->prev->next = d->next;
  else
    clump_list = d->next;

  if( d->next )
    d->next->prev = d->prev;

  free_clump( d );

  clumps[c->type - '1']--;
}

int move_north( struct clump *l )
{
  struct clump *k;
  int moves=1;
  int i,j;
  int x,y,w,h;
  short int *shape;

  if( !l )
    return 1;

  x = l->x; y = l->y;
  w = l->w; h = l->h;
  shape = l->shape;

  if( y > 0 && !l->moved)
    {
      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if( (shape[i*w + j]) && board[i+y][j+x+1] == WALL )
	    {
	      l->moved = 1;
	      return 0;
	    }

      /*On fait comme si on avait réussi à bouger la pièce */
      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if(shape[i*w + j])
	    {
	      whom[y+i+1][x+j+1] = NULL;
	      board[y+i+1][x+j+1] = FLOOR;
	    }

      y--;
      l->y = y;

      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if( (shape[i*w + j]) && (k = whom[y+i+1][x+j+1]) )
	    moves &= move_north( k );

      if( !moves )
	l->y = ++y;

      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if(shape[i*w + j])
	    {
	      whom[y+i+1][x+j+1] = l;
	      board[y+i+1][x+j+1] = l->type;
	    }

      l->moved = 1;

      return moves;
    }
  else
    return 0;
}

int move_south( struct clump *l )
{
  struct clump *k;
  int moves=1;
  int i,j;
  int x,y,w,h;
  short int *shape;

  if( !l )
    return 1;

  x = l->x; y = l->y;
  w = l->w; h = l->h;
  shape = l->shape;

  if( y < 15 && !l->moved)
    {
      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if( (shape[i*w + j]) && board[i+y+2][j+x+1] == WALL )
	    {
	      l->moved = 1;
	      return 0;
	    }

      /*On fait comme si on avait réussi à bouger la pièce */
      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if(shape[i*w + j])
	    {
	      whom[y+i+1][x+j+1] = NULL;
	      board[y+i+1][x+j+1] = FLOOR;
	    }

      y++;
      l->y = y;

      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if( (shape[i*w + j]) && (k = whom[y+i+1][x+j+1]) )
	    moves &= move_south( k );

      if( !moves )
	l->y = --y;

      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if(shape[i*w + j])
	    {
	      whom[y+i+1][x+j+1] = l;
	      board[y+i+1][x+j+1] = l->type;
	    }

      l->moved = 1;

      return moves;
    }
  else
    return 0;
}

int move_east( struct clump *l )
{
  struct clump *k;
  int moves=1;
  int i,j;
  int x,y,w,h;
  short int *shape;

  if( !l )
    return 1;

  x = l->x; y = l->y;
  w = l->w; h = l->h;
  shape = l->shape;

  if( x < 15 && !l->moved)
    {
      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if( (shape[i*w + j]) && board[i+y+1][j+x+2] == WALL )
	    {
	      l->moved = 1;
	      return 0;
	    }

      /*On fait comme si on avait réussi à bouger la pièce */
      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if(shape[i*w + j])
	    {
	      whom[y+i+1][x+j+1] = NULL;
	      board[y+i+1][x+j+1] = FLOOR;
	    }

      x++;
      l->x = x;

      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if( (shape[i*w + j]) && (k = whom[y+i+1][x+j+1]) )
	    moves &= move_east( k );

      if( !moves )
	l->x = --x;

      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if(shape[i*w + j])
	    {
	      whom[y+i+1][x+j+1] = l;
	      board[y+i+1][x+j+1] = l->type;
	    }

      l->moved = 1;

      return moves;
    }
  else
    return 0;
}

int move_west( struct clump *l )
{
  struct clump *k;
  int moves=1;
  int i,j;
  int x,y,w,h;
  short int *shape;

  if( !l )
    return 1;

  x = l->x; y = l->y;
  w = l->w; h = l->h;
  shape = l->shape;

  if( x > 0 && !l->moved)
    {
      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if( (shape[i*w + j]) && board[i+y+1][j+x] == WALL )
	    {
	      l->moved = 1;
	      return 0;
	    }

      /*On fait comme si on avait réussi à bouger la pièce */
      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if(shape[i*w + j])
	    {
	      whom[y+i+1][x+j+1] = NULL;
	      board[y+i+1][x+j+1] = FLOOR;
	    }

      x--;
      l->x = x;

      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if( (shape[i*w + j]) && (k = whom[y+i+1][x+j+1]) )
	    moves &= move_west( k );

      if( !moves )
	l->x = ++x;

      for( i = 0 ; i < h ; i++ )
	for( j = 0 ; j < w ; j++ )
	  if(shape[i*w + j])
	    {
	      whom[y+i+1][x+j+1] = l;
	      board[y+i+1][x+j+1] = l->type;
	    }

      l->moved = 1;

      return moves;
    }
  else
    return 0;
}

void move( int direction )
{
  if( !running )
    return;

  struct clump *l;
  int (*move) (struct clump *);

  switch( direction )
    {
    case NORTH:
      move = move_north;
      break;
    case SOUTH:
      move = move_south;
      break;
    case EAST:
      move = move_east;
      break;
    case WEST:
      move = move_west;
      break;
    }

  l = clump_list;
  while( l )
    {
      l->moved = 0;

      l = l->next;
    }

  l = clump_list;
  while( l )
    {
      move( l );

      l = l-> next;
    }

  /* On regarde maintenant si on doit fusionner des clumps */
  int i,j;
  struct clump *c,*d;

  for( i = 1 ; i <= 16 ; i++ )
    for( j = i%2+1 ; j <= 16 ; j+=2 )
      if( (c = whom[i][j]) != NULL )
	{
	  if( (c != (d = whom[i-1][j])) && (board[i][j] == board[i-1][j]) )
	    fusionne( c, d );
	  if( (c != (d = whom[i+1][j])) && (board[i][j] == board[i+1][j]) )
	    fusionne( c, d );
	  if( (c != (d = whom[i][j-1])) && (board[i][j] == board[i][j-1]) )
	    fusionne( c, d );
	  if( (c != (d = whom[i][j+1])) && (board[i][j] == board[i][j+1]) )
	    fusionne( c, d );
	}

  print_board();

  /* Enfin on vérifié que le niveau n'est pas terminé */
  for( i = 0 ; i < 9 ; i++ )
    if( (gather_color[i]) && clumps[i] != 1 )
      break;

  if( i == 9 )
    {
      running = 0;
      next_level();
    }
}

void print_clump_list( void )
{
  struct clump *l;

  l = clump_list;
  while( l )
    {
      print_clump( l );

      l = l->next;
    }
}

void _free_clump_list( struct clump *l )
{
  if( l )
    {
      _free_clump_list( l->next );
      free_clump( l );
    }
}

void free_clump_list( void )
{
  _free_clump_list( clump_list );
}

void get_clump( color, i, j, rect, clump )
     int color;
     int i,j,rect[4];
     short int clump[16][16];
{
  if( j > 0 )
    if( board[i+1][j] == color && !clump[i][j-1] )
      {
	clump[i][j-1] = 1;
	rect[0] = MIN(rect[0],j-1);
	get_clump( color, i, j-1, rect, clump );
      }

  if( i > 0 )
    if( board[i][j+1] == color && !clump[i-1][j] )
      {
	clump[i-1][j] = 1;
	rect[1] = MIN(rect[1],i-1);
	get_clump( color, i-1, j, rect, clump );
      }

  if( j < 15 )
    if( board[i+1][j+2] == color && !clump[i][j+1] )
      {
	clump[i][j+1] = 1;
	rect[2] = MAX(rect[2],j+1);
	get_clump( color, i, j+1, rect, clump );
      }

  if( i < 15 )
    if( board[i+2][j+1] == color && !clump[i+1][j] )
      {
	clump[i+1][j] = 1;
	rect[3] = MAX(rect[3],i+1);
	get_clump( color, i+1, j, rect, clump );
      }
}

void build_clump_list( void )
{
  int i,j,k,l;
  unsigned char color;
  short int done[16][16], clump[16][16];
  int rect[4];
  struct clump *new_clump;
  int x,y,w,h;
  short int *shape;

  free_clump_list();
  for( i = 0 ; i < 16 ; i++ )
    for( j = 0 ; j < 16 ; j++ )
      done[i][j] = 0;

  for( i=0 ; i < 16 ; i++ )
    for( j=0 ; j < 16 ; j++ )
      {
	color = board[i+1][j+1];
	if( color >= '1' && color <= '9' && !done[i][j] )
	  {
	    for( k = 0 ; k < 16 ; k++ )
	      for( l = 0 ; l < 16 ; l++ )
		clump[k][l] = 0;

	    clump[i][j] = 1;

	    rect[0]=j;rect[2]=j;
	    rect[1]=i;rect[3]=i;
	    get_clump( color, i, j, rect, clump );

	    done[i][j] = 1;
	    if( gather_color[color - '1'] )
	      clumps[color - '1']++;

	    x = rect[0];
	    y = rect[1];
	    w = rect[2] - x +1;
	    h = rect[3] - y +1;

	    shape = (short int *)calloc( h, w*(sizeof( short int )));

	    new_clump = (struct clump *)malloc( sizeof( struct clump ) );
	    new_clump->type  = color;
	    new_clump->x     = x;
	    new_clump->y     = y;
	    new_clump->w     = w;
	    new_clump->h     = h;
	    new_clump->shape = shape;

	    for( k = 0 ; k < h ; k++ )
	      for( l = 0 ; l < w ; l++ )
		{
		  if( clump[y + k][x + l] )
		    {
		      shape[ k*w + l ] = 1;
		      whom[y + k+1][x + l+1] = new_clump;
		      done[y + k][x + l] = 1;
		    }
		  else
		    shape[ k*w + l ] = 0;
		}

	    new_clump->next = clump_list;
	    if( clump_list )
	      clump_list->prev = new_clump;
	    new_clump->prev = NULL;
	    clump_list = new_clump;
	  }
      }
}

int init_game( int *argc, char *argv[] )
{
  int i,j;

  for( i = 0 ; i < 9 ; i++ )
      clumps[i] = 0;

  clump_list = NULL;

  for( i = 0 ; i < 18 ; i++ )
    for( j = 0 ; j < 18 ; j++ )
      whom[i][j] = NULL;

  build_clump_list();
  return 0;
}

void end_game( void )
{
  free_clump_list();
}

int restart_game( void )
{
  int c;

  end_game();
  c = get_board( NULL );
  if( c<0 )
    return c;
  init_game( NULL, NULL );
  return 0;
}

int next_level( void )
{
  if( level == -1 )
    end();

  char *s;
  int c;
  
  s = (char *)malloc(sizeof("level999.lvl") +1);/*Pas plus de 999 niveaux quand même ! */
  end_game();
  level++;
  if( level <= LEVELS )
    sprintf( s, "level%d.lvl", level );
  else
    strcpy( s, "level1.lvl" );
  prepare_gui(1);
  c = get_board( s );
  if( c < 0 )
    {
      get_board( NULL );
      init_game( NULL, NULL );

      return c;
    }

  init_game( NULL, NULL );
  return 0;
}
