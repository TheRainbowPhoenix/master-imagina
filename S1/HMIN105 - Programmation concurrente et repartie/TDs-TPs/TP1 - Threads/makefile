### DESCRIPTION

# Ce makefile permet de chercher et compiler automatiquement des
# fichiers sources en c, c++ et assembleur.

### FONCTIONNEMENT

# 1. Recherche des elements à compiler.
# 2. Compilation des éléments et sortie dans $(OBJ_DIR). 
# 3. Création de $(BINS_LIST) qui contiend le nom des fichiers objets
#    avec fonction main.
# 4. Construction des binaires pour chaque objets dont le nom sera
#    présent dans $(BINS_LIST) et sortie dans $(BIN_DIR).

### IMPORTANT

# Ne pas modifier le fichier .bins.nfo il contient le nom des binaires a generer.
# variable pour ajouter un fichier source à la compilation
# variable pour ajouter un fichier à la construction du binaire

### VARIABLES STATIQUES

CC      = gcc
CFLAGS  = -Wall -Wextra
LDFLAGS = -lpthread

# Dossiers de sorties pour les fichiers objets et binaires.

OBJ_DIR = obj
BIN_DIR = bin

# Dossiers de depart pour la recherche des sources, includes et libraries.
# Utilisez . pour faire une recherche dans tous les sous-dossiers.

SRC_SEARCH_DIRS = .
INC_SEARCH_DIRS = .
LIB_SEARCH_DIRS = .

DEBUG = true

# Extra element for the compiler and linker

CEXTRAS  =
LDEXTRAS =

### VARIABLES CALCULEES

# Detection des sources, includes et libraries selon leurs dossiers de recherche (sort enlève les doublons)

SRCS   := $(shell find $(SRC_SEARCH_DIRS) -iname "*.c" -or -iname "*.cc" -or -iname "*.cpp" -or -iname "*.c++" -or -iname "*.cxx" -or -iname "*.s") $(CEXTRAS)
INCS   := $(shell find $(INC_SEARCH_DIRS) -iname "*.h" -or -iname "*.hh" -or -iname "*.hpp" -or -iname "*.h++" -or -iname "*.hxx")
IFLAGS := $(addprefix -I,$(sort $(dir $(INCS))))
LIBS   := $(shell find $(LIB_SEARCH_DIRS) -iname "*.lib" -or -iname "*.so" -or -iname "*.a")

# Fichiers objets necessaires à la compilation.

OBJS := $(addsuffix .o,$(basename $(SRCS:%=$(OBJ_DIR)/%)))

# Dependances des headers

CFLAGS += -MMD -MP
DEPS := $(OBJS:.o=.d)

# Fichiers informant le nom des objets sans/avec fonction main.

BINS_LIST := $(BIN_DIR)/.bins.nfo

# Fichiers binaires à construire.

BINS := $(shell cat $(BINS_LIST) 2> /dev/null)

MAIN_OBJS    := $(BINS:$(BIN_DIR)/%=$(OBJ_DIR)/%.o)
REGULAR_OBJS := $(filter-out $(MAIN_OBJS), $(OBJS))

# Dossier requis pour la constructions des binaires/objs (sort enlève les doublons)

PREREQ_BIN_DIRS := $(sort $(dir $(BINS)) $(BIN_DIR))
PREREQ_OBJ_DIRS := $(sort $(dir $(OBJS)) $(OBJ_DIR))
PREREQ_DIRS     := $(sort $(PREREQ_BIN_DIRS) $(PREREQ_OBJ_DIRS))

ifeq ($(DEBUG),true)
	CFLAGS += -g -D DEBUG
else
	CFLAGS += -O3
endif

### REGLES DE CONSTRUCTION

# Cibles qui ne represent pas des fichiers.

.PHONY: all clean clean-all

# Si le fichier $(BINS_LIST) n'existe pas on le créé et on relance
# le makefile pour qu'il prenne sa creation.
# Sinon on créé les binaires.

all: build-bins

