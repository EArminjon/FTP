/*
** EPITECH PROJECT, 2018
** stor
** File description:
** stor
*/

#include "client.h"

t_bool upload_send(char *cmd, t_client *client)
{
	char *buffer = NULL;
	size_t size = 0;
	FILE *fd = fopen(cmd, "r");

	if (fd == NULL)
		return (close(client->data.socket_fd), fprintf(stderr,
		"No such file or directory\n"), true);
	while (getline(&buffer, &size, fd) > 0) {
		write(client->data.socket_fd, buffer, strlen(buffer));
		buffer != NULL ? free(buffer), buffer = NULL : (char *) buffer;
	}
	buffer != NULL ? free(buffer) : (char *) buffer;
	return (fclose(fd), close(client->data.socket_fd), true);
}

t_bool upload_function(int client_fd, char *cmd, t_client *client)
{
	char *b = NULL;
	size_t size = 0;
	t_client new = {-1, client->data.port, client->data.ip, {}};
	FILE *fd_client = fdopen(dup(client_fd), "r");

	if (fd_client == NULL)
		return (fprintf(stderr, "Client stor fdopen fail.\n"), false);
	else if (getline(&b, &size, fd_client) <= 0 || b == NULL)
		return (fprintf(stderr, "stor getline fail.\n"), false);
	fclose(fd_client);
	if (printf("%s", b) >= 0 && strncmp(b, "150 ", strlen("150 ")) != 0)
	return (free(b), read_server(client));
	b != NULL ? free(b), b = NULL : (char *) b;
	if (connect_socket(&new, "TCP") == false)
		return (fprintf(stderr, "Create data socket: fail.\n"), false);
	client->data.socket_fd = new.socket_fd;
	if (++cmd && cmd[0] == '\0')
		return (close(client->data.socket_fd), fprintf(stderr,
		"STOR need a path to file argument.\n"), read_server(client));
	return (upload_send(cmd, client));
}
