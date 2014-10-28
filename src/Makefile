CC = gcc

LIBS_GTK  = `pkg-config --libs gtk+-2.0 gtkglext-1.0`
FLAGS_GTK = `pkg-config --cflags gtk+-2.0 gtkglext-1.0`

SRC  = files.c game.c gui.c gfx.c main.c 
OBJ  = $(SRC:.c=.o)
PROG = denki

FLAGS = $(FLAGS_GTK) -Wall -g
LIBS  = $(LIBS_GTK) -lm -lGL -lGLU

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o $(PROG) $(OBJ) $(LIBS) 

%.o: %.c %.h
	$(CC) $(FLAGS) -c $<

clean:
	rm -f $(PROG) $(OBJ) *~ \#*\#
