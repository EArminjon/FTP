/*
** EPITECH PROJECT, 2018
** ls;cwd;cd...
** File description:
** client cmd
*/

#include "client.h"

t_bool ls_function(int client_fd, char *cmd, t_client *client)
{
	char *buffer = NULL;
	char *tmp;

	(void) client_fd;
	(void) client;
	++cmd;
	tmp = strtok(cmd, ";");
	if (tmp != NULL)
		cmd = tmp;
	asprintf(&buffer, "ls --color %s", cmd);
	system(buffer);
	free(buffer);
	return (true);
}

t_bool pwd_function(int client_fd, char *cmd, t_client *client)
{
	(void) client_fd;
	(void) cmd;
	(void) client;
	system("pwd");
	return (true);
}

t_bool cd_function(int client_fd, char *cmd, t_client *client)
{
	(void) client_fd;
	(void) client;
	++cmd;
	if (chdir(strtok(cmd, ";")) == -1)
		fprintf(stderr, "No such file or directory.\n");
	return (true);
}

t_bool client_command(t_client *client, char *b)
{
	size_t nb = 3;
	static t_pointer_cmd cmd[] =
		{
			{"ls", strlen("ls"), ls_function},
			{"cd", strlen("cd"), cd_function},
			{"pwd\0", strlen("pwd "), pwd_function},
		};

	for (size_t i = 0 ; i < nb ; ++i)
		if (strncmp(b, cmd[i].name, cmd[i].size) == 0)
			return (cmd[i].ptr_function(client->socket_fd,
						&b[cmd[i].size], client));
	return (false);
}
