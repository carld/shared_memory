
CC        =  gcc

ifdef GCOV
CFLAGS   +=  -fprofile-arcs -ftest-coverage
LDFLAGS  +=  -fprofile-arcs -ftest-coverage
endif

ifdef STATIC
LDFLAGS  +=  $(shell sdl2-config --static-libs)
else
LDFLAGS  +=  $(shell sdl2-config --libs)
#LDFLAGS  +=  -lGL -lm #-framework OpenGL
endif

CFLAGS   +=  $(shell sdl2-config --cflags)
CFLAGS   +=  -ansi -Wall
CFLAGS   +=  -g
CFLAGS   +=  -ggdb
CFLAGS   +=  -pedantic

SRC       =  main.c 
OBJ       =  $(SRC:.c=.o)
BIN       =  shm_window

%.o: %.c
	$(CC) -c $(CFLAGS) $<

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
	strip $@

clean:
	rm -f $(OBJ) $(BIN) *.o

.PHONY: clean

