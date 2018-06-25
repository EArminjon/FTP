/*
** EPITECH PROJECT, 2018
** session
** File description:
** session
*/

#include "server.h"

void create_session(t_session *session, char *root, char *home, char *ip)
{
	session->login = NULL;
	session->pass = NULL;
	session->home_directory = NULL;
	session->authent = false;
	asprintf(&session->home_directory, "%s", home);
	asprintf(&session->root_directory, "%s", root);
	session->data.mod = NONE;
	session->data.socket_fd = -1;
	session->data.client_fd = -1;
	session->data.port = -1;
	session->data.ip = NULL;
	session->client_ip = strdup(ip);
}

t_bool delete_session(t_session *session)
{
	int res = 0;

	free(session->login);
	free(session->pass);
	free(session->home_directory);
	free(session->root_directory);
	free(session->data.ip);
	free(session->client_ip);
	if (session->data.mod != NONE) {
		if (res != -1 && fcntl(session->data.socket_fd, F_GETFD) != -1)
			res = close(session->data.socket_fd);
		if (res != -1 && fcntl(session->data.client_fd, F_GETFD) != -1)
			res = close(session->data.client_fd);
		if (res != 0)
			return (false);
	}
	return (true);
}

t_bool ftp_destroy(t_ftp *ftp)
{
	free(ftp->home_directory);
	free(ftp->root_directory);
	if (fcntl(ftp->socket_fd, F_GETFD) != -1)
		if (close(ftp->socket_fd) == -1)
			return (false);
	return (true);
}
