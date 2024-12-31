#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Fonction pour afficher un message d'erreur et quitter
void error(const char *msg) 
{
	perror(msg);
	exit(EXIT_FAILURE);
}

#endif // COMMON_H
