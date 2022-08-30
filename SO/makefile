CC     = gcc
CFLAGS = -Wextra -g
DEBUGFLASGS = -DDEBUGSEMAFORO

PROG = pingpong-prodcons
OBJS = ppos_core.o queue.o

.PHONY: clean purge all

all: $(PROG)

debug: CFLAGS += $(DEBUGFLASGS)
debug: all

%.o: %.c
	$(CC) -c $(CFLAGS) $<

$(PROG) : % :  $(OBJS) %.o
	$(CC) -o $@ $^

clean:
	rm -f *.o

purge: clean  
	rm -f $(PROG) 
