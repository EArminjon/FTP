/*
** EPITECH PROJECT, 2018
** stor, pasv
** File description:
** action before
*/

#include "client.h"

t_bool download_send(char *cmd, t_client *client)
{
	int file_fd;
	FILE *fd = fdopen(client->data.socket_fd, "r");
	char *buffer = NULL;
	size_t size = 0;

	if (fd == NULL)
	return (fprintf(stderr, "Retr chanel opening fail.\n"), false);
	++cmd;
	if (strchr(cmd, '/'))
		cmd = &(strchr(cmd, '/')[1]);
	file_fd = open(cmd, O_WRONLY | O_CREAT, 0644);
	if (file_fd == -1)
		return (fprintf(stderr, "Retr open fail.\n"), false);
	while (getline(&buffer, &size, fd) > 0) {
		printf("data:%s", buffer);
		write(file_fd, buffer, strlen(buffer));
		buffer != NULL ? free(buffer), buffer = NULL : (char *) buffer;
	}
	buffer != NULL ? free(buffer) : (char *) buffer;
	return (close(file_fd), fclose(fd), true);
}

t_bool download_function(int client_fd, char *cmd, t_client *client)
{
	char *buffer = NULL;
	size_t size = 0;
	t_client new = {-1, client->data.port, client->data.ip, {}};
	FILE *fd_client = fdopen(dup(client_fd), "r");

	if (fd_client == NULL)
		return (fprintf(stderr, "Client retr fdopen fail.\n"));
	else if (getline(&buffer, &size, fd_client) <= 0 || buffer == NULL)
		return (fprintf(stderr, "Stor: getline fail.\n"), false);
	fclose(fd_client);
	if (strncmp(buffer, "150 ", strlen("150 ")) != 0)
	return (printf("%s", buffer), free(buffer), read_server(client));
	printf("%s", buffer);
	buffer != NULL ? free(buffer), buffer = NULL : (char *) buffer;
	size = 0;
	if (connect_socket(&new, "TCP") == false)
	return (fprintf(stderr, "Create data socket: fail.\n"), false);
	client->data.socket_fd = new.socket_fd;
	return (download_send(cmd, client));
}

t_bool port_function(int c_fd, char *cmd, t_client *c)
{
	char *msg = NULL;
	struct sockaddr_in s_in;
	socklen_t size = sizeof(s_in);

	if (c->data.mod != NONE && (close_last_chanel(c) == false))
		return ((void) c_fd, false);
	++cmd;
	if (cmd[0] == '\0' || cmd[0] == '\n')
	return (fprintf(stderr, "Need a port number.\n"), read_server(c));
	c->data.port = atoi(cmd);
	if (c->data.port <= 0 || create_socket(&c->data, "TCP") == false)
	return (fprintf(stderr, "Invalid port.\n"), read_server(c));
	asprintf(&msg, "PORT %i\r\n", c->data.port);
	write(c->socket_fd, msg, strlen(msg));
	c->data.client_fd = accept(c->data.socket_fd, SOCKADDR&s_in, &size);
	if (c->data.client_fd == -1)
		return (free(msg), false);
	c->data.mod = ACTIF;
	c->data.ip = strdup(c->ip);
	return (free(msg), true);
}

t_bool patch(t_client *client, char **b)
{
	char *msg = NULL;
	char *file = NULL;

	if (check_status(client) == false)
		return (false);
	if (strchr(*b, '/'))
		file = &(strchr(*b, '/')[1]);
	else if (strchr(*b, ' '))
		file = &(strchr(*b, ' ')[1]);
	else
		file = &((*b)[strlen("STOR")]);
	asprintf(&msg, "STOR %s", file);
	write(client->socket_fd, msg, strlen(msg));
	*b = strtok(*b, MY_EOT);
	free(msg);
	return (true);
}

t_bool action_before_send(t_client *client, char *b)
{
	static t_pointer_cmd c[] = {{"PORT", strlen("PORT"), port_function},
		{"STOR", strlen("STOR"), upload_function},
		{"RETR", strlen("RETR"), download_function},
		{"LIST", strlen("LIST"), list_function}};

	if (my_strncmp(b, "STOR", strlen("STOR")) == 0) {
		if (patch(client, &b) == false)
			return (false);
	} else if (my_strncmp(b, "PORT", strlen("PORT")) != 0) {
		if (check_status(client) == false)
			return (false);
		write(client->socket_fd, b, strlen(b));
		b = strtok(b, MY_EOT);
	}
	for (size_t i = 0 ; i < 4 ; ++i)
		if (my_strncmp(b, c[i].name, c[i].size) == 0)
			return (c[i].ptr_function(client->socket_fd,
					&b[c[i].size], client));
	return (true);
}
