prefix = /usr/local
exec_prefix = ${prefix}
libdir = ${exec_prefix}/lib
globesconf= $(exec_prefix)/bin/globes-config

local_CFLAGS = -g -O4

INCFLAGS:=$(shell $(globesconf) --include)
local_LDFLAGS:=$(shell $(globesconf) --libs)
local_LTLDFLAGS:=$(shell $(globesconf) --ltlibs)



BIN = example-prob th13delta
OBJ = example-prob.o th13delta.o myio.o

all: $(BIN)

example-prob: example-prob.o myio.o
	gcc example-prob.o myio.o -o example-prob $(LDFLAGS) $(local_LDFLAGS)

th13delta: th13delta.o
	gcc th13delta.o -o th13delta $(LDFLAGS) $(local_LDFLAGS)

%.o : %.c
	gcc $(CFLAGS) $(local_CFLAGS) -c $< $(INCFLAGS)
.PHONY: clean
clean:
	rm -f $(BIN) $(OBJ)
