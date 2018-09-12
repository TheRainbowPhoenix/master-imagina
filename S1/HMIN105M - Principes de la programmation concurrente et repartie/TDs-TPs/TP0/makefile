CC     = gcc
CFLAGS = -Wall

SOURCES_EXTENSION  = .c .C .cc .cpp .CPP .c++ .cxx
INCLUDES_EXTENSION = .h .H .hh .hpp .HPP .h++ .hxx
LIBS_EXTENSION     = .lib .so .a

SOURCES_DIR  = .
INCLUDES_DIR = .
LIBS_DIR     = .
INSTALL_DIR  = install

EXEC = run

SOURCES = $(foreach EXT, $(SOURCES_EXTENSION), $(wildcard $(SOURCES_DIR)/*$(EXT)))
OBJECTS = $(addsuffix .o,$(basename $(SOURCES)))

INCLUDES = $(foreach EXT, $(INCLUDES_EXTENSION), $(wildcard $(INCLUDES_DIR)/*$(EXT)))
LIBS     = $(foreach EXT, $(LIBS_EXTENSION), $(wildcard $(LIBS_DIR)/*$(EXT)))

# les cibles à construire
all : $(EXEC)

# la cible principale
$(EXEC) : $(OBJECTS)
	@$(CC) -o $(EXEC) $(OBJECTS) $(LIBS)

# genere les fichiers objets à partir des extensions
%.o: $(foreach EXT, $(SOURCES_EXTENSION), %$(EXT))
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

# liste des cibles à construire sans verification
.PHONY : clean mrproper

# supprime tout les fichiers intermediaires
clean :
	@rm -f $(OBJECTS)

# supprime tout les fichiers généré
mrproper : clean
	@rm -f $(EXEC)

release : all clean

install : all
	@echo "Installing $(EXEC) to $(INSTALL_DIR)/"
	@mkdir -p $(INSTALL_DIR)
	@cp $(EXEC) $(INSTALL_DIR)

uninstall :
	@echo "Removing $(INSTALL_DIR)/$(EXEC)"
	@rm -f $(INSTALL_DIR)/$(EXEC)