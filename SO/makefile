CC     = gcc
CFLAGS = -Wextra -g
LFLAGS = -lm
DEBUGFLASGS = -DDEBUGSEMAFORO

PROG = pingpong-mqueue
OBJS = ppos_core.o queue.o

.PHONY: clean purge all

all: $(PROG)

debug: CFLAGS += $(DEBUGFLASGS)
debug: all

# %.o: %.c
# 	$(CC) -c $(CFLAGS) $<

$(PROG) : % :  $(OBJS) %.o
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	rm -f *.o

purge: clean  
	rm -f $(PROG) 
