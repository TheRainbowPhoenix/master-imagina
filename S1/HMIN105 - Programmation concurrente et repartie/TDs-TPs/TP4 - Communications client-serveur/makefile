CC = gcc
FLAGS = -W -Wall

SRCS0 = client.c
SRCS1 = server.c

BIN0 = client
BIN1 = server

INCLUDE = 

EXT = *.o *~

default: $(BIN0) $(BIN1) clean

%.o: %.c
	$(CC) $(FLAGS) $(INCLUDE) -c $< -o $@

$(BIN0): $(SRCS0:%.c=%.o)
	$(CC) -o $@ $+

$(BIN1): $(SRCS1:%.c=%.o)
	$(CC) -o $@ $+

clean:
	rm -f $(EXT)

mrproper: clean
	rm -f $(BIN0) $(BIN1)