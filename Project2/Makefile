CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -Wno-unused-parameter -Wno-unused-variable -pthread -lpthread
UOBJ = client/U1.o
Q1OBJ = server/Q1.o
Q2OBJ = server/Q2.o server/queue.o
COBJ = utils/utils.o parser/parser.o 
DEPS = server/Q2.h server/Q1.h server/ServerArgs.h server/queue.h client/U1.h client/ClientArgs.h utils/utils.h parser/parser.h

TARGETS = U1 Q1 Q2

all: U1 Q1 Q2

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

U1: $(UOBJ) $(COBJ)
	$(CC) $(CFLAGS) $(COBJ) $(UOBJ) -o $@

Q1: $(Q1OBJ) $(COBJ)
	$(CC) $(CFLAGS) $(COBJ) $(Q1OBJ) -o $@

Q2: $(Q2OBJ) $(COBJ)
	$(CC) $(CFLAGS) $(COBJ) $(Q2OBJ) -o $@

.PHONY : clean
clean:
	rm $(UOBJ) $(Q1OBJ) $(COBJ) $(TARGETS)