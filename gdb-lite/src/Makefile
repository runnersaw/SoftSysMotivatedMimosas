IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=

_DEPS = run.h breakpoint.h dump.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = gdb-lite.o run.o breakpoint.o dump.o nm.o
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

$(GDB_LITE_OUT): $(OBJ)
	gcc -o $(GDB_LITE_OUT) $^ $(CFLAGS) $(LIBS)

gdb-lite: $(GDB_LITE_OUT)

.PHONY: clean gdb-lite

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
