/*
** EPITECH PROJECT, 2018
** dl
** File description:
** dl
*/

#include "server.h"

void upload(FILE *fd, int target_fd, char *buffer)
{
	t_bool fail = true;
	char *data = NULL;
	size_t size = 0;

	while (getline(&data, &size, fd) > 0) {
		fail = false;
		write(target_fd, data, strlen(data));
		data != NULL ? free(data), data = NULL : (char *) data;
	}
	data != NULL ? free(data), data = NULL : (char *) data;
	if (fail == true) {
		asprintf(&data, "rm -f %s", buffer);
		system(data);
		free(data);
	}
	fclose(fd);
}

t_bool upload_send_function(int client_fd, char *buffer, t_session *session)
{
	FILE *f = NULL;
	int target_fd;

	f = fdopen(session->data.client_fd, "r");
	if (f == NULL)
		return (fprintf(stderr, "data chanel opening fail.\n"), false);
	target_fd = open(buffer, O_WRONLY | O_CREAT, 0644);
	if (target_fd == -1)
		return (fclose(f), write(client_fd, SERVER_DATA_TRANSFERT_FAIL,
			strlen(SERVER_DATA_TRANSFERT_FAIL)), true);
	upload(f, target_fd, buffer);
	write(client_fd, SERVER_CLOSE, strlen(SERVER_CLOSE));
	if (close(target_fd) == -1)
		return (fprintf(stderr, "STOR: Close failed.\n"));
	return (true);
}

t_bool upload_function(int client_fd, char *buffer, t_session *session)
{
	if (session->data.mod == NONE)
		return (write(client_fd, SERVER_PORT,
			strlen(SERVER_PORT)), true);
	++buffer;
	if (buffer[0] == '\0' || buffer[0] == '\n')
		return (write(client_fd, SERVER_DATA_TRANSFERT_EMPTY,
			strlen(SERVER_DATA_TRANSFERT_EMPTY)), true);
	write(client_fd, SERVER_OPEN, strlen(SERVER_OPEN));
	if (wait_connection(session) == false)
		return (false);
	if (fcntl(session->data.client_fd, F_GETFD) == -1)
		return (fprintf(stderr, "Server: close connection.\n"), true);
	return (upload_send_function(client_fd, buffer, session));
}

t_bool download_send(int c, char *b, t_session *s)
{
	char *data = NULL;
	size_t size = 0;
	FILE *fd = fopen(b, "r");

	if (fd == NULL)
		return (fprintf(stderr, "Retr chanel opening fail.\n"), false);
	while (getline(&data, &size, fd) > 0) {
		write(s->data.client_fd, data, strlen(data));
		data != NULL ? free(data), data = NULL : (char *) data;
	}
	data != NULL ? free(data), data = NULL : (char *) data;
	write(c, SERVER_CLOSE, strlen(SERVER_CLOSE));
	if ((fclose(fd) == EOF) || (close(s->data.client_fd) == -1))
		return (fprintf(stderr, "RETR: Close failed.\n"));
	return (true);
}

t_bool download_function(int c, char *b, t_session *s)
{
	if (s->data.mod == NONE)
		return (write(c, SERVER_PORT, strlen(SERVER_PORT)), true);
	++b;
	if (b[0] == '\0' || b[0] == '\n')
		return (write(c, SERVER_DATA_TRANSFERT_EMPTY,
			strlen(SERVER_DATA_TRANSFERT_EMPTY)), true);
	if (access(b, F_OK) == -1)
		return (write(c, SERVER_FILE_NOT_FOUND,
			strlen(SERVER_FILE_NOT_FOUND)), true);
	write(c, SERVER_OPEN, strlen(SERVER_OPEN));
	if (wait_connection(s) == false)
		return (false);
	return (download_send(c, b, s));
}
