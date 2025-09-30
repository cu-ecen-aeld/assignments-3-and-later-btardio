default: all

UNAME_P := $(shell uname -p)

UNITY_ROOT=test/Unity/

# CFLAGS=-std=c89
CCFLAGS += -Wall
CCFLAGS += -Wextra
CCFLAGS += -Wpointer-arith
CCFLAGS += -Wcast-align
CCFLAGS += -Wwrite-strings
CCFLAGS += -Wswitch-default
CCFLAGS += -Wunreachable-code
CCFLAGS += -Winit-self
CCFLAGS += -Wmissing-field-initializers
CCFLAGS += -Wno-unknown-pragmas
CCFLAGS += -Wundef
CCFLAGS += -Wold-style-definition

LDFLAGS += -pthread


COMPILER_EXECUTABLE=$(CROSS_COMPILE)gcc


SRC_FILES=src/serverf.c src/server.c
TEST_SRC_FILES=$(UNITY_ROOT)/src/unity.c src/serverf.c test/server_test.c test/test_runners/server_test_runner.c
INC_DIRS=-Isrc -I$(UNITY_ROOT)/src
TARGET_SERVER = aesdsocket
TARGET_TEST = aesdsocket_test


clean:
	rm -f aesdsocket
	rm -f aesdsocket_test


all: $(TARGET_SERVER)

$(TARGET_SERVER):
	@($(COMPILER_EXECUTABLE) $(CCFLAGS) $(LDFLAGS) $(INC_DIRS) $(SRC_FILES) -o $(TARGET_SERVER))

test: server_test_runner
	$(CC) $(CFLAGS) $(LDFLAGS) $(INC_DIRS) $(SYMBOLS) -g $(TEST_SRC_FILES) -o $(TARGET_TEST)

server_test_runner: test/server_test.c
	ruby $(UNITY_ROOT)/auto/generate_test_runner.rb test/server_test.c test/test_runners/server_test_runner.c






