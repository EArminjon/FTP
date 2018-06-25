/*
** EPITECH PROJECT, 2018
** link
** File description:
** link
*/

#include "client.h"

t_bool check_status(t_client *client)
{
	int error = 0;
	socklen_t len = sizeof(error);

	getsockopt(client->socket_fd, SOL_SOCKET, SO_ERROR, &error, &len);
	if (error != 0 || my_break(false) == true)
		return (false);
	return (true);
}

t_bool pasv_function(int client_fd, char *buffer, t_client *client)
{
	size_t index = 0;

	(void) client_fd;
	if (client->data.mod != NONE && (close_last_chanel(client) == false))
		return (false);
	buffer = &strchr(buffer, '(')[1];
	client->data.ip = calloc(strlen(buffer) + 1, sizeof(char));
	for (int nb = 0 ; buffer[index] != '\0' ;  ++index) {
		if (buffer[index] == ',') {
			buffer[index] = '.';
			++nb;
			if (nb >= 4)
			break;
		}
		client->data.ip[index] = buffer[index];
	}
	client->data.ip[index] = '\0';
	client->data.port = atoi(&buffer[index + 1]) * 256;
	client->data.port += atoi(&strchr(&buffer[index + 1], ',')[1]);
	return (true);
}

t_bool quit_function(int client_fd, char *cmd, t_client *client)
{
	(void) client_fd;
	(void) cmd;
	(void) client;
	my_break(true);
	return (true);
}

void my_handler(int s)
{
	(void) s;
	my_break(true);
}

t_bool create_prompt(t_client *client)
{
	FILE *fd;
	t_mod mod = LISTEN;
	t_bool res = true;

	fd = fdopen(client->socket_fd, "rwx");
	if (fd == NULL)
		return (fprintf(stderr, "fdopen failed.\n"), false);
	while (my_break(false) == false && res == true) {
		if (mod == LISTEN)
			res = listen_server(fd, client);
		else
			res = read_server(client);
		mod = (mod == LISTEN ? READ : LISTEN);
	}
	fclose(fd);
	return (true);
}
