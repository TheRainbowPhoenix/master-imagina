### VARIABLES MAKEFILE

JAVA    = java
JAVAC   = javac
JFLAGS  = -Wall -Wextra

# Dossiers de sorties pour les fichiers objets et binaires.
OBJ_DIR = obj
BIN_DIR = bin

# Dossiers de depart pour la recherche des sources, includes et libraries.
# Utilisez . pour faire une recherche dans tous les sous-dossiers.
SRC_SEARCH_DIRS = .
LIB_SEARCH_DIRS = .

# Extra element for the compiler and linker
CEXTRAS  =
LDEXTRAS =

### VARIABLES CALCULEES

# Detection des sources, includes et libraries selon leurs dossiers de recherche (sort enlève les doublons)
SRCS   := $(shell find $(SRC_SEARCH_DIRS) -iname "*.java")
LIBS   := $(shell find $(LIB_SEARCH_DIRS) -iname "*.jar")

# Fichiers objets necessaires à la compilation.
OBJS := $(SRCS:%.java=%.class)

# Dossier requis pour la constructions des binaires/objs (sort enlève les doublons)
PREREQ_OBJ_DIRS := $(sort $(dir $(OBJS)) $(OBJ_DIR))
PREREQ_DIRS     := $(sort $(PREREQ_OBJ_DIRS))

### REGLES DE CONSTRUCTION

.PHONY: all clean clean-all

all: build-bins

$(OBJS): | $(PREREQ_OBJ_DIRS)
 
$(PREREQ_DIRS):
	mkdir -p $@

$(OBJ_DIR)/%.class: %.java
	$(JAVAC) $(JFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(BINS) $(DEPS)

clean-all: clean
	find $(OBJ_DIR) -empty -type d -delete 2> /dev/null || true
	find $(BIN_DIR) -empty -type d -delete 2> /dev/null || true
