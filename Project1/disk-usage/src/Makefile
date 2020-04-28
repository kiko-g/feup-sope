CC = gcc
CFLAGS = -lm -Wall
OBJS = simpledu.o register.o parser.o signals.o
XHDRS = simpledu.h register.h parser.h signals.h
EXEC = simpledu

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ -lm

%.o: %.c %.h $(XHDRS)
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	-rm $(OBJS) $(EXEC)
