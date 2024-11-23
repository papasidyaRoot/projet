#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 // Nombre de threads

sem_t semaphore;
int current_thread = 0; // Indicateur du thread en cours

void* thread_function(void* arg) {
    int thread_id = *(int*)arg;

    while (1) {
        // Attendre que ce soit son tour
        sem_wait(&semaphore);
        
        // Si c'est son tour (vérifier l'indice)
        if (thread_id == current_thread) {
            printf("Thread %d s'exécute\n", thread_id + 1);
            // Simuler un travail
            usleep(500000);

            // Passer le tour au thread suivant
            current_thread = (current_thread + 1) % N;
            sem_post(&semaphore); // Libérer le sémaphore pour le prochain thread
        } else {
            // Si ce n'est pas son tour, il attend
            sem_post(&semaphore); // Relâcher le sémaphore pour les autres threads
        }
    }

    return NULL;
}

int main() {
    pthread_t threads[N];
    int thread_ids[N];

    // Initialiser le sémaphore
    sem_init(&semaphore, 0, 1); // Un seul sémaphore partagé entre les threads

    // Créer les threads
    for (int i = 0; i < N; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]) != 0) {
            perror("Erreur lors de la création du thread");
            exit(EXIT_FAILURE);
        }
    }

    // Rejoindre les threads (inaccessible dans ce cas à cause de la boucle infinie)
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    // Nettoyer le sémaphore
    sem_destroy(&semaphore);

    return 0;
}
