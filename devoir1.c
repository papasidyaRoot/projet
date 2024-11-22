#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

int fonction_f(int n) {
    int produit=1;
     for(int i=0;i<n;i++)
       produit*=2;   
return produit;   
}

int fonction_g(int n) {
    return 2 * n;
}

int main(int argc, char *argv[]) {
    int opt;
    int valeur_f = 0, valeur_g = 0; // Flags pour savoir si les options ont été appelées
    int resultat = 0, n = 0;

    while ((opt = getopt(argc, argv, "f:g:")) != -1) {
        switch (opt) {
            case 'f':
                n = atoi(optarg);
                valeur_f = 1;
                if (valeur_g) {  // Si `g` a été défini avant
                    resultat = fonction_f(fonction_g(n));
                } else {
                    resultat = fonction_f(n);
                }
                break;
            case 'g':
                n = atoi(optarg);
                valeur_g = 1;
                if (valeur_f) {  // Si `f` a été défini avant
                    resultat = fonction_g(fonction_f(n));
                } else {
                    resultat = fonction_g(n);
                }
                break;
            default:
                fprintf(stderr, "Usage: %s -f entier | -g entier | -f entier -g | -g entier -f\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (valeur_f || valeur_g) {
        printf("Résultat : %d\n", resultat);
    } else {
        fprintf(stderr, "Usage: %s -f entier | -g entier | -f entier -g | -g entier -f\n", argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

