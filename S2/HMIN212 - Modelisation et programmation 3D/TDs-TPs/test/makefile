############################## PARTIE MODIFIABLE ##############################

CC       = g++
CC_FLAGS = -Wall -Wextra
LD_FLAGS = -lncursesw

# Dossier de sortie pour les fichiers objets / binaires.

OBJ_DIR = obj
BIN_DIR = bin

# Extension de sortie pour les fichiers objets / binaires.

OBJ_EXT = .o
BIN_EXT = 

##### RECHERCHE RECURSIVE DES SOURCES / HEADERS / LIBRARIES A TRAITER

# Extensions de fichiers à chercher.

SRC_SEARCH_EXTS = c cc cp cpp c++ cxx
INC_SEARCH_EXTS = h hh hp hpp h++ hxx
LIB_SEARCH_EXTS = lib so a

# Dossiers de depart pour la recherche (laisser vide pour ne pas faire de recherche).

SRC_SEARCH_DIRS = .
INC_SEARCH_DIRS = .
LIB_SEARCH_DIRS = .

##### FICHIERS SOURCES / HEADERS / OBJETS / LIBRARIES A TRAITER EN PLUS DE LA RECHERCHE RECURSIVE

# Serons traités durant la phase de compilation.

SRCS_EXTRAS = 
INCS_EXTRAS = 
CC_EXTRAS   = 

# Serons traités durant la phase d'édition de liens.

OBJS_EXTRAS = 
LIBS_EXTRAS = 
LD_EXTRAS   = 

##### SOURCES / HEADERS / LIBRARIES A NE PAS TRAITER

# Dossiers à ne pas prendre en compte.

SRC_EXCLUDE_DIRS = 
INC_EXCLUDE_DIRS = 
LIB_EXCLUDE_DIRS = 
EXCLUDE_DIRS     = 

# Fichiers à ne pas prendre en compte.

SRC_EXCLUDES = 
INC_EXCLUDES = 
LIB_EXCLUDES = 
EXCLUDES     = 

############################## PARTIE NON MODIFIABLE ##############################

##### DEFINITIONS DE FONCTIONS

# Renvoie la tete / queue d'une liste passée en paramètre.

head = $(word 1,$(1))
tail = $(wordlist 2,$(words $(1)),$(1))

# Genere les patternes de recherche par extension pour le programme find à partir d'une liste d'extensions (non vide).

by-ext = $(if $(strip $(1)),$(shell echo "-iname '*.$(call head,$(1))'"; for i in $(call tail,$(1)); do echo "-or -iname '*.$$i'"; done),)

# Genere une liste de fichiers à partir d'une liste de dossiers et une liste d'extensions (non vide).

find-by-ext = $(if $(and $(strip $(1)),$(strip $(2))),$(shell realpath --relative-base . $(shell find $(1) $(call by-ext,$(2))) 2> /dev/null),)

# Genere une liste de fichiers objets à partir d'une liste de fichiers binaires et inversement.

attach-tail-with = $(if $(call head,$(2)),$(1)$(call head,$(2))$(call attach-tail-with,$(1),$(call tail,$(2)))) 
attach-with = $(call head,$(2))$(call attach-tail-with,$(1),$(call tail,$(2)))

bin-to-obj = $(addsuffix $(OBJ_EXT),$(basename $(patsubst $(BIN_DIR)/%,$(OBJ_DIR)/%, $(1))))
obj-to-bin = $(addsuffix $(BIN_EXT),$(basename $(patsubst $(OBJ_DIR)/%,$(BIN_DIR)/%, $(1))))
bin-to-dep = $(addsuffix .d,$(basename $(patsubst $(BIN_DIR)/%,$(OBJ_DIR)/%, $(1))))

