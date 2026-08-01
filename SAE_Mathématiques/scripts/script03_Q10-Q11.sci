funcprot(0)

function q = quartile(x, p)
    x = gsort(x, "g", "i");
    n = length(x);
    h = (n - 1) * p + 1;
    lo = floor(h);
    hi = ceil(h);
    if lo == hi then
        q = x(lo);
    else
        q = x(lo) + (h - lo) * (x(hi) - x(lo));
    end
endfunction

// Partie 10
scf(0);
clf();
masse_min = min(masse_num);
masse_max = max(masse_num);

q_masse(1) = quartile(masse_num, 0.25);
q_masse(2) = quartile(masse_num, 0.50);
q_masse(3) = quartile(masse_num, 0.75);

cx = 0;
larg = 0.4;

plot([cx-larg, cx+larg, cx+larg, cx-larg, cx-larg], ...
     [q_masse(1), q_masse(1), q_masse(3), q_masse(3), q_masse(1)], "b-");
plot([cx-larg, cx+larg], [q_masse(2), q_masse(2)], "r-");
plot([cx, cx], [q_masse(3), masse_max], "b--");
plot([cx, cx], [q_masse(1), masse_min], "b--");
plot([cx-larg*0.5, cx+larg*0.5], [masse_max, masse_max], "b-");
plot([cx-larg*0.5, cx+larg*0.5], [masse_min, masse_min], "b-");

offset_x = cx + larg + 0.05;
xstring(offset_x, masse_max  - 30, "Max : " + string(masse_max)  + " g");
xstring(offset_x, q_masse(3) - 30, "Q3  : " + string(q_masse(3)) + " g");
xstring(offset_x, q_masse(2) - 30, "Méd : " + string(q_masse(2)) + " g");
xstring(offset_x, q_masse(1) - 30, "Q1  : " + string(q_masse(1)) + " g");
xstring(offset_x, masse_min  - 30, "Min : " + string(masse_min)  + " g");

gca().axes_visible = ["off", "on"];
gca().data_bounds = [-1, 2000; 1.5, 7000];
title("Boîte à moustaches de la masse corporelle globale (en g)", "fontsize", 4);
ylabel("Masse corporelle (g)", "fontsize", 3);

printf("\n=== QUESTION 10 ===\n");
printf("-----------------------------\n");
printf("| Indicateur | Valeur (g)   |\n");
printf("-----------------------------\n");
printf("| Minimum    | %12.2f |\n", masse_min);
printf("| Q1 (25%%)   | %12.2f |\n", q_masse(1));
printf("| Médiane    | %12.2f |\n", q_masse(2));
printf("| Q3 (75%%)   | %12.2f |\n", q_masse(3));
printf("| Maximum    | %12.2f |\n", masse_max);
printf("-----------------------------\n");
printf("====================\n");

// Partie Q11
scf(1);
clf();

especes   = ["Adelie", "Chinstrap", "Gentoo"];
positions = [0, 1, 2];
larg      = 0.3;
couleurs  = ["b", "r", "g"];

stats = zeros(3, 5);
for i = 1:3
    m_esp = masse_texte(species == especes(i) & masse_texte <> "NA");
    m_num = strtod(m_esp);
    stats(i, 1) = min(m_num);
    stats(i, 2) = quartile(m_num, 0.25);
    stats(i, 3) = quartile(m_num, 0.50);
    stats(i, 4) = quartile(m_num, 0.75);
    stats(i, 5) = max(m_num);
end

for i = 1:3
    cx    = positions(i);
    col   = couleurs(i);
    s_min = stats(i, 1);
    s_q1  = stats(i, 2);
    s_med = stats(i, 3);
    s_q3  = stats(i, 4);
    s_max = stats(i, 5);

    plot([cx-larg, cx+larg, cx+larg, cx-larg, cx-larg], ...
         [s_q1, s_q1, s_q3, s_q3, s_q1], col + "-");
    plot([cx-larg, cx+larg], [s_med, s_med], col + "-");
    plot([cx, cx], [s_q3, s_max], col + "--");
    plot([cx, cx], [s_q1, s_min], col + "--");
    plot([cx-larg*0.5, cx+larg*0.5], [s_max, s_max], col + "-");
    plot([cx-larg*0.5, cx+larg*0.5], [s_min, s_min], col + "-");

    offset_x = cx + larg + 0.03;
    xstring(offset_x, s_max - 30, "Max:" + string(s_max));
    xstring(offset_x, s_q3  - 30, "Q3 :" + string(s_q3));
    xstring(offset_x, s_med - 30, "Méd:" + string(s_med));
    xstring(offset_x, s_q1  - 30, "Q1 :"  + string(s_q1));
    xstring(offset_x, s_min - 30, "Min:" + string(s_min));
end

gca().axes_visible = ["off", "on"];
gca().data_bounds = [-0.6, 2000; 2.9, 7000];
title("Boîtes à moustaches de la masse corporelle par espèce (en g)", "fontsize", 4);
ylabel("Masse corporelle (g)", "fontsize", 3);
xstring(-0.1, 2150, "Adelie");
xstring(0.9,  2150, "Chinstrap");
xstring(1.9,  2150, "Gentoo");

printf("\n=== QUESTION 11 ===\n");
printf("--------------------------------------------------\n");
printf("| Indicateur | Adelie   | Chinstrap | Gentoo    |\n");
printf("--------------------------------------------------\n");
printf("| Minimum    |  %7.0f |   %7.0f | %7.0f   |\n", stats(1,1), stats(2,1), stats(3,1));
printf("| Q1 (25%%)   |  %7.0f |   %7.0f | %7.0f   |\n", stats(1,2), stats(2,2), stats(3,2));
printf("| Médiane    |  %7.0f |   %7.0f | %7.0f   |\n", stats(1,3), stats(2,3), stats(3,3));
printf("| Q3 (75%%)   |  %7.0f |   %7.0f | %7.0f   |\n", stats(1,4), stats(2,4), stats(3,4));
printf("| Maximum    |  %7.0f |   %7.0f | %7.0f   |\n", stats(1,5), stats(2,5), stats(3,5));
printf("--------------------------------------------------\n");
printf("====================\n");
