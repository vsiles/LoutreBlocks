#ifndef _GUI_H_
#define _GUI_H_

/* The GUI is GTK based */
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <gdk/gdkkeysyms.h>


void init_gui( int *argc, char *argv[] );
void end_gui( void );
void start_gui( void );

void prepare_gui( int new_level);

#endif /* _GUI_H_ */
