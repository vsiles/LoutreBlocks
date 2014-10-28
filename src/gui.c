/**********************************************************************
 *                            denki_blocks                            *
 **********************************************************************
 *                                                                    *
 *gui.c:                                                              *
 *           Gère l'affichage de la fenêtre principale et les         *
 *   évènements qui y sont associés.                                  *
 *                                                                    *
 *                                                                    *
 *********************************************************************/


#define _GUI_

/***        Librairies        ***/
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "main.h"
#include "game.h"
#include "gfx.h"
#include "gui.h"
/***        /Librairies       ***/
/***        Définitions       ***/
/***       /Définitions       ***/
/***    Variables globales    ***/
#include "variables.h"
GtkWidget *main_win;
GtkWidget *label;
GtkWidget *button;
time_t start,current;
/***   /Variables globales    ***/

gint init_ext(GtkWidget * widget, gpointer data)
{
  GdkGLContext * contexte = gtk_widget_get_gl_context(GTK_WIDGET(widget));
  GdkGLDrawable * surface = gtk_widget_get_gl_drawable(GTK_WIDGET(widget));

  if(gdk_gl_drawable_gl_begin(surface,contexte)){
    init_gfx();
    gdk_gl_drawable_gl_end(surface);
  }
  else
    return FALSE;

  return TRUE;
}

gint reshape_ext(GtkWidget * widget, GdkEventConfigure * ev, gpointer data)
{
  GdkGLContext * contexte = gtk_widget_get_gl_context(GTK_WIDGET(widget));
  GdkGLDrawable * surface = gtk_widget_get_gl_drawable(GTK_WIDGET(widget));

  if(gdk_gl_drawable_gl_begin(surface,contexte)){
    reshape_gfx(ev->width,ev->height);
    gdk_gl_drawable_gl_end(surface);
  }
  else
    return FALSE;
  
  return TRUE;
}

gint draw_ext(GtkWidget * widget, gpointer data)
{
  GdkGLContext * contexte = gtk_widget_get_gl_context(GTK_WIDGET(widget));
  GdkGLDrawable * surface = gtk_widget_get_gl_drawable(GTK_WIDGET(widget));

  if(gdk_gl_drawable_gl_begin(surface,contexte)){
    draw_gfx();
    gdk_gl_drawable_swap_buffers(surface);
    gdk_gl_drawable_gl_end(surface);
  }
  else
    return FALSE;
  return TRUE;
}


GtkWidget * CreateGL_GtkGlExt()
{
  GtkWidget * widget_gl;
  GdkGLConfig * glconfig;
  static const gint attrList[] = {
    GDK_GL_DOUBLEBUFFER,
    GDK_GL_DEPTH_SIZE,     16,
    GDK_GL_RGBA,
    GDK_GL_RED_SIZE,        1,
    GDK_GL_GREEN_SIZE,      1,
    GDK_GL_BLUE_SIZE,       1,
    GDK_GL_ATTRIB_LIST_NONE };

  glconfig = gdk_gl_config_new(attrList);

  widget_gl = gtk_drawing_area_new();

  gtk_widget_set_gl_capability(GTK_WIDGET(widget_gl),glconfig,NULL,TRUE,GDK_GL_RGBA_TYPE);

  g_signal_connect(G_OBJECT(widget_gl),"realize",G_CALLBACK(init_ext),NULL);
  g_signal_connect(G_OBJECT(widget_gl),"configure_event",G_CALLBACK(reshape_ext),NULL);
  g_signal_connect(G_OBJECT(widget_gl),"expose_event",G_CALLBACK(draw_ext),NULL);

  return widget_gl;
}


void splash_key(GtkWidget *widget, GdkEventKey *event)
{
  gtk_widget_destroy(widget);
}

void map_key( GtkWidget *widget, GdkEventKey *event)
{
  switch(event->keyval){
  case GDK_Escape:
    gtk_main_quit();
    break;
  case GDK_Up:
    rotX -= 1;
    break;
  case GDK_Down:
    rotX += 1;
    break;
  case GDK_Left:
    rotY += 1;
    break;
  case GDK_Right:
    rotY -= 1.0f;
    break;
  case GDK_Page_Up:
    Z -= 0.5f;
    break;
  case GDK_Page_Down:
    Z += 0.5f;
    break;
  case GDK_Home:
    Z = 16.0f;
    rotX = rotY = 0.0f;
    break;
  case GDK_j:
    move( WEST );
    break;
  case GDK_l:
    move( EAST );
    break;
  case GDK_i:
    move( NORTH );
    break;
  case GDK_k:
    move( SOUTH );
    break;
  }

  return ;
}

void window_info(gchar *msg)
{
  GtkWidget* win;
  GtkWidget* lbl;

  win = gtk_dialog_new_with_buttons("Héhéhé",
				    GTK_WINDOW(main_win),
				    GTK_DIALOG_MODAL,
				    GTK_STOCK_OK,GTK_RESPONSE_OK,
				    NULL);

    lbl = gtk_label_new(msg);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(win)->vbox), lbl, TRUE, FALSE, 0);

    gtk_widget_show_all(GTK_DIALOG(win)->vbox);

    switch (gtk_dialog_run(GTK_DIALOG(win)))
      {
      case GTK_RESPONSE_OK:
      default:
	break;
      }

    gtk_widget_destroy(win);
}


