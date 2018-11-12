########################################
#~ définitions
########################################

# nom de l'executable
BIN=./bin/jeu

# liste des fichiers sources  
SRCS=jeu.c

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	gcc -Wall -g -Iinclude -c $< -o $@

bin/jeu: $(SRCS:%.c=obj/%.o)
	gcc  -o $@ obj/etape.o $+ -lpthread

clean:
	rm -f $(BIN) obj/jeu.o *~