headers-dep = $(shell cat $(call bin-to-dep,$(1)) | sed -E -n '/.($(call attach-with,|,$(INC_SEARCH_EXTS))):$$/p' | sed s/:// 2> /dev/null)
maximum-possible-objects-dep = $(addprefix $(OBJ_DIR)/,$(addsuffix $(OBJ_EXT),$(basename $(call headers-dep,$(1)))))
objects-dep = $(call bin-to-obj,$(1)) $(filter $(call maximum-possible-objects-dep,$(1)),$(OBJS_WITHOUT_MAIN))

# Définition : template de règles pour la compilation, genere un objet en fonction de sa source correspondante.

define template-rule-compile
$$(OBJ_DIR)/%$(OBJ_EXT): %.$(1)
	$$(CC) $$(CC_FLAGS) $$(INC_FLAGS) -c $$< -o $$@
	@if [ -n "`nm "$$@" | grep -w 'main'`" ]; then \
		if [ ! -f $(OBJS_WITH_MAIN_LOG_FILE) ] || [ ! -n "`cat $(OBJS_WITH_MAIN_LOG_FILE) | grep -w '$$@'`"  ]; then \
			echo '$$@' >> $(OBJS_WITH_MAIN_LOG_FILE); \
		fi; \
	else \
		if [ -f $(OBJS_WITH_MAIN_LOG_FILE) ] && [ -n "`cat $(OBJS_WITH_MAIN_LOG_FILE) | grep -w '$$@'`"  ]; then \
			sed -i '\ $$@  d' $(OBJS_WITH_MAIN_LOG_FILE); \
		fi; \
		if [ -f $$(call obj-to-bin, $$@) ]; then \
			echo suppression du fichier...; \
			rm $$(call obj-to-bin, $$@); \
		fi; \
	fi
endef

# Définition : template de règles pour la construction des binaires, genere un binaire en fonctions de ses dépendances d'objets.

define template-rule-link
$(1): $(call objects-dep,$(1))
	$$(CC) -o $$@ $(call objects-dep,$(1)) $$(LIBS) $$(LD_EXTRAS) $$(LD_FLAGS)
endef

# Detection des sources, includes et libraries selon leurs dossiers de recherche (sort enlève les doublons).

##### TRAITEMENT DES DONNÉES

SRCS := $(call find-by-ext,$(SRC_SEARCH_DIRS),$(SRC_SEARCH_EXTS)) $(SRCS_EXTRAS)
INCS := $(call find-by-ext,$(INC_SEARCH_DIRS),$(INC_SEARCH_EXTS)) $(INCS_EXTRAS)
LIBS := $(call find-by-ext,$(LIB_SEARCH_DIRS),$(LIB_SEARCH_EXTS)) $(LIBS_EXTRAS)

# Fichiers objets necessaires à la compilation.

OBJS := $(addsuffix $(OBJ_EXT),$(basename $(SRCS:%=$(OBJ_DIR)/%)))

# Flags permettant d'inclures les headers à la compilation.

INC_FLAGS := $(addprefix -I,$(sort $(dir $(INCS))))

# Flags permettant de generer des fichiers de dépendances aux headers à la compilation

CC_FLAGS += -MMD -MP

# Fichiers contenant les nom des objets ou le symbole main à été détécté.

OBJS_WITH_MAIN_LOG_FILE := $(OBJ_DIR)/.main.log

# Separations des objets en deux categories

OBJS_WITH_MAIN    := $(shell cat $(OBJS_WITH_MAIN_LOG_FILE) 2> /dev/null)
OBJS_WITHOUT_MAIN := $(filter-out $(OBJS_WITH_MAIN),$(OBJS))

# Fichiers binaires à construire.

BINS := $(call obj-to-bin,$(OBJS_WITH_MAIN))

# Dossier requis pour la constructions des binaires/objs (sort enlève les doublons).

PREREQ_BIN_DIRS := $(sort $(dir $(BINS)))
PREREQ_OBJ_DIRS := $(sort $(dir $(OBJS)))
PREREQ_DIRS     := $(sort $(PREREQ_BIN_DIRS) $(PREREQ_OBJ_DIRS))

ifeq ($(findstring release,$(MAKECMDGOALS)),release)
	CC_FLAGS += -Ofast
else
	CC_FLAGS += -Og -D DEBUG
endif

# Cibles qui ne represent pas des fichiers (toujours applicable).

.PHONY: all debug clean clean-all test show-sources show-headers show-objects show-objects-with-main show-objects-without-main show-binaries show-version test-all

### REGLES DE CONSTRUCTIONS

all: debug

debug: update-and-build-bins

release: update-and-build-bins

update-and-build-bins: $(OBJS_WITH_MAIN_LOG_FILE)
	@$(MAKE) build-bins --no-print-directory | sed 's/\[1\]//'

$(OBJS_WITH_MAIN_LOG_FILE): | $(PREREQ_OBJ_DIRS) $(OBJS)

build-bins: | $(PREREQ_BIN_DIRS) $(BINS)

$(PREREQ_DIRS):
	mkdir -p $@

# Genere les règles de compilation en fonction des extensions de sources definis.

$(foreach EXT, $(SRC_SEARCH_EXTS), $(eval $(call template-rule-compile,$(EXT))))

# Genere les règles d'édition de liens en fonction des binaires detectés

$(foreach BIN, $(BINS), $(eval $(call template-rule-link,$(BIN))))

clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/* $(OBJS_WITH_MAIN_LOG_FILE)

clean-all:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

show-sources:
	@echo $(SRCS)

show-headers:
	@echo $(INCS)

show-objects:
	@echo $(OBJS)

show-objects-with-main:
	@echo $(OBJS_WITH_MAIN)

show-objects-without-main:
	@echo $(OBJS_WITHOUT_MAIN)

show-binaries:
	@echo $(BINS)

show-version:
	@echo 1.0

# Fonctions permettant de tester si une evaluation renvoie bien le resultat attendu.

equal? = $(shell if [ "$(strip $(1))" = "$(strip $(2))" ]; then echo true; fi)
test = $(if $(call equal?,$(1),$(2)),"OK\n$(1)\n$(2)","FAILED\n$(1)\n$(2)")

# Set de tests pour mes fonctions.

test-all:
	@echo TEST : head
	@echo 1 : $(call test, $(call head,),)
	@echo 2 : $(call test, $(call head,a),a)
	@echo 3 : $(call test, $(call head,a b),a)
	@echo TEST : tail
	@echo 1 : $(call test, $(call tail,),)
	@echo 2 : $(call test, $(call tail,a),)
	@echo 3 : $(call test, $(call tail,a b),b)
	@echo 4 : $(call test, $(call tail,a b c),b c)
	@echo TEST : by-ext
	@echo 1 : $(call test, $(call by-ext, ), )
	@echo 2 : $(call test, $(call by-ext, c ), -iname '*.c')
	@echo 3 : $(call test, $(call by-ext, c cpp ), -iname '*.c' -or -iname '*.cpp')
	@echo TEST : find-by-ext
	@echo 1 : $(call test, $(call find-by-ext, , ), )
	@echo 2 : $(call test, $(call find-by-ext, . , .), )
	@echo 3 : $(call test, $(call find-by-ext, test , cpp), test/test_shape.cpp test/test_par.cpp test/test_col.cpp test/test_vois.cpp)
	@echo 4 : $(call test, $(call find-by-ext, test , c cpp ), test/test_shape.cpp test/test_par.cpp test/test_col.cpp test/test_vois.cpp)
	@echo 5 : $(call test, $(call find-by-ext, src test , c cpp ), test/test_shape.cpp test/test_par.cpp test/test_col.cpp test/test_vois.cpp)
	@echo 6 : $(call test, $(call find-by-ext, src , c ), )
	@echo TEST : obj-to-bin
	@echo 1 : $(call test, $(call obj-to-bin, ), )
	@echo 2 : $(call test, $(call obj-to-bin, $(OBJ_DIR)/test/test_par.o), $(BIN_DIR)/test/test_par)
	@echo 3 : $(call test, $(call obj-to-bin, $(OBJ_DIR)/test/test_par.o $(OBJ_DIR)/test/test_shape.o), $(BIN_DIR)/test/test_par $(BIN_DIR)/test/test_shape)
	@echo TEST : bin-to-obj
	@echo 1 : $(call test, $(call bin-to-obj, ), )
	@echo 2 : $(call test, $(call bin-to-obj, $(BIN_DIR)/test/test_par), $(OBJ_DIR)/test/test_par.o)
	@echo 3 : $(call test, $(call bin-to-obj, $(BIN_DIR)/test/test_par $(BIN_DIR)/test/test_shape), $(OBJ_DIR)/test/test_par.o $(OBJ_DIR)/test/test_shape.o)
	@echo TEST : attach-with
	@echo 1 : $(call test, $(call attach-with,|,c),c)
	@echo 2 : $(call test, $(call attach-with,|,c cpp),c|cpp)
	@echo 2 : $(call test, $(call attach-with,|, c cpp),c|cpp)
	@echo 3 : $(call test, $(call attach-with, | ,c cpp c),c | cpp | c)
	@echo 3 : $(call test, $(call attach-with,|,$(INC_SEARCH_EXTS)),h|hh|hp|hpp|h++|hxx)
	@echo 3 : $(call test, $(call attach-with,-or,$(INC_SEARCH_EXTS)),h|hh|hp|hpp|h++|hxx)
	@echo TEST : obj-to-dep
	@echo 1 : $(call test, $(call obj-to-dep,obj/src/test.o), obj/src/test.d)
	@echo 1 : $(call test, $(call obj-to-dep,obj/test.o), obj/test.d)
	@echo TEST : headers-dep
	@echo 1 : $(call test, $(call headers-dep,bin/test/test_par),src/Particle.h src/Canvas.h src/Term.h src/Window.h src/Rect.h src/Vector2.h src/Cell.h src/Attr.h src/Color.h src/Mouse.h src/Keyboard.h)
	@echo 1 : $(call test, $(call headers-dep,bin/test/test_vois),src/Turtle.h src/Canvas.h src/Term.h src/Window.h src/Rect.h src/Vector2.h src/Cell.h src/Attr.h src/Color.h src/Mouse.h src/Keyboard.h)
	@echo TEST : maximum-possible-objects-dep
	@echo 1 : $(call test, $(call maximum-possible-objects-dep,bin/test/test_par),obj/src/Particle.o obj/src/Canvas.o obj/src/Term.o obj/src/Window.o obj/src/Rect.o obj/src/Vector2.o obj/src/Cell.o obj/src/Attr.o obj/src/Color.o obj/src/Mouse.o obj/src/Keyboard.o)
	@echo 1 : $(call test, $(call maximum-possible-objects-dep,bin/test/test_vois),obj/src/Turtle.o obj/src/Canvas.o obj/src/Term.o obj/src/Window.o obj/src/Rect.o obj/src/Vector2.o obj/src/Cell.o obj/src/Attr.o obj/src/Color.o obj/src/Mouse.o obj/src/Keyboard.o)
	@echo TEST : objects-dep
	@echo 1 : $(call test, $(call objects-dep,bin/test/test_col),)
	@echo 1 : $(call test, $(call objects-dep,bin/test/test_par),)
	@echo 1 : $(call test, $(call objects-dep,bin/test/test_shape),)
	@echo 1 : $(call test, $(call objects-dep,bin/test/test_vois),)

# Inclusion des fichiers de dépendances des headers généré durant la compilation.

-include $(OBJS:%$(OBJ_EXT)=%.d)

### AMELIORATIONS ET POSSIBLE PROBLÈMES

# - La regle show-binaries n'affichera pas de resultat si on supprime le fichier .main.log