CC = gcc
CFLAGS = -Wall
OBJS = simpledu.o register.o parser.o
XHDRS = simpledu.h register.h parser.h
EXEC = simpledu

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c %.h $(XHDRS)
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	-rm $(OBJS) $(EXEC)
