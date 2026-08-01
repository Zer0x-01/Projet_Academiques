// Partie Q15
flipper_texte   = data(2:$, 5);
masse_texte2    = data(2:$, 6);

masque = (flipper_texte <> "NA") & (masse_texte2 <> "NA");
flipper_num2 = strtod(flipper_texte(masque));
masse_num2   = strtod(masse_texte2(masque));

scf(4); clf();
plot(flipper_num2, masse_num2, "b.");
title("Nuage de points : longueur des nageoires vs masse corporelle", "fontsize", 4);
xlabel("Longueur des nageoires (mm)", "fontsize", 3);
ylabel("Masse corporelle (g)", "fontsize", 3);

printf("\n=== QUESTION 15 ===\n");
printf("Nuage de points affiché - Figure 4\n");
printf("====================\n");

// Partie Q16
function r = pearson(x, y)
    r = sum((x - mean(x)) .* (y - mean(y))) / (sqrt(sum((x - mean(x)).^2)) * sqrt(sum((y - mean(y)).^2)));
endfunction

r_flipper_masse = pearson(flipper_num2, masse_num2);

printf("\n=== QUESTION 16 ===\n");
printf("Coefficient de corrélation (nageoires / masse) : %.4f\n", r_flipper_masse);
printf("====================\n");

// Partie Q17
culmen_len_texte   = data(2:$, 3);
culmen_depth_texte = data(2:$, 4);

masque_bec = (culmen_len_texte <> "NA") & (culmen_depth_texte <> "NA");
culmen_len_num   = strtod(culmen_len_texte(masque_bec));
culmen_depth_num = strtod(culmen_depth_texte(masque_bec));

r_bec = pearson(culmen_len_num, culmen_depth_num);

printf("\n=== QUESTION 17 ===\n");
printf("Coefficient de corrélation (longueur bec / profondeur bec) : %.4f\n", r_bec);
printf("====================\n");
