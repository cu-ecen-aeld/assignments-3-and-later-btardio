default: all

UNAME_P := $(shell uname -p)

COMPILER_EXECUTABLE=$(CROSS_COMPILE)gcc

OBJ = -o server server.c

clean:
	rm -f server

vars:

    UNAME_P := $(shell uname -p)

all: server

server: vars
	$(info $(COMPILER_EXECUTABLE) -Wall $(CCFLAGS) $(OBJ))
	@($(COMPILER_EXECUTABLE) -Wall $(CCFLAGS) $(OBJ))








