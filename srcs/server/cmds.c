/*
** EPITECH PROJECT, 2018
** LIST DELE
** File description:
** ls rm
*/

#include "server.h"

t_bool noop_function(int client_fd, char *cmd, t_session *session)
{
	(void) session;
	(void) client_fd;
	(void) cmd;
	return (write(client_fd, SERVER_OK, strlen(SERVER_OK)), true);
}

t_bool quit_function(int client_fd, char *cmd, t_session *session)
{
	(void) client_fd;
	(void) cmd;
	(void) session;
	return (write(client_fd, SERVER_QUIT, strlen(SERVER_QUIT)), false);
}

t_bool dele_function(int client_fd, char *buffer, t_session *session)
{
	int res = -1;
	char *cmd = NULL;

	(void) session;
	++buffer;
	buffer = strtok(buffer, MY_EOL);
	if (buffer && (buffer[0] == '\0' || buffer[0] == '\n'))
		return (write(client_fd, SERVER_DELE_NO_FILE,
			strlen(SERVER_DELE_NO_FILE)), true);
	if (buffer && access(buffer, F_OK) != -1) {
		asprintf(&cmd, "rm -f %s", buffer);
		res = system(cmd);
	}
	free(cmd);
	if (res != 0)
		return (write(client_fd, SERVER_DELE_NO_FILE,
			strlen(SERVER_DELE_NO_FILE)), true);
	write(client_fd, SERVER_REQUEST, strlen(SERVER_REQUEST));
	return (true);
}
