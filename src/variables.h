#ifdef _MAIN_
#define __EXTERN 
#else
#define __EXTERN extern
#endif

__EXTERN int level;
__EXTERN int gather_color[9]; /* gather_color[i]==1 si les gumblocks de couleur i doivent être rassemblés */
__EXTERN int running;         /* running != 0 si le jeu a commencé */
__EXTERN unsigned char board[18][18];
__EXTERN float rotX,rotY,Z;
