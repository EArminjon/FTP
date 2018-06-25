/*
** EPITECH PROJECT, 2018
** salut
** File description:
** balek
*/

#include "server.h"

void my_handler(int s)
{
	(void) s;
	my_break(true);
}

int my_signal(void)
{
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);
	return 0;
}

t_bool manage_cmds(int fd, char *b, t_session *session)
{
	static t_pointer_cmd c[] = {
		{"CWD", strlen("CWD"), cwd_function},
		{"PWD\0", strlen("PWD "), pwd_function},
		{"PASV\0", strlen("PASV "), pasv_function},
		{"PORT", strlen("PORT"), port_function},
		{"DELE", strlen("DELE"), dele_function},
		{"CDUP\0", strlen("CDUP "), cdup_function},
		{"NOOP\0", strlen("NOOP "), noop_function},
		{"TYPE", strlen("TYPE"), noop_function},
		{"LIST", strlen("LIST"), list_function},
		{"RETR", strlen("RETR"), download_function},
		{"STOR", strlen("STOR"), upload_function},
		{"HELP", strlen("HELP"), help_function},
		{"QUIT\0", strlen("QUIT "), quit_function}};

	for (size_t i = 0 ; i < 13 ; ++i)
		if (my_strncmp(b, c[i].name, c[i].size) == 0)
			return (c[i].ptr_function(fd, &b[c[i].size], session));
	return (write(fd, SERVER_CMD_NOT_FOUND,
		strlen(SERVER_CMD_NOT_FOUND)), true);
}

t_bool server_loop(t_ftp *ftp)
{
	int error = 0;
	t_bool res = true;

	if (chdir(ftp->home_directory) == -1) {
		fprintf(stderr, "Invalid path argument, no such directory.\n");
		return (false);
	}
	while (!error && my_break(false) == false && res == true)
		res = create_client(ftp);
	return (res);
}

int main(int ac, char **av)
{
	t_ftp ftp = {NULL, NULL, -1, -1, {}};
	t_bool result;
	char *path = NULL;

	if (ac != 3 || (ac == 2 && av[1] != NULL && strcmp(av[1], "--help")))
		return (help(av[0]), 84);
	my_signal();
	ftp.port = atoi(av[1]);
	if (ftp.port <= 0)
		return (fprintf(stderr, "Port must be positif\n"), 84);
	if (create_socket(&ftp, "TCP") == false)
		return (84);
	path = getcwd(NULL, 0);
	asprintf(&ftp.home_directory, "%s", av[2]);
	asprintf(&ftp.root_directory, "%s", path);
	free(path);
	result = server_loop(&ftp);
	if (ftp_destroy(&ftp) == false)
		return (false);
	return (result == false ? 84 : 0);
}
