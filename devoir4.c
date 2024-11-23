#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

// Variables globales
int valeur = 0;
int i=0;                 // La valeur partagée entre les 
int n;                          // Limite supérieure
sem_t sem1;                     // Sémaphore pour le thread 1
sem_t sem2;                     // Sémaphore pour le thread 2

// Fonction exécutée par le thread 1 (+1 jusqu'à n)
void *thread1_func(void *arg) {
    while (1) {
        sem_wait(&sem1);         // Attente du sémaphore 1
        printf("%d-> %d\n", ++i,++valeur);
        if (valeur == n) {
         for(int i=0;i<2*n;i++)
            sem_post(&sem2);     // Signal au thread 2
           
        }
    }
     pthread_exit(NULL);  // Fin du thread
}

// Fonction exécutée par le thread 2 (-1 jusqu'à -n)
void *thread2_func(void *arg) {
    while (1) {
        sem_wait(&sem2);         // Attente du sémaphore 2
        printf("%d-> %d\n", ++i,--valeur);
        if (valeur == -n) {
          for(int i=0;i<2*n;i++)
            sem_post(&sem1);     // Signal au thread 1
            
        }
    }
    pthread_exit(NULL);  // Fin du thread
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage : %s <n>\n", argv[0]);
        return EXIT_FAILURE;
    }

    n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Erreur : n doit être un entier positif.\n");
        return EXIT_FAILURE;
    }

    // Initialisation des sémaphores
    sem_init(&sem1, 0, n);  // Thread 1 commence
    sem_init(&sem2, 0, 0);  // Thread 2 attend

    // Création des threads
    pthread_t thread1, thread2;
    if (pthread_create(&thread1, NULL, thread1_func, NULL) != 0) {
        perror("Erreur de création du thread 1");
        return EXIT_FAILURE;
    }
    if (pthread_create(&thread2, NULL, thread2_func, NULL) != 0) {
        perror("Erreur de création du thread 2");
        return EXIT_FAILURE;
    }

    // Attente de la fin des threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Libération des sémaphores
    sem_destroy(&sem1);
    sem_destroy(&sem2);

    return EXIT_SUCCESS;
}