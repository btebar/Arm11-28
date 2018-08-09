#CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -D_DEFAULT_SOURCE -std=c99

ASSEMBLE_SOURCES = assemble.c $(wildcard assemble_utils/*.c)
ASSEMBLE_OBJECTS = assemble.o $(ASSEMBLE_SOURCES:.c=.o)

EMULATE_SOURCES = emulate.c $(wildcard emulate_utils/*.c)
EMULATE_OBJECTS = emulate.o $(EMULATE_SOURCES:.c=.o)

.SUFFIXES: .c .o

.PHONY: all clean

all: emulate assemble

assemble: $(ASSEMBLE_OBJECTS)

emulate: $(EMULATE_OBJECTS)

clean:
	rm -f $(ASSEMBLE_OBJECTS)
	rm -f $(EMULATE_OBJECTS)
	rm -f assemble
	rm -f emulate
	rm -f out

test:
	make
	@echo "Make sure arm11_1718_testsuite is cloned in the same directory as arm11_TEAMNUMBER"
	ruby ../../arm11_1718_testsuite/testserver.rb .

gdb-emulate:
	make
	@echo "Make sure arm11_1718_testsuite is cloned in the same directory as arm11_TEAMNUMBER"
	@echo "use like this: make gdb what=(assemble/emulate) test=NAME_OF_TEST.s"
	gdb --args emulate ../../arm11_1718_testsuite/test_cases/$(test)

gdb-assemble:
	make
	@echo "Make sure arm11_1718_testsuite is cloned in the same directory as arm11_TEAMNUMBER"
	@echo "use like this: make gdb what=(assemble/emulate) test=NAME_OF_TEST.s"
	gdb --args assemble ../../arm11_1718_testsuite/test_cases/$(test) out
