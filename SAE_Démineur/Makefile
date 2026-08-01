JAVAC = javac
JAVA  = java
MAIN  = Main

SOURCES = \
    Case.java \
    Grille.java \
    Partie.java \
    Sauvegarde.java \
    FenetreMenu.java \
    FenetreConfig.java \
    PanneauGrille.java \
    FenetreJeu.java \
    FenetreFinPartie.java \
    Main.java

all: Main.class

Main.class: $(SOURCES)
	$(JAVAC) -implicit:none -encoding UTF-8 $(SOURCES)

run: all
	$(JAVA) $(MAIN)

clean:
ifeq ($(OS),Windows_NT)
	del /Q *.class
else
	rm -f *.class
endif