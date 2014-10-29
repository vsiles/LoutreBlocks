/**********************************************************************
 *                            denki_blocks                            *
 **********************************************************************
 *                                                                    *
 *gui.c:                                                              *
 *        Deals with graphical event and OpenGL callbacks             *
 *                                                                    *
 *                                                                    *
 *********************************************************************/


/***        Libraries         ***/
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string.h>

#include "denki.h"
#include "gfx.h"
/***        /Libraries       ***/
/***        Définitions        ***/

#define WALL_COLOR 4
static float COLOR[WALL_COLOR +1][3] ={ {1,0,0}, {0,1,0}, {0,0,1}, {0,1,1}, {.734,.68,.195}};
/***       /Dfinitions       ***/
/***    Global variables     ***/
static GLuint id;   /* id de la texture en damier */
/* variable pour faire bouger le plateau */
static GLubyte image[16][16][3];/* pour stocker le damier */
static short int edge[17][17];    /* sert a detecter les aretes */

/***   Global variables      ***/
#include "variables.h"
static void draw_plateau();

void init_gfx(void){
    int i,j,c;

    rotX=-39;
    rotY=0;
    Z=16;
    glClearColor(0,0,0.2f,0.0);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D,id);
    for(i=0;i<16;i++){
        for(j=0;j<16;j++){
            c = (((i&0x4)==0)^((j&0x4)==0));
            if(c){
                image[i][j][0] = 179;
                image[i][j][1] = 127;
                image[i][j][2] = 255;
            }else{
                image[i][j][0] = 204;
                image[i][j][1] = 153;
                image[i][j][2] = 255;
            }
        }
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexImage2D(GL_TEXTURE_2D,0,3,16,16,0,GL_RGB,GL_UNSIGNED_BYTE,&image[0][0][0]);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
}

void reshape_gfx(int width, int height)
{
    if(height == 0)
        height = 1;
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(GLfloat)width/(GLfloat)height,1.0,1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void idle_gfx()
{

}

void draw_gfx()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0,0,Z,0,0,0,0,1,0);
    glRotatef(rotX,1,0,0);
    glRotatef(rotY,0,0,1);
    glTranslatef(-8,8,0);
    glScalef(1,-1,1);
    draw_plateau();
}

void draw_block(int i,int j, float color[3])
{
    float x=0,y=1,z1=0.1f,z2=0.8f;

    glColor3f(color[0],color[1],color[2]);
    glVertex3f(j+x  ,i+x ,z2);
    glVertex3f(j+y  ,i+x ,z2);
    glVertex3f(j+y  ,i+y ,z2);
    glVertex3f(j+x  ,i+y ,z2);

    glVertex3f(j+x  ,i+x  ,z1);
    glVertex3f(j+y,i+x  ,z1);
    glVertex3f(j+y,i+x ,z2);
    glVertex3f(j+x ,i+x ,z2);

    glVertex3f(j+y,i+x ,z1);
    glVertex3f(j+y,i+y,z1);
    glVertex3f(j+y,i+y,z2);
    glVertex3f(j+y,i+x ,z2);

    glVertex3f(j+x ,i+y ,z1);
    glVertex3f(j+y,i+y ,z1);
    glVertex3f(j+y,i+y ,z2);
    glVertex3f(j+x ,i+y ,z2);

    glVertex3f(j+x ,i+x  ,z1);
    glVertex3f(j+x ,i+x  ,z2);
    glVertex3f(j+x ,i+y ,z2);
    glVertex3f(j+x ,i+y ,z1);

}

