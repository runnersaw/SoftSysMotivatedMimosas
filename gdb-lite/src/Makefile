IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=

_DEPS = run.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = gdb-lite.o run.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

GDB_LITE_C = ../gdb-lite.c
GDB_LITE_OUT = ../gdb-lite

dir_guard=@mkdir -p $(@D)

$(ODIR)/gdb-lite.o: $(GDB_LITE_C)
	$(dir_guard)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.c $(DEPS)
	$(dir_guard)
	$(CC) -c -o $@ $< $(CFLAGS)

gdb-lite: $(OBJ)
	gcc -o $(GDB_LITE_OUT) $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

