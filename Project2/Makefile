CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -Wno-unused-parameter -Wno-unused-variable -pthread -lpthread
UOBJ = client/U1.o
QOBJ = server/Q1.o server/queue.o
COBJ = utils/utils.o parser/parser.o 
DEPS = server/Q1.h server/ServerArgs.h server/queue.h client/U1.h client/ClientArgs.h utils/utils.h parser/parser.h

TARGETS = U1 Q1

all: U1 Q1

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

U1: $(UOBJ) $(COBJ)
	$(CC) $(CFLAGS) $(COBJ) $(UOBJ) -o $@

Q1: $(QOBJ) $(COBJ)
	$(CC) $(CFLAGS) $(COBJ) $(QOBJ) -o $@

.PHONY : clean
clean:
	rm $(UOBJ) $(QOBJ) $(COBJ) $(TARGETS)