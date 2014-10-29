/**********************************************************************
 *                            denki_blocks                            *
 **********************************************************************
 *                                                                    *
 *denki.h:                                                            *
 *           Global macros & constants                                *
 *                                                                    *
 *                                                                    *
 *********************************************************************/

#ifndef _DENKI_H_
#define _DENKI_H_

/*
 * Various macros.
 */
#define MIN(X,Y) ((X)>(Y) ? (Y):(X))
#define MAX(X,Y) ((X)<(Y) ? (Y):(X))


/*
 * Object kind that can be displayed in maps.
 */

#define FLOOR   '.'
#define WALL    '#'
#define BOULDER '%'
#define RED     '1'
#define GREEN   '2'
#define BLUE    '3'

/*
 * Error codes.
 */

#define BAD_FILE_FORMAT -1
#define NO_SUCH_FILE    -2

#endif /* _DENKI_H_ */
