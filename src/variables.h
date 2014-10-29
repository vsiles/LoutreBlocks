#ifndef _VARIABLES_H_
#define _VARIABLES_H_

/* 
 * Nasty macro to distinguish declaration from actual allocation
 * of non-static global variables
 */
#ifdef _MAIN_
#define __EXTERN 
#else
#define __EXTERN extern
#endif

__EXTERN int level;
/* gather_color[i]==1 iff the gumblocks of color [i] have to be gathered together */
__EXTERN int gather_color[9]; 
/* running != 0 iff the game has already started */
__EXTERN int running;         
/* Logical view of the board */
__EXTERN unsigned char board[18][18];
/* global rotation/translation value to setup the 3D view */
__EXTERN float rotX,rotY,Z;

#endif /* _VARIABLES_H_ */
