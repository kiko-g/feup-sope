CC = gcc
CFLAGS = -pthread
OBJS = U1.o ../parser/parser.o
XHDRS = U1.h Arguments_client.h ../parser/parser.h
EXEC = U1

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ -lm

%.o: %.c %.h $(XHDRS)
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	-rm $(OBJS) $(EXEC)
