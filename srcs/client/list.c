/*
** EPITECH PROJECT, 2018
** list
** File description:
** list
*/

#include "client.h"

t_bool list_read(t_client *client)
{
	char *b = NULL;
	size_t size = 0;
	FILE *fd = fdopen(client->data.socket_fd, "r");

	if (fd == NULL)
		return (fprintf(stderr, "List chanel opening fail.\n"), false);
	while (getline(&b, &size, fd) > 0) {
		printf("%s", b);
		b != NULL ? free(b), b = NULL : (char *) b;
	}
	b != NULL ? free(b), b = NULL : (char *) b;
	return (fclose(fd), true);
}

t_bool list_function(int client_fd, char *cmd, t_client *client)
{
	char *b = NULL;
	size_t size = 0;
	t_client new = {-1, client->data.port, client->data.ip, {}};
	FILE *fd_client = fdopen(dup(client_fd), "r");

	(void) cmd;
	if (fd_client == NULL)
		return (fprintf(stderr, "Client list fdopen fail.\n"));
	else if ((getline(&b, &size, fd_client) <= 0) || b == NULL)
		return (fprintf(stderr, "list getline fail.\n"), false);
	fclose(fd_client);
	if (strncmp(b, "150 ", strlen("150 ")) != 0)
		return (printf("%s", b), free(b), read_server(client));
	else if (connect_socket(&new, "TCP") == false)
		return (fprintf(stderr, "Create data socket: fail.\n"), false);
	printf("%s", b);
	client->data.socket_fd = new.socket_fd;
	if (list_read(client) == false)
		return (false);
	return (b != NULL ? free(b), true : true);
}
