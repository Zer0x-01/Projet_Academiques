// Partie Q4
masse_texte = data(2:$, 6); // colonne 6 sans l'en-tête
masse_valides = masse_texte(masse_texte <> "NA");
masse_num = strtod(masse_valides);
masse_moyenne = mean(masse_num);

printf("\n=== QUESTION 4 ===\n");
printf("La masse moyenne des manchots est : %.2f grammes\n", masse_moyenne);
printf("==================\n");

// Partie Q5
masse_mediane = median(masse_num);
masse_ecart_type = stdev(masse_num);

printf("\n=== QUESTION 5 ===\n");
printf("Statistiques sur la masse corporelle :\n");
printf("  - Moyenne    : %.2f grammes\n", masse_moyenne);
printf("  - Médiane    : %.2f grammes\n", masse_mediane);
printf("  - Écart-type : %.2f grammes\n", masse_ecart_type);
printf("==================\n");

// Partie Q6

// Moyenne pour les Adelie
masses_adelie = masse_texte(species == "Adelie" & masse_texte <> "NA");
moyenne_adelie = mean(strtod(masses_adelie));

// Moyenne pour les Chinstrap
masses_chinstrap = masse_texte(species == "Chinstrap" & masse_texte <> "NA");
moyenne_chinstrap = mean(strtod(masses_chinstrap));

// Moyenne pour les Gentoo
masses_gentoo = masse_texte(species == "Gentoo" & masse_texte <> "NA");
moyenne_gentoo = mean(strtod(masses_gentoo));

printf("\n=== QUESTION 6 ===\n");
printf("Masse moyenne par espèce :\n");
printf("  - Adelie    : %.2f grammes\n", moyenne_adelie);
printf("  - Chinstrap : %.2f grammes\n", moyenne_chinstrap);
printf("  - Gentoo    : %.2f grammes\n", moyenne_gentoo);
printf("==================\n");

// Partie Q7
ecart_type_adelie    = stdev(strtod(masses_adelie));
ecart_type_chinstrap = stdev(strtod(masses_chinstrap));
ecart_type_gentoo    = stdev(strtod(masses_gentoo));

// Calcul des coefficients de variation en %
cv_adelie    = (ecart_type_adelie / moyenne_adelie) * 100;
cv_chinstrap = (ecart_type_chinstrap / moyenne_chinstrap) * 100;
cv_gentoo    = (ecart_type_gentoo / moyenne_gentoo) * 100;

printf("\n=== QUESTION 7 ===\n");
printf("Variabilité par espèce (Écart-type | Coef. Variation) :\n");
printf("  - Adelie    : %.2f g  |  CV : %.2f%%\n", ecart_type_adelie, cv_adelie);
printf("  - Chinstrap : %.2f g  |  CV : %.2f%%\n", ecart_type_chinstrap, cv_chinstrap);
printf("  - Gentoo    : %.2f g  |  CV : %.2f%%\n", ecart_type_gentoo, cv_gentoo);
printf("==================\n");

// Partie Q8
q_masse = quart(masse_num); // renvoie un vecteur [Q1; Q2; Q3]

printf("\n=== QUESTION 8 ===\n");
printf("Quartiles de la masse corporelle globale :\n");
printf("  - Q1 (25%%) : %.2f grammes\n", q_masse(1));
printf("  - Q2 (50%%) : %.2f grammes\n", q_masse(2));
printf("  - Q3 (75%%) : %.2f grammes\n", q_masse(3));
printf("==================\n");

// Partie Q9
IQ = q_masse(3) - q_masse(1);
printf("\n=== QUESTION 9 ===\n");
printf("L''écart interquartile (IQR) est de : %.2f grammes\n", IQ);
printf("==================\n");

