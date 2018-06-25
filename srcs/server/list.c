/*
** EPITECH PROJECT, 2018
** list
** File description:
** list
*/

#include "server.h"

int is_directory(const char *path)
{
	struct stat statbuf;

	if (stat(path, &statbuf) != 0)
		return 0;
	return (S_ISDIR(statbuf.st_mode));
}

char *create_cmds(char *buffer, t_session *session)
{
	char *cmd = NULL;
	char path[PATH_MAX + 1];

	if (buffer[0] == '\0' || buffer[0] == '\n' ||
	(strcmp(realpath(buffer, path), session->root_directory) == 0))
		asprintf(&cmd, "%s", "/bin/ls -ld *");
	else if (is_directory(buffer))
		asprintf(&cmd, "%s %s%s", "/bin/ls -ld", buffer, "/*");
	else
		asprintf(&cmd, "%s %s", "/bin/ls -ld", buffer);
	return (cmd);
}

t_bool list_send(int fd, char *buffer, t_session *session)
{
	FILE *fp = NULL;
	char *cmd = NULL;
	size_t size = 0;

	if (buffer != NULL)
		cmd = create_cmds(buffer, session);
	else
		asprintf(&cmd, "%s", "/bin/ls -ld *");
	fp = popen(cmd, "r");
	if (fp == NULL)
		return (write(fd, SERVER_CMD_NOT_FOUND,
			strlen(SERVER_CMD_NOT_FOUND)), false);
	cmd != NULL ? free(cmd), cmd = NULL : (char *) cmd;
	while (getline(&cmd , &size, fp) > 0) {
		write(session->data.client_fd, cmd, strlen(cmd));
		cmd != NULL ? free(cmd), cmd = NULL : (char *) cmd;
	}
	cmd != NULL ? free(cmd), cmd = NULL : (char *) cmd;
	pclose(fp);
	return (true);
}

t_bool list_function(int fd, char *buffer, t_session *session)
{
	++buffer;
	buffer = strtok(buffer, ";");
	if (session->data.mod == NONE)
		return (write(fd, SERVER_PORT, strlen(SERVER_PORT)), true);
	write(fd, SERVER_OPEN, strlen(SERVER_OPEN));
	if (wait_connection(session) == false)
		return (false);
	if (list_send(fd, buffer, session) == false)
		return (true);
	if (close(session->data.client_fd) == -1)
		return (fprintf(stderr, "LIST: Close fail.\n"), false);
	return (write(fd, SERVER_CLOSE, strlen(SERVER_CLOSE)), true);
}
