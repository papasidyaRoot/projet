#include "common.h"

int main() 
{
    int server_fd, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int compteur = 0;  // Initialisation du compteur

	// Création de la socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		error("Erreur de création de la socket");

	// Configuration de l'adresse
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Attachement de la socket au port
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		error("Erreur lors du bind");

	// Écoute des connexions
	if (listen(server_fd, 3) < 0)
		error("Erreur lors de l'écoute");

	printf("Serveur en attente de connexion...\n");

	// Acceptation d'une connexion client
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
	        error("Erreur lors de l'acceptation");

	printf("Client connecté.\n");

	while (1) 
	{
		char buffer[BUFFER_SIZE] = {0};

		// Envoi du compteur au client
		sprintf(buffer, "%d", compteur);
		send(new_socket, buffer, strlen(buffer), 0);

		// Réception du compteur du client
		read(new_socket, buffer, BUFFER_SIZE);
		compteur = atoi(buffer);

		printf("Compteur reçu du client : %d\n", compteur);

		// Incrémentation et pause
		compteur++;
		sleep(1);
	}

	close(new_socket);
	close(server_fd);
	return 0;
}
