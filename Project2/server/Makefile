CC = gcc
CFLAGS = -lm -Wall
OBJS = Q1.o
XHDRS = Q1.h
EXEC = Q1

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ -lm

%.o: %.c %.h $(XHDRS)
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	-rm $(OBJS) $(EXEC)
