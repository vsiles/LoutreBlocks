/**********************************************************************
 *                            denki_blocks                            *
 **********************************************************************
 *                                                                    *
 *denki.h:                                                            *
 *           D�finitions globales pour le programme.                  *
 *                                                                    *
 *                                                                    *
 *********************************************************************/

/*
 * Un peu de tout
 */

#define MIN(X,Y) ((X)>(Y) ? (Y):(X))
#define MAX(X,Y) ((X)<(Y) ? (Y):(X))


/*
 * Les diff�rents types d'objets possibles
 */

#define FLOOR   '.'
#define WALL    '#'
#define BOULDER '%'
#define RED     '1'
#define GREEN   '2'
#define BLUE    '3'

/*
 * Les erreurs
 */

#define BAD_FILE_FORMAT -1
#define NO_SUCH_FILE    -2
