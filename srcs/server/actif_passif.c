/*
** EPITECH PROJECT, 2018
** mod
** File description:
** mod
*/

#include "server.h"

char *get_host_ip(socklen_t client_fd)
{
	struct sockaddr_in server;
	int server_size;

	server_size = sizeof(server);
	if (getsockname(client_fd, &server, (socklen_t *)&server_size) == -1)
		return (fprintf(stderr, "getsockname host IP fail.\n"), NULL);
	return (inet_ntoa(server.sin_addr));
}

char *transform_ip(char *ip)
{
	for (size_t index = 0 ; ip[index] != '\0' ; ++index) {
		if (ip[index] == '.')
			ip[index] = ',';
	}
	return (ip);
}

t_bool close_last_chanel(t_session *session)
{
	int res = 0;

	if (session->data.ip != NULL)
		free(session->data.ip);
	session->data.ip = NULL;
	if (res != -1 && fcntl(session->data.socket_fd, F_GETFD) != -1)
		res = close(session->data.socket_fd);
	if (res != -1 && fcntl(session->data.socket_fd, F_GETFD) != -1)
		res = close(session->data.client_fd);
	if (res == -1) {
		return (fprintf(stderr, "CLOSE FAIL WHEN CLOSE CHANEL.\n"),
			false);
	}
	session->data.socket_fd = -1;
	session->data.client_fd = -1;
	session->data.mod = NONE;
	return (true);
}

t_bool pasv_function(int client_fd, char *buffer, t_session *session)
{
	t_ftp ftp = {NULL, NULL, -1, session->data.port, {}};
	char *msg = NULL;

	(void) buffer;
	if (session->data.mod != NONE && close_last_chanel(session) == false)
		return (false);
	for (t_bool res = false ; res == false
		&& ftp.port == session->data.port ;) {
		ftp.port = my_random(3000, 50000);
		res = create_socket(&ftp, "TCP");
	}
	session->data.mod = PASSIF;
	session->data.socket_fd = ftp.socket_fd;
	session->data.ip = strdup(get_host_ip(client_fd));
	session->data.port = ftp.port;
	session->data.ip = transform_ip(session->data.ip);
	asprintf(&msg, "%s (%s,%i,%i)\n",  SERVER_PASV, session->data.ip,
		(session->data.port - (session->data.port % 256)) / 256,
		session->data.port % 256);
	return (write(client_fd, msg, strlen(msg)), free(msg), true);
}

t_bool port_function(int client_fd, char *buffer, t_session *session)
{
	t_session tmp = {NULL, NULL, false, NULL, NULL,
			{NONE, -1, -1, -1, NULL}, NULL};

	(void) client_fd;
	if (session->data.mod != NONE && close_last_chanel(session) == false)
		return (false);
	++buffer;
	if (buffer[0] == '\0' || buffer[0] == '\n')
		return (write(client_fd, SERVER_MISSING_ARGUMENT,
			strlen(SERVER_MISSING_ARGUMENT)), true);
	session->data.port = atoi(buffer);
	session->data.ip = strdup(session->client_ip);
	tmp.data.port = session->data.port;
	tmp.data.ip = session->data.ip;
	if (connect_socket(&tmp, "TCP") == false)
		return (fprintf(stderr, "PORT connect to client failed.\n"),
			false);
	session->data.socket_fd = tmp.data.socket_fd;
	session->data.mod = ACTIF;
	return (write(client_fd, SERVER_OK, strlen(SERVER_OK)), true);
}
