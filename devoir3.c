#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void split(const char *nomFichier, int taille) {
    int fd = open(nomFichier, O_RDONLY);
    if (fd == -1) {
        printf("Erreur : Impossible d'ouvrir le fichier source '%s'.\n", nomFichier);
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    int compteur = 1;

    while (1) {
        char nomPartie[256];
        snprintf(nomPartie, sizeof(nomPartie), "%s.part%d", nomFichier, compteur);

        int fdPartie = open(nomPartie, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fdPartie == -1) {
            printf("Erreur : Impossible de créer la partie '%s'.\n", nomPartie);
            close(fd);
            exit(EXIT_FAILURE);
        }

        int restant = taille;
        while (restant > 0) {
            int aLire = (restant < (int)sizeof(buffer)) ? restant : sizeof(buffer);
            int lus = read(fd, buffer, aLire);
            if (lus == -1) {
                printf("Erreur : Lecture échouée sur le fichier source '%s'.\n", nomFichier);
                close(fd);
                close(fdPartie);
                exit(EXIT_FAILURE);
            }
            if (lus == 0) break; // Fin du fichier

            int ecrit = write(fdPartie, buffer, lus);
            if (ecrit == -1) {
                printf("Erreur : Écriture échouée dans la partie '%s'.\n", nomPartie);
                close(fd);
                close(fdPartie);
                exit(EXIT_FAILURE);
            }

            restant -= ecrit;
        }

        close(fdPartie);
        compteur++;
        if (restant > 0) break; // Fin du fichier
    }

    close(fd);
    printf("Fichier '%s' découpé en %d parties.\n", nomFichier, compteur - 1);
}

void join(const char *nomFichier, int nbParties, char *parties[]) {
    int fdFinal = open(nomFichier, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fdFinal == -1) {
        printf("Erreur : Impossible de créer le fichier final '%s'.\n",nomFichier);
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    for (int i = 0; i < nbParties; i++) {
        int fdPartie = open(parties[i], O_RDONLY);
        if (fdPartie == -1) {
            printf("Erreur : Impossible d'ouvrir la partie '%s'.\n", parties[i]);
            close(fdFinal);
            exit(EXIT_FAILURE);
        }

        while (1) {
            int lus = read(fdPartie, buffer, sizeof(buffer));
            if (lus == -1) {
                printf("Erreur : Lecture échouée sur la partie '%s'.\n", parties[i]);
                close(fdPartie);
                close(fdFinal);
                exit(EXIT_FAILURE);
            }
            if (lus == 0) break; // Fin de la partie

            int ecrit = write(fdFinal, buffer, lus);
            if (ecrit == -1) {
                printf("Erreur : Écriture échouée dans le fichier final '%s'.\n", nomFichier);
                close(fdPartie);
                close(fdFinal);
                exit(EXIT_FAILURE);
            }
        }

        close(fdPartie);
    }

    close(fdFinal);
    printf("Fichier '%s' recréé à partir des parties fournies.\n", nomFichier);
}

int main(int argc, char *argv[]) {

    if (argc ==3) 
    	{
     
        char *nomFichier = argv[1];
        int taille = atoi(argv[2]);
        if (taille <= 0) {
            printf("Erreur : La taille doit être un entier positif.\n");
            return EXIT_FAILURE;
        }

        split(nomFichier, taille);
     }
     else
     {
       char *nomFichier = argv[1];
       int nbParties = argc - 2;
        join(nomFichier, nbParties, &argv[2]);
        
      }
      return 0;
 
}