void draw_line(int mask, int i,int j)
{
    float x=-0.01f,y=1.01f,z1=0.09f,z2=0.81f;
    /* haut */
    if( (mask & (1<<8)) == (1<<8) )
    {
        glVertex3f(j+x  ,i+x  ,z1);
        glVertex3f(j+y,i+x  ,z1);
    }

    if( (mask & (1<<9)) == (1<<9) )
    {
        glVertex3f(j+y,i+x  ,z1);
        glVertex3f(j+y,i+x ,z2);
    }

    if( (mask & (1<<10)) == (1<<10) )
    {
        glVertex3f(j+y,i+x ,z2);
        glVertex3f(j+x ,i+x ,z2);
    }

    if( (mask & (1<<11)) == (1<<11) )
    {
        glVertex3f(j+x ,i+x ,z2);
        glVertex3f(j+x  ,i+x  ,z1);
    }
    /* droite */
    if( (mask & (1<<5)) == (1<<5) )
    {
        glVertex3f(j+y,i+x ,z1);
        glVertex3f(j+y,i+y,z1);
    }

    if( (mask & (1<<1)) == (1<<1) )
    {
        glVertex3f(j+y,i+y,z1);
        glVertex3f(j+y,i+y,z2);
    }

    if( (mask & (1<<6)) == (1<<6) )
    {
        glVertex3f(j+y,i+y,z2);
        glVertex3f(j+y,i+x ,z2);
    }

    if( (mask & (1<<9)) == (1<<9) )
    {
        glVertex3f(j+y,i+x ,z2);
        glVertex3f(j+y,i+x ,z1);
    }
    /* bas */
    if( (mask & (1<<0)) == (1<<0))
    {
        glVertex3f(j+x ,i+y ,z1);
        glVertex3f(j+y,i+y ,z1);
    }

    if( (mask & (1<<1)) == (1<<1))
    {
        glVertex3f(j+y,i+y ,z1);
        glVertex3f(j+y,i+y ,z2);
    }

    if( (mask & (1<<2)) == (1<<2))
    {
        glVertex3f(j+y,i+y ,z2);
        glVertex3f(j+x ,i+y ,z2);
    }

    if( (mask & (1<<3)) == (1<<3))
    {
        glVertex3f(j+x ,i+y ,z2);
        glVertex3f(j+x ,i+y ,z1);
    }
    /* gauche */
    if( (mask & (1<<11)) == (1<<11))
    {
        glVertex3f(j+x ,i+x  ,z1);
        glVertex3f(j+x ,i+x  ,z2);
    }

    if( (mask & (1<<7)) == (1<<7))
    {
        glVertex3f(j+x ,i+x  ,z2);
        glVertex3f(j+x ,i+y ,z2);
    }

    if( (mask & (1<<3)) == (1<<3))
    {
        glVertex3f(j+x ,i+y ,z2);
        glVertex3f(j+x ,i+y ,z1);
    }

    if( (mask & (1<<4)) == (1<<4))
    {
        glVertex3f(j+x ,i+y ,z1);
        glVertex3f(j+x ,i+x  ,z1);
    }
}

static void draw_plateau()
{
    int i=0,j=0;
    int c;

    memset(edge,0,sizeof(edge));
    /* affiche le sol */
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3f(1,1,1);
    glTexCoord2i(0,0);glVertex2i(0,0);
    glTexCoord2i(4,0);glVertex2i(16,0);
    glTexCoord2i(4,4);glVertex2i(16,16);
    glTexCoord2i(0,4);glVertex2i(0,16);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    for(i=0;i<16;i++)
        for(j=0;j<16;j++)
            if( (c = board[i+1][j+1]) != '.')
            {
                if( c >= '1' && c <= '9' )
                    draw_block(i,j,COLOR[c - '1']);
                else if( c == '#' )
                    draw_block(i,j,COLOR[WALL_COLOR]);
                edge[i+1][j+1] = 1;
            }  
    glEnd();


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2.f);
    glColor3f(0,0,0);

    glBegin(GL_LINES);
    /*for(i=0;i<16;i++)
      for(j=0;j<16;j++)
      {
      if(edge[i][j])
      { 
      c = (1<<12) - 1;
      if( (i>0) && (edge[i-1][j]) && (board[i+1][j+1] == board[i][j+1]))
      c &= ( (1<<12)-1   -  (1<<8) - (1<<9) - (1<<10) - (1<<11) );
      if( (i<15) && (edge[i+1][j]) && (board[i+1][j+1] == board[i+2][j+1]))
      c &= ( (1<<12)-1  - (1<<0) - (1<<1) - (1<<2) - (1<<3) );
      if( (j>0) && (edge[i][j-1]) && (board[i+1][j+1] == board[i+1][j]))
      c &= ( (1<<12)-1  - (1<<3) - (1<<4) - (1<<7) - (1<<11) );
      if( (j<15) && (edge[i][j+1]) && (board[i+1][j+1] == board[i+1][j+2]))
      c &= ( (1<<12)-1  - (1<<1) - (1<<5) - (1<<6) - (1<<9) );

      draw_line(c,i,j);
      }
      }*/ 
    for(i=1;i<17;i++)
        for(j=1;j<17;j++)
        {
            if(edge[i][j])
            { 
                c = (1<<12) - 1;
                if( (edge[i-1][j]) && (board[i][j] == board[i-1][j]))
                    c &= ( (1<<12)-1   -  (1<<8) - (1<<9) - (1<<10) - (1<<11) );
                if( (edge[i+1][j]) && (board[i][j] == board[i+1][j]))
                    c &= ( (1<<12)-1  - (1<<0) - (1<<1) - (1<<2) - (1<<3) );
                if( (edge[i][j-1]) && (board[i][j] == board[i][j-1]))
                    c &= ( (1<<12)-1  - (1<<3) - (1<<4) - (1<<7) - (1<<11) );
                if( (edge[i][j+1]) && (board[i][j] == board[i][j+1]))
                    c &= ( (1<<12)-1  - (1<<1) - (1<<5) - (1<<6) - (1<<9) );

                draw_line(c,i-1,j-1);
            }
        }

    glEnd();
    glLineWidth(1.f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


}
