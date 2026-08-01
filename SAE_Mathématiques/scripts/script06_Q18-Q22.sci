// Données communes (nageoires + masse, sans NA)
flipper_texte  = data(2:$, 5);
masse_texte2   = data(2:$, 6);
masque = (flipper_texte <> "NA") & (masse_texte2 <> "NA");
flipper_num2 = strtod(flipper_texte(masque));
masse_num2   = strtod(masse_texte2(masque));

// Fonction corrélation de Pearson (réutilisée depuis script05)
function r = pearson(x, y)
    r = sum((x - mean(x)) .* (y - mean(y))) / (sqrt(sum((x - mean(x)).^2)) * sqrt(sum((y - mean(y)).^2)));
endfunction

// Partie Q18
moy_x = mean(flipper_num2);
moy_y = mean(masse_num2);

a = sum((flipper_num2 - moy_x) .* (masse_num2 - moy_y)) / sum((flipper_num2 - moy_x).^2);
b = moy_y - a * moy_x;

printf("\n=== QUESTION 18 ===\n");
printf("Droite de régression : masse = %.4f * nageoires + (%.4f)\n", a, b);
printf("====================\n");

// Partie Q20
masse_pred = a * flipper_num2 + b;
SS_res = sum((masse_num2 - masse_pred).^2);
SS_tot = sum((masse_num2 - moy_y).^2);
R2 = 1 - SS_res / SS_tot;

printf("\n=== QUESTION 20 ===\n");
printf("Coefficient de détermination R² : %.4f\n", R2);
printf("====================\n");

// Partie Q21
nageoire_cible = 210;
masse_estimee  = a * nageoire_cible + b;

printf("\n=== QUESTION 21 ===\n");
printf("Masse estimée pour nageoires = %d mm : %.2f g\n", nageoire_cible, masse_estimee);
printf("====================\n");

scf(5); clf();
especes  = ["Adelie", "Chinstrap", "Gentoo"];
couleurs = ["b", "r", "g"];
species2 = data(2:$, 1);
species2 = species2(masque);

printf("\n=== QUESTION 22 ===\n");
printf("-------------------------------------------------------------\n");
printf("| Espèce    |   Pente (a) |  Intercept (b) |    r (Pearson) |\n");
printf("-------------------------------------------------------------\n");

x_range  = linspace(min(flipper_num2), max(flipper_num2), 100);
handles  = [];

for i = 1:3
    idx   = (species2 == especes(i));
    x_esp = flipper_num2(idx);
    y_esp = masse_num2(idx);

    moy_xe = mean(x_esp);
    moy_ye = mean(y_esp);
    a_esp  = sum((x_esp - moy_xe) .* (y_esp - moy_ye)) / sum((x_esp - moy_xe).^2);
    b_esp  = moy_ye - a_esp * moy_xe;
    r_esp  = pearson(x_esp, y_esp);

    // Points en transparence (marqueur seul)
    plot(x_esp, y_esp, couleurs(i) + ".");

    // Droite de régression — on garde le handle
    h = plot(x_range, a_esp * x_range + b_esp, couleurs(i) + "-");
    handles = [handles, h(1)];

    printf("| %-9s | %11.4f | %14.4f | %14.4f |\n", especes(i), a_esp, b_esp, r_esp);
end

printf("-------------------------------------------------------------\n");
printf("====================\n");

title("Droites de régression par espèce", "fontsize", 4);
xlabel("Longueur des nageoires (mm)", "fontsize", 3);
ylabel("Masse corporelle (g)", "fontsize", 3);

// Légende avec seulement les 3 droites, bien associées
legend(handles, ["Adelie", "Chinstrap", "Gentoo"], 4);
