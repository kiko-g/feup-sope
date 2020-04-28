CC = gcc
CFLAGS = -pthread
OBJS = Q1.o ../parser/parser.o ../utils/utils.o
XHDRS = Q1.h Arguments_Server.h ../parser/parser.h ../utils/utils.h
EXEC = Q1

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ -lm

%.o: %.c %.h $(XHDRS)
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	-rm $(OBJS) $(EXEC)
