/*
** EPITECH PROJECT, 2018
** socket
** File description:
** scoket
*/

#include "client.h"

t_bool close_last_chanel(t_client *client)
{
	int res = 0;

	if (client->data.ip != NULL)
		free(client->data.ip);
	client->data.ip = NULL;
	if (res != -1 && fcntl(client->data.socket_fd, F_GETFD) != -1)
		res = close(client->data.socket_fd);
	if (res != -1 && fcntl(client->data.client_fd, F_GETFD) != -1)
		res = close(client->data.client_fd);
	if (res == -1) {
		fprintf(stderr, "CLOSE FAIL WHEN CLOSE LAST CHANEL.\n");
		return (false);
	}
	client->data.client_fd = -1;
	client->data.socket_fd = -1;
	client->data.port = -1;
	return (true);
}

t_bool create_socket(t_data_chanel *c, char *proto_name)
{
	struct protoent *pe = getprotobyname(proto_name);
	struct sockaddr_in s_in;

	if (!pe)
		return (fprintf(stderr, "Getprotobyname fail\n"), false);
	c->socket_fd = socket(AF_INET, SOCK_STREAM, pe->p_proto);
	if (c->socket_fd == -1)
		return (fprintf(stderr, "Socket fail\n"), false);
	if (setsockopt(c->socket_fd, SOL_SOCKET, SO_REUSEADDR,
		&(int){1}, sizeof(int)) < 0)
		return (fprintf(stderr,  "setsockopt failed)"), false);
	s_in.sin_family = AF_INET;
	s_in.sin_port = htons(c->port);
	s_in.sin_addr.s_addr = INADDR_ANY;
	if ((bind(c->socket_fd, (const struct sockaddr *)&s_in,
	sizeof(s_in))) == -1) {
	return (close(c->socket_fd) < 0 ? fprintf(stderr, "Close 3\n"), false :
		fprintf(stderr, "CLOSE 3\n"), false);
	}
	if (listen(c->socket_fd, 42) != -1)
		return (true);
	return (close(c->socket_fd) < 0 ? fprintf(stderr, "Close 2\n"), false :
		fprintf(stderr, "CLOSE 2\n") , false);
}

t_bool connect_socket(t_client *client, char *proto_name)
{
	struct protoent *pe;
	struct sockaddr_in s_in;

	pe = getprotobyname(proto_name);
	if (!pe)
		return (fprintf(stderr, "Getprotobyname fail\n"), false);
	client->socket_fd = socket(AF_INET, SOCK_STREAM, pe->p_proto);
	if (client->socket_fd == -1)
		return (fprintf(stderr, "Socket fail\n"), false);
	s_in.sin_family = AF_INET;
	s_in.sin_port = htons(client->port);
	s_in.sin_addr.s_addr = inet_addr(client->ip);
	if (connect(client->socket_fd,
		(struct sockaddr *)&s_in, sizeof(s_in)) == -1) {
		if (close(client->socket_fd) == -1)
			return (fprintf(stderr, "Close 2 fail\n"), false);
		return (fprintf(stderr, "Close 2\n"), false);
	}
	return (true);
}
