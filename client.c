#include "common.h"

int main() 
{
	int sock = 0;
	struct sockaddr_in serv_addr;

	// Création de la socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error("Erreur de création de la socket");

	// Configuration de l'adresse du serveur
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
		error("Adresse invalide ou non supportée");

	// Connexion au serveur
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("Connexion au serveur échouée");

	printf("Connecté au serveur.\n");

	while (1) 
	{
		char buffer[BUFFER_SIZE] = {0};

		// Réception du compteur du serveur
		read(sock, buffer, BUFFER_SIZE);
		int compteur = atoi(buffer);

		printf("Compteur reçu du serveur : %d\n", compteur);

		// Incrémentation et envoi au serveur
		compteur++;
		sprintf(buffer, "%d", compteur);
		send(sock, buffer, strlen(buffer), 0);

		sleep(1);
	}

	close(sock);
	return 0;
}
