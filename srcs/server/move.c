/*
** EPITECH PROJECT, 2018
** cwd, pwd, cdup
** File description:
** pwd, cwp, cdup
*/

#include "server.h"

t_bool cwd_function(int client_fd, char *buffer, t_session *session)
{
	char *last = NULL, *path = NULL;

	++buffer;
	last = getcwd(NULL, 0);
	if (chdir(buffer) == 0) {
		path = getcwd(NULL, 0);
		if (strncmp(path, session->home_directory,
			strlen(session->home_directory)) != 0) {
			chdir(last);
			return (free(last), free(path), write(client_fd,
			SERVER_CHANGE_DIR, strlen(SERVER_CHANGE_DIR)), true);
		}
		return (free(last), free(path), write(client_fd,
			SERVER_CHANGE_DIR, strlen(SERVER_CHANGE_DIR)), true);
	}
	return (free(last), write(client_fd, SERVER_CHANGE_DIR_FAIL,
		strlen(SERVER_CHANGE_DIR_FAIL)), true);
}

t_bool pwd_function(int client_fd, char *buffer, t_session *session)
{
	char *path = getcwd(NULL, 0);

	(void) session;
	(void) buffer;
	write(client_fd, "257 \"", strlen("257 \""));
	write(client_fd, path, strlen(path));
	free(path);
	return (write(client_fd, "\"\n", strlen("\"\n")), true);
}

t_bool cdup_function(int client_fd, char *buffer, t_session *session)
{
	int res;

	(void) buffer;
	res = strcmp(session->home_directory, getcwd(NULL, 0));
	if (res != 0 && (chdir("..") == 0)) {
		return (write(client_fd, SERVER_CHANGE_DIR,
			strlen(SERVER_CHANGE_DIR)), true);
	} else if (res == 0)
		return (write(client_fd, SERVER_CHANGE_DIR,
			strlen(SERVER_CHANGE_DIR)), true);
	return (write(client_fd, SERVER_CHANGE_DIR_FAIL,
		strlen(SERVER_CHANGE_DIR_FAIL)), true);
}
