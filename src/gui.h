#ifndef __GUI_H__
#define __GUI_H__

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <gdk/gdkkeysyms.h>


void init_gui( int *argc, char *argv[] );
void end_gui( void );
void start_gui( void );

void prepare_gui( int new_level);
#endif//__GUI_H__