void start_gui( void )
{
  GtkWidget *win,*statusbar;

  win = gtk_window_new(GTK_WINDOW_POPUP);
  gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
  gtk_window_set_modal(GTK_WINDOW(win), TRUE);
  gtk_window_set_type_hint (GTK_WINDOW (win),     GDK_WINDOW_TYPE_HINT_DIALOG);

  GtkWidget *box=gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(win), box);

  GtkWidget *widget_image=gtk_image_new_from_file("splash.png");
  gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(widget_image), FALSE, FALSE, 0);

  //status bar
  statusbar=gtk_statusbar_new();
  gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(statusbar), FALSE);
  gtk_statusbar_push (GTK_STATUSBAR(statusbar),0,"Welcome to Denki Blocks!!!!! Press a Key");
  gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(statusbar), FALSE, FALSE, 0);

  gtk_widget_set_events(GTK_WIDGET(win),GDK_KEY_PRESS_MASK);
  g_signal_connect(G_OBJECT(win),"key_press_event",G_CALLBACK(splash_key),NULL);

  gtk_widget_show_all(GTK_WIDGET(win));
  gtk_widget_realize(GTK_WIDGET(win));

  gtk_main();
}

void destroy( GtkWidget *widget, gpointer data )
{
   gtk_main_quit();
}

gboolean delete_event( GtkWidget *widget, GdkEventKey *event, gpointer data )
{
  return FALSE;
}

gboolean idling(gpointer data)
{
  char msg[1024];//jamais trop prudent
  idle_gfx();
  gtk_widget_queue_draw(GTK_WIDGET(data));
  if(running){
    current = time(NULL);
    sprintf(msg,"Niveau: %i  -  Temps: %i  -  Score: #",level,(int)(current-start));
    gtk_label_set_text(GTK_LABEL(label),msg);
  }
  return TRUE;
}

void prepare_gui( int new_level )
{
  char msg[1024];
  if(new_level)
    window_info("\n\t We've got a Winner!!!\t\n");
  running = 0;
  sprintf(msg,"Niveau: %i - Temps: 0 - Score: #",level);
  gtk_label_set_text(GTK_LABEL(label),msg);
  gtk_button_set_label(GTK_BUTTON(button),"Start!!!");
}

gboolean BoutonClick(GtkWidget *btn, gpointer data)
{
  int c;

  start = current = time(NULL);
  if(running){
    /* recommencer le niveau */
    window_info("\n\tToo bad :( So Sad :'(\t\n");
    prepare_gui(0);
    if( (c = restart_game()) < 0 )
      return c;
  }
  else{
    window_info("\n\tC'est parti!!\t\n");
    gtk_button_set_label(GTK_BUTTON(btn),"Recommencer");
    running = 1;
    /* commencer le niveau */
  }
  return TRUE;
}

void end_gui( void )
{

}

void init_gui( int *argc, char *argv[] )
     /*
      * Crée la fenêtre principale
      */
{
  GtkWidget *gl;
  GtkWidget *vbox,*vbox2;
  GtkWidget *menu;

  gtk_init( argc , &argv );

  main_win = gtk_window_new( GTK_WINDOW_TOPLEVEL );

  gtk_window_set_title( GTK_WINDOW( main_win ), "Loutre Blocks" );
  g_signal_connect( G_OBJECT( main_win ), "destroy", G_CALLBACK( destroy ), NULL );
  g_signal_connect( G_OBJECT( main_win ), "delete_event", G_CALLBACK( delete_event ), NULL );

  gtk_widget_set_events(GTK_WIDGET(main_win),GDK_KEY_PRESS_MASK);
  g_signal_connect(G_OBJECT(main_win),"key_press_event",G_CALLBACK(map_key),NULL);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(main_win),vbox);

  gl = CreateGL_GtkGlExt(label);

  gtk_box_pack_start(GTK_BOX(vbox), gl , TRUE, TRUE, 0);

  menu = gtk_frame_new("Menu:");
  gtk_frame_set_shadow_type(GTK_FRAME(menu), GTK_SHADOW_IN);
  gtk_box_pack_start(GTK_BOX(vbox), menu , FALSE, TRUE, 0);

  vbox2 = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(menu),vbox2);

  label = gtk_label_new("Niveau: 0  -  Temps: 0  -  Score: 0");
  gtk_box_pack_start(GTK_BOX(vbox2),label,FALSE,TRUE,0);
  button = gtk_button_new_with_label("Start!!!");
  gtk_box_pack_start(GTK_BOX(vbox2),button,FALSE,TRUE,0);
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(BoutonClick),NULL);

  gtk_window_set_default_size(GTK_WINDOW(main_win),640,480);
  gtk_window_set_position(GTK_WINDOW(main_win),GTK_WIN_POS_CENTER);

  gtk_idle_add(idling,(gpointer)gl);

  gtk_widget_show_all( main_win );
}
