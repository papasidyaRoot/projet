#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define START_PORT 0    // Port de début
#define END_PORT 3000   // Port de fin
#define TARGET_IP "127.0.0.1"  // Adresse IP cible

void scan_ports(const char *ip, int start, int end) 
{
	struct sockaddr_in address;
	int sock, port;

	// Configuration de l'adresse du serveur
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ip);

	printf("Analyse des ports de %d à %d sur %s...\n", start, end, ip);

	for (port = start; port <= end; port++) 
	{
		// Création de la socket
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) 
		{
			perror("Erreur de création de socket");
			exit(EXIT_FAILURE);
		}

		// Configuration du port cible
		address.sin_port = htons(port);

		// Tentative de connexion
		if (connect(sock, (struct sockaddr *)&address, sizeof(address)) == 0)
			printf("Port ouvert : %d\n", port);  // Affichage du port ouvert

		// Fermeture de la socket
		close(sock);
	}
}

int main() 
{
	// Appel de la fonction de scan
	scan_ports(TARGET_IP, START_PORT, END_PORT);

	return 0;
}
