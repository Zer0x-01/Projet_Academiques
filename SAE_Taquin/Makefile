# Nom de l'exécutable
EXEC = taquin

# Compilateur C
CC = gcc

# Options de compilation :
# -Wall : Tous les avertissements SAUF ceux spécifiés ci-dessous
# -g : Informations de débogage (peut être retiré si non nécessaire)
# -std=c89 : Utilise la norme C89 (ANSI C)
# -Wno-unused-but-set-variable : Désactive l'avertissement de "variable set but not used"
CFLAGS = -Wall -g -std=c89 -I/usr/include/X11 -Wno-unused-but-set-variable

# Fichiers objet
OBJ = main.o taquin.o

# Librairies de lien
LDFLAGS = -L/usr/lib -lgraph -lX11 -lm

# ---------------- RÈGLES ----------------

# Règle principale : crée l'exécutable 'taquin'
all: $(EXEC)

# Règle pour l'édition de liens
$(EXEC): $(OBJ)
	@echo "Liaison des objets pour créer l'exécutable $(EXEC)..."
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)
	@echo "Compilation terminée ! Exécuter avec ./taquin"

# Règle pour compiler main.c
main.o: main.c taquin.h
	@echo "Compilation de main.c..."
	$(CC) $(CFLAGS) -c main.c

# Règle pour compiler taquin.c
taquin.o: taquin.c taquin.h
	@echo "Compilation de taquin.c..."
	$(CC) $(CFLAGS) -c taquin.c

# Règle de nettoyage
clean:
	@echo "Nettoyage des fichiers objets (*.o) et de l'exécutable..."
	rm -f $(OBJ) $(EXEC) *~

.PHONY: all clean