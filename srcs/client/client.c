/*
** EPITECH PROJECT, 2018
** salut
** File description:
** balek
*/

#include "client.h"

int my_signal(void)
{
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);
	return 0;
}

t_bool my_client(t_client *client)
{
	t_bool res;

	my_signal();
	res = connect_socket(client, "TCP");
	if (res == false)
		return (false);
	res = create_prompt(client);
	return (res);
}

void create_client(t_client *client)
{
	client->ip = NULL;
	client->data.mod = NONE;
	client->data.socket_fd = -1;
	client->data.client_fd = -1;
	client->data.port = -1;
	client->data.ip = NULL;
}

t_bool delete_client(t_client *client)
{
	int res = 0;

	free(client->ip);
	if (client->data.ip != NULL)
		free(client->data.ip);
	if (res != -1 && fcntl(client->socket_fd, F_GETFD) != -1)
		res = close(client->socket_fd);
	if (res != -1 && fcntl(client->data.socket_fd, F_GETFD) != -1)
		res = close(client->data.socket_fd);
	if (res != -1 && fcntl(client->data.client_fd, F_GETFD) != -1)
		res = close(client->data.client_fd);
	if (res != 0) {
		fprintf(stderr, "Client destroy: close fail.\n");
		return (false);
	}
	return (true);
}

int main(int ac, char **av)
{
	t_bool res;
	t_client client;
	char *ip = NULL;

	if (ac != 3)
		return (fprintf(stderr, "Need two arguments.\n"), 1);
	ip = strdup(av[1]);
	if (check_ip(ip) == false)
		return (fprintf(stderr, "IP4 bad format.\n"), 1);
	free(ip);
	client.port = atoi(av[2]);
	if (client.port <= 0)
		return (fprintf(stderr, "Port need to positif.\n"), 1);
	create_client(&client);
	asprintf(&client.ip, "%s", av[1]);
	res = my_client(&client);
	if (delete_client(&client) == false)
		return (false);
	return (res);
}