ifeq ("$(wildcard $(BINS_LIST))","")
build-bins: $(BINS_LIST)
	@echo ""; \
	echo "INCLUDE(S) DIR(S):" $(INC_DIRS); \
	echo "SOURCE(S):" $(SRCS); \
	if [ -f $(BINS_LIST) ]; then \
		echo "OBJECT(S):" $(OBJS); \
		echo "BINARIE(S):" $$(cat $(BINS_LIST)); \
		echo ""; \
		$(MAKE) --no-print-directory | sed -e 's/\[1\]//'; \
	else \
		echo "ATTENTION : aucun main trouvé (verifiez qu'aucun fichier ne comporte d'espace dans son chemin)"; \
	fi
else
build-bins: $(BINS) 
endif

lib: DEBUG=no
lib: $(BINS_LIST)


$(OBJS): | $(PREREQ_OBJ_DIRS)
$(BINS) $(BINS_LIST): | $(PREREQ_BIN_DIRS)
 
$(BINS_LIST): $(OBJS)
	@rm -f $@; \
	for obj in $(OBJS); do \
		obj_contain_main=$$(nm "$$obj" | grep -w "main"); \
		if [ -n "$$obj_contain_main" ]; then \
			printf "$$obj\n" | sed -e "s/$(OBJ_DIR)\//$(BIN_DIR)\//" | rev | cut -d '.' -f2- | rev >> $@; \
		fi; \
	done

$(PREREQ_DIRS):
	mkdir -p $@

# Définition : template de règles pour la compilation de .$(EXT) en .o

define rule_compile_src_template
$$(OBJ_DIR)/%.o: %.$1
	$$(CC) $$(CFLAGS) $$(IFLAGS) -c $$< -o $$@
endef

# Extensions possibles pour les sources

SRC_EXTS = c C cc CC cpp CPP c++ C++ cxx CXX s S

$(foreach EXT, $(SRC_EXTS), $(eval $(call rule_compile_src_template,$(EXT))))

# Définition : template de règles pour la construction des binaires

define rule_link_src_template
$1: $2
	$$(CC) -o $$@ $$< $$(REGULAR_OBJS) $$(LIBS) $$(LDEXTRAS) $$(LDFLAGS)
endef

$(foreach BIN, $(BINS), $(eval $(call rule_link_src_template,$(BIN),$(BIN:$(BIN_DIR)/%=$(OBJ_DIR)/%.o))))

clean:
	rm -f $(OBJS) $(BINS) $(DEPS) $(BINS_LIST)

clean-all: clean
	find $(OBJ_DIR) -empty -type d -delete 2> /dev/null || true
	find $(BIN_DIR) -empty -type d -delete 2> /dev/null || true

-include $(DEPS)

### AMELIORATIONS

# - Trouver un moyen de mettre a jour le fichier .bins.nfo si modifié.
# - Parallélisation pour la verification des fichier objets contenant une fonciton main:
# 
# rm -f $@; \
# for obj in $$(cat $<); do \
# 	(obj_contain_main=$$(nm "$$obj" | grep -w "main"); \
# 	if [ -n "$$obj_contain_main" ]; then \
# 		(flock -x 200 printf "$$obj\n" | sed -e "s/$(OBJ_DIR)\//$(BIN_DIR)\//" | cut -f 1 -d '.' >> $@) 200 > $@; \
# 	fi) & \
# done; wait; \
# rm -f 200
# 
# - Fonction makefile optimisées:
# 
# uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))
# rwildcard = $(strip $(foreach d,$(wildcard $(addsuffix *,$(1))),$(call rwildcard,$(d)/,$(2)) $(filter $(subst *,%,$(2)),$(d))))
#
# - Construct lib
# 
# g++ -Wall -fPIC -c $(find src -name "*.cpp")
# g++ -shared -Wl,-soname,libunidraw.so -o libunidraw.so *.o
# g++ -Wall -L. -Wl,-rpath,. -Isrc test/test_par.cpp -o test_par -lunidraw -lncursesw