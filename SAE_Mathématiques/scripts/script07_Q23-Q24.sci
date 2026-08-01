// Partie Q23
masse_texte3 = data(2:$, 6);
sex3         = data(2:$, 7);

masses_males    = strtod(masse_texte3(sex3 == "MALE"   & masse_texte3 <> "NA"));
masses_femelles = strtod(masse_texte3(sex3 == "FEMALE" & masse_texte3 <> "NA"));

printf("\n=== QUESTION 23 ===\n");
printf("--------------------------------------------------\n");
printf("| Indicateur  |    Mâles    |   Femelles         |\n");
printf("--------------------------------------------------\n");
printf("| Moyenne (g) | %11.2f | %11.2f         |\n", mean(masses_males),   mean(masses_femelles));
printf("| Médiane (g) | %11.2f | %11.2f         |\n", median(masses_males), median(masses_femelles));
printf("| Écart-type  | %11.2f | %11.2f         |\n", stdev(masses_males),  stdev(masses_femelles));
printf("--------------------------------------------------\n");
printf("====================\n");

// Partie Q24
function r = pearson(x, y)
    r = sum((x - mean(x)) .* (y - mean(y))) / (sqrt(sum((x - mean(x)).^2)) * sqrt(sum((y - mean(y)).^2)));
endfunction

masque_all = (masse_texte3 <> "NA") & (data(2:$, 3) <> "NA") & ...
             (data(2:$, 4) <> "NA") & (data(2:$, 5) <> "NA");

masse_all   = strtod(masse_texte3(masque_all));
culmen_len  = strtod(data(2:$, 3)(masque_all));
culmen_dep  = strtod(data(2:$, 4)(masque_all));
flipper_all = strtod(data(2:$, 5)(masque_all));

r_len  = pearson(culmen_len,  masse_all);
r_dep  = pearson(culmen_dep,  masse_all);
r_flip = pearson(flipper_all, masse_all);

printf("\n=== QUESTION 24 ===\n");
printf("------------------------------------------\n");
printf("| Variable           | Corrélation avec masse |\n");
printf("------------------------------------------\n");
printf("| Longueur du bec    | %22.4f |\n", r_len);
printf("| Profondeur du bec  | %22.4f |\n", r_dep);
printf("| Longueur nageoires | %22.4f |\n", r_flip);
printf("------------------------------------------\n");
printf("====================\n");
