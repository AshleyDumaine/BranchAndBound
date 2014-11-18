CFLAGS=-pthread -g
CC=cc

SRCS1 = knapsack_serial.c
OBJS1 = $(SRCS1:.c=.o)

SRCS2 = knapsack_threaded.c
OBJS2 = $(SRCS2:.c=.o)

all: knapsack_serial knapsack_threaded

hw7_serial: $(OBJS1)
        $(CC) $(CFLAGS) -o $@ $(OBJS1)

hw7_threaded: $(OBJS2)
        $(CC) $(CFLAGS) -o $@ $(OBJS2)

%.o : %.c
        $(CC) -c $(CFLAGS) $<


clean:
        rm -rf a.out $(OBJS1) $(OBJS2) knapsack_serial knapsack_threaded
