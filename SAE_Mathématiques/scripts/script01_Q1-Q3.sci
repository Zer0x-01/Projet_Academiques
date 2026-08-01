// Partie Q1
data = csvRead("penguins_size.csv", ",", [], "string");
nb_manchots = size(data, 1) - 1; // On soustrait 1 pour l'entête (la premiere ligne)

printf("\n=== QUESTION 1 ===\n");
printf("Le nombre total de manchots est : %d\n", nb_manchots);


// Partie Q2
species = data(2:$, 1); // colonne 1 sans l'en-tête

nb_adelie    = sum(species == "Adelie");
nb_chinstrap = sum(species == "Chinstrap");
nb_gentoo    = sum(species == "Gentoo");

printf("\n=== QUESTION 2 ===\n");
printf("Répartition des individus par espèce :\n");
printf("  - Adelie    : %d\n", nb_adelie);
printf("  - Chinstrap : %d\n", nb_chinstrap);
printf("  - Gentoo    : %d\n", nb_gentoo);
printf("==================\n");

// Partie Q3
sex = data(2:$, 7); // colonne 7 sans l'en-tête

nb_males    = sum(sex == "MALE");
nb_females  = sum(sex == "FEMALE");
// On prend le total (344) et on enlève les mâles et les femelles pour trouver les inconnus
nb_inconnus = size(sex, 1) - nb_males - nb_females; 

printf("\n=== QUESTION 3 ===\n");
printf("Répartition des individus par sexe :\n");
printf("  - Mâles    : %d\n", nb_males);
printf("  - Femelles : %d\n", nb_females);
printf("  - Inconnus : %d\n", nb_inconnus);
printf("==================\n");
