/*
** EPITECH PROJECT, 2018
** server client
** File description:
** client
*/

#include "server.h"

t_bool manage_buffer(int client_fd, char *buffer, t_session *session)
{
	t_bool cmd;

	buffer = strtok(buffer, "\r\n");
	printf("BUFFER: %s\n", buffer);
	if (buffer == NULL)
		return (true);
	else if (session->authent == false)
		cmd = login(client_fd, buffer, session);
	else
		cmd = manage_cmds(client_fd, buffer, session);
	return (cmd);
}

t_bool process_client(char *root, char *home, int client_fd, char *ip)
{
	t_bool cmd = true;
	FILE *file = NULL;
	size_t size = 0;
	char *buffer = NULL;
	t_session session;

	file = fdopen(dup(client_fd), "rwx");
	if (file == NULL)
		return (fprintf(stderr, "fdopen failed\n"), false);
	create_session(&session, root, home, ip);
	while (cmd == true && (getline(&buffer, &size, file) != -1)) {
		cmd = manage_buffer(client_fd, buffer, &session);
		if (buffer != NULL)
			free(buffer);
		buffer = NULL;
	}
	if (buffer != NULL)
		free(buffer);
	fclose(file);
	return (delete_session(&session));
}

t_bool destroy_client(t_ftp *ftp, int fd)
{
	if (fcntl(fd, F_GETFD) != -1)
		close(fd);
	free(ftp->home_directory);
	free(ftp->root_directory);
	return (true);
}

t_bool create_client(t_ftp *ftp)
{
	struct sockaddr_in s_client;
	socklen_t s_size = sizeof(s_client);
	int fd, pid;

	fd = accept(ftp->socket_fd, (struct sockaddr *)&s_client, &s_size);
	if (fd == -1)
		return (fprintf(stderr, "Client: accept failed.\n"), false);
	pid = fork();
	if (pid == -1)
		return (fprintf(stderr, "Fork failed\n"), false);
	else if (pid == 0) {
		write(fd, SERVER_CODE_READY, strlen(SERVER_CODE_READY));
		process_client(ftp->root_directory, ftp->home_directory,
			fd, inet_ntoa(s_client.sin_addr));
		exit(destroy_client(ftp, fd) == true ? 0 : 84);
	}
	return ((fcntl(fd, F_GETFD) != -1 && close(fd) == -1) ? false : true);
}
