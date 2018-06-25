/*
** EPITECH PROJECT, 2018
** socket
** File description:
** socket
*/

#include "server.h"

t_bool wait_connection(t_session *session)
{
	struct sockaddr_in data_client;
	socklen_t data_client_size;

	data_client_size = sizeof(data_client);
	session->data.client_fd = accept(session->data.socket_fd,
					(struct sockaddr *)&data_client,
					&data_client_size);
	if (session->data.client_fd == -1) {
		fprintf(stderr, "CREATION DE LA SOCKET DATA: FAIL.\n");
		return (false);
	}
	return (true);
}

t_bool create_socket(t_ftp *ftp, char *proto_name)
{
	struct protoent *pe = getprotobyname(proto_name);

	if (!pe)
		return (fprintf(stderr, "Getprotobyname fail\n"), false);
	ftp->socket_fd = socket(AF_INET, SOCK_STREAM, pe->p_proto);
	if (ftp->socket_fd == -1)
		return (fprintf(stderr, "Socket fail\n"), false);
	if (setsockopt(ftp->socket_fd, SOL_SOCKET, SO_REUSEADDR,
		&(int){1}, sizeof(int)) < 0)
		return (fprintf(stderr,  "setsockopt failed)"), false);
	ftp->s_in.sin_family = AF_INET;
	ftp->s_in.sin_port = htons(ftp->port);
	ftp->s_in.sin_addr.s_addr = INADDR_ANY;
	if ((bind(ftp->socket_fd, (const struct sockaddr *)&ftp->s_in,
		sizeof(ftp->s_in))) == -1) {
		if (close(ftp->socket_fd) == -1)
			return (fprintf(stderr, "Close 3 fail\n"), false);
		return (fprintf(stderr, "CLOSE 3\n"), false);
	}
	if (listen(ftp->socket_fd, 42) != -1)
		return (true);
	return (fprintf(stderr, "CLOSE 2\n") , false);
}

t_bool connect_socket(t_session *session, char *proto_name)
{
	struct protoent *pe;
	struct sockaddr_in s_in;

	pe = getprotobyname(proto_name);
	if (!pe)
		return (fprintf(stderr, "Getprotobyname fail\n"), false);
	session->data.socket_fd = socket(AF_INET, SOCK_STREAM, pe->p_proto);
	if (session->data.socket_fd == -1)
		return (fprintf(stderr, "Socket fail\n"), false);
	s_in.sin_family = AF_INET;
	s_in.sin_port = htons(session->data.port);
	s_in.sin_addr.s_addr = inet_addr(session->data.ip);
	if (connect(session->data.socket_fd,
		(struct sockaddr *)&s_in, sizeof(s_in)) != -1)
		return (true);
	if (close(session->data.socket_fd) == -1)
		return (fprintf(stderr, "Close 2 fail\n"), false);
	return (fprintf(stderr, "Close 2\n"), false);
}
