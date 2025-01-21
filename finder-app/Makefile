default: writer

UNAME_P := $(shell uname -p)

AAA = gcc -o writer writer.c

clean:
	rm -f writer

vars:

    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CCFLAGS += -D AMD64
        AAA = gcc -o writer writer.c
    endif
    ifeq ($(UNAME_P),ARM)
        CCFLAGS += -D ARM
        AAA = /arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc -o writer writer.c
    endif

writer: vars
	$(info $(AAA))
	@($(AAA))