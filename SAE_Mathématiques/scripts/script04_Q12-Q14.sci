// Partie Q12 — Histogramme de la masse corporelle
masse_texte   = data(2:$, 6);
masse_valides = masse_texte(masse_texte <> "NA");
masse_num     = strtod(masse_valides);

masse_min      = min(masse_num);   
masse_max      = max(masse_num);   

borne_inf      = masse_min;        
borne_sup      = masse_max;        
largeur_classe = 400;           

bornes_m       = borne_inf:largeur_classe:borne_sup; 
nb_classes     = length(bornes_m) - 1;               
effectifs      = zeros(1, nb_classes);

for i = 1:nb_classes - 1
    effectifs(i) = sum(masse_num >= bornes_m(i) & masse_num < bornes_m(i+1));
end
effectifs(nb_classes) = sum(masse_num >= bornes_m(nb_classes) & masse_num <= bornes_m(nb_classes+1));

scf(2);
clf();
bar(bornes_m(1:nb_classes) + largeur_classe/2, effectifs, 1);
a = gca();
a.axes_visible = ["on", "on"];
a.data_bounds  = [borne_inf - largeur_classe/2, 0; borne_sup + largeur_classe/2, max(effectifs) + 10];
a.x_ticks = tlist(["ticks", "locations", "labels"], bornes_m, string(bornes_m));
a.x_label.font_size = 2;

title("Histogramme de la masse corporelle (en g)", "fontsize", 4);
xlabel("Masse corporelle (g)", "fontsize", 3);
ylabel("Effectif", "fontsize", 3);

printf("\n=== QUESTION 12 ===\n");
printf("Histogramme de la masse corporelle\n");
printf("Min reel = %d g | Max reel = %d g\n", masse_min, masse_max);
printf("Classes de %dg, de %d a %d - Figure 2 affichee\n", largeur_classe, borne_inf, borne_sup);
printf("====================\n");

// Partie Q13 — Classe modale et fréquences
n_total  = sum(effectifs);
freq_rel = effectifs / n_total * 100;

[f_max, idx_modal] = max(effectifs);
classe_modale_min  = bornes_m(idx_modal);
classe_modale_max  = bornes_m(idx_modal + 1);

printf("\n=== QUESTION 13 ===\n");
printf("---------------------------------------------------------\n");
printf("| Classe (g)      | Effectif | Frequence (%%)           |\n");
printf("---------------------------------------------------------\n");
for i = 1:nb_classes
    printf("| [%4.0f - %4.0f[  |   %3.0f    |    %5.1f %%\n", ...
           bornes_m(i), bornes_m(i+1), effectifs(i), freq_rel(i));
end
printf("---------------------------------------------------------\n");
printf("Classe MODALE : [%4.0f - %4.0f[ avec effectif %3.0f\n", classe_modale_min, classe_modale_max, f_max);
printf("---------------------------------------------------------\n");
printf("====================\n");

// Partie Q14 — Histogramme de la longueur des nageoires (Correction Bornes Éxactes)
flipper_texte   = data(2:$, 5);
flipper_valides = flipper_texte(flipper_texte <> "NA");
flipper_num     = strtod(flipper_valides);
flipper_num     = flipper_num(flipper_num > 0); 

flipper_min  = min(flipper_num);   // 172 mm
flipper_max  = max(flipper_num);   // 231 mm

borne_inf_f  = flipper_min;        // Départ exact à 172 mm
borne_sup_f  = flipper_max;        // Fin exacte à 231 mm
nb_flip      = 8;                  // 8 classes pour correspondre au README
largeur_flip = (borne_sup_f - borne_inf_f) / nb_flip; // Pas exact de 7.375 mm

bornes_f     = linspace(borne_inf_f, borne_sup_f, nb_flip + 1);

effectifs_flipper = zeros(1, nb_flip);
for i = 1:nb_flip - 1
    effectifs_flipper(i) = sum(flipper_num >= bornes_f(i) & flipper_num < bornes_f(i+1));
end
effectifs_flipper(nb_flip) = sum(flipper_num >= bornes_f(nb_flip) & flipper_num <= bornes_f(nb_flip+1));

n_total_flip = sum(effectifs_flipper);
freq_flip    = effectifs_flipper / n_total_flip * 100;

scf(3);
clf();
bar(bornes_f(1:nb_flip) + largeur_flip/2, effectifs_flipper, 1);
a = gca();
a.axes_visible = ["on", "on"];
a.data_bounds  = [borne_inf_f - largeur_flip/2, 0; borne_sup_f + largeur_flip/2, max(effectifs_flipper) + 10];

// Formatage propre des étiquettes des axes avec une décimale
labels_f = [];
for i = 1:length(bornes_f)
    labels_f = [labels_f, msprintf("%.1f", bornes_f(i))];
end
a.x_ticks = tlist(["ticks", "locations", "labels"], bornes_f, labels_f);
a.x_label.font_size = 2;

title("Histogramme de la longueur des nageoires (en mm)", "fontsize", 4);
xlabel("Longueur des nageoires (mm)", "fontsize", 3);
ylabel("Effectif", "fontsize", 3);

printf("\n=== QUESTION 14 ===\n");
printf("-----------------------------------------------------------\n");
printf("| Classe (mm)        | Effectif | Frequence (%%)           |\n");
printf("-----------------------------------------------------------\n");
for i = 1:nb_flip
    printf("| [%5.1f - %5.1f[  |   %3.0f    |   %5.1f %%               |\n", ...
           bornes_f(i), bornes_f(i+1), effectifs_flipper(i), freq_flip(i));
end
printf("-----------------------------------------------------------\n");
printf("Total : %d manchots\n", n_total_flip);
printf("====================\n");