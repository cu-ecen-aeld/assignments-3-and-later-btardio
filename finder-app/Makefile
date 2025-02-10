default: writer

UNAME_P := $(shell uname -p)

COMPILER_EXECUTABLE=$(CROSS_COMPILE)gcc

OBJ = -o writer writer.c

clean:
	rm -f writer

vars:

    UNAME_P := $(shell uname -p)
    #    ifeq ($(UNAME_P),x86_64)
#        CCFLAGS += -D AMD64
#        COMPILER_EXECUTABLE=gcc
#        AAA = -o writer writer.c

#    endif
#    ifeq ($(UNAME_P),ARM)
#        CCFLAGS += -D ARM
#        COMPILER_EXECUTABLE=/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc
#        AAA = -o writer writer.c
#    endif
#    ifeq ($(CROSS_COMPILE),ARM)
#    	COMPILER_EXECUTABLE=/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc
#    	AAA = -o writer writer.c
#    endif

all: writer

writer: vars
	$(info $(COMPILER_EXECUTABLE) -Wall $(CCFLAGS) $(OBJ))
	@($(COMPILER_EXECUTABLE) -Wall $(CCFLAGS) $(OBJ))



