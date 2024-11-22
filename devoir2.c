#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

void afficher_matrice(int *matrice, int dimension) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            printf("%d ", matrice[i * dimension + j]);
        }
        printf("\n");
    }
}

void sauvegarder_matrice(int *matrice, int dimension, const char *nom_fichier, int mode_binaire) {
    int fd = open(nom_fichier, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    if (mode_binaire) {
        write(fd, matrice, dimension * dimension * sizeof(int));
    } else {
        char buffer[16];
        for (int i = 0; i < dimension * dimension; i++) {
            int taille = snprintf(buffer, sizeof(buffer), "%d ", matrice[i]);
            write(fd, buffer, taille);
            if ((i + 1) % dimension == 0) {
                write(fd, "\n", 1);
            }
        }
    }
    close(fd);
}

void charger_et_afficher_matrice(const char *nom_fichier, int dimension, int mode_binaire) {
    int fd = open(nom_fichier, O_RDONLY);
    if (fd == -1) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    int *matrice = malloc(dimension * dimension * sizeof(int));
    if (mode_binaire) {
        read(fd, matrice, dimension * dimension * sizeof(int));
    } else {
        char buffer[16];
        for (int i = 0; i < dimension * dimension; i++) {
            read(fd, buffer, sizeof(buffer));
            matrice[i] = atoi(buffer);
        }
    }
    close(fd);

    afficher_matrice(matrice, dimension);
    free(matrice);
}

int main(int argc, char *argv[]) {
    int creer = 0, afficher = 0, mode_binaire = 1, dimension = 0;
    char nom_fichier[256] = "";

    int opt;
    while ((opt = getopt(argc, argv, "cad:btf:")) != -1) {
        switch (opt) {
            case 'c':
                creer = 1;
                break;
            case 'a':
                afficher = 1;
                break;
            case 'd':
                dimension = atoi(optarg);
                break;
            case 'b':
                mode_binaire = 1;
                break;
            case 't':
                mode_binaire = 0;
                break;
            case 'f':
                strncpy(nom_fichier, optarg, sizeof(nom_fichier) - 1);
                break;
            default:
                fprintf(stderr, "Usage: %s -c|-a -d dimension -f nom_fichier [-b|-t]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (dimension <= 0 || strlen(nom_fichier) == 0) {
        fprintf(stderr, "Usage: %s -c|-a -d dimension -f nom_fichier [-b|-t]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (creer) {
        srand((unsigned)time(NULL));
        int *matrice = malloc(dimension * dimension * sizeof(int));
        for (int i = 0; i < dimension * dimension; i++) {
            matrice[i] = rand() % 100; // Valeurs aléatoires entre 0 et 99
        }

        sauvegarder_matrice(matrice, dimension, nom_fichier, mode_binaire);
        free(matrice);
    }

    if (afficher) {
        charger_et_afficher_matrice(nom_fichier, dimension, mode_binaire);
    }

    return EXIT_SUCCESS;
}

