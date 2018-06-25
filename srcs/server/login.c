/*
** EPITECH PROJECT, 2018
** login
** File description:
** login
*/

#include "server.h"

t_bool user_function(int client_fd, char *cmd, t_session *session)
{
	if (cmd[0] == '\0')
		return (write(client_fd, SERVER_PERMISSION_DENIED,
			strlen(SERVER_PERMISSION_DENIED)), true);
	++cmd;
	asprintf(&session->login, "%s", cmd);
	write(client_fd, SERVER_LOGIN, strlen(SERVER_LOGIN));
	return (true);
}

t_bool pass_function(int client_fd, char *cmd, t_session *session)
{
	++cmd;
	if (strcmp(session->login, "\0") != 0) {
		asprintf(&session->pass, "%s", cmd);
		if (authentification(session) == false)
			return (write(client_fd, SERVER_PASS_BAD,
				strlen(SERVER_PASS_BAD)), true);
		session->authent = true;
		return (write(client_fd, SERVER_PASS,
			strlen(SERVER_PASS)), true);
	}
	return (write(client_fd, SERVER_USER_BEFORE,
		strlen(SERVER_USER_BEFORE)), true);
}

t_bool login(int fd, char *b, t_session *s)
{
	size_t nb = 4;
	static t_pointer_cmd cmd[] = {
			{"USER", strlen("USER"), user_function},
			{"PASS", strlen("PASS"), pass_function},
			{"HELP", strlen("HELP"), help_login_function},
			{"QUIT\0", strlen("QUIT "), quit_function}};

	for (size_t i = 0 ; i < nb ; ++i)
		if (my_strncmp(b, cmd[i].name, cmd[i].size) == 0)
			return (cmd[i].ptr_function(fd, &b[cmd[i].size], s));
	write(fd, SERVER_LOGIN_FIRST, strlen(SERVER_LOGIN_FIRST));
	return (true);
}
