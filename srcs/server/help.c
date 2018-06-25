/*
** EPITECH PROJECT, 2018
** help
** File description:
** help
*/

#include "server.h"

t_bool help_all_function(int client_fd, t_pointer_help *help,
			size_t nb, t_session *session)
{
	char *buffer = NULL;

	(void) session;
	buffer = strdup("\0");
	for (size_t index = 0 ; index < nb ; ++index) {
		buffer = realloc(buffer, 1 + strlen(buffer)
				+ strlen(help[index].msg));
		buffer = strcat(buffer, help[index].msg);
	}
	buffer = realloc(buffer, 1 + strlen(buffer) + strlen(SERVER_HELP));
	buffer = strcat(buffer, SERVER_HELP);
	write(client_fd, buffer, strlen(buffer));
	free(buffer);
	return (true);
}

t_pointer_help *get_help(void)
{
	static t_pointer_help hlp[] = {
	{"CWD", "CWD  <SP> <pathname> <CRLF> : Change working directory\n"},
	{"CDUP", "CDUP <CRLF> : Change working directory to parent\n"},
	{"QUIT", "QUIT <CRLF> : Disconnection\n"},
	{"DELE", "DELE <SP> <pathname> <CRLF> : Delete file on the server\n"},
	{"PWD", "PWD  <CRLF> : Print working directory\n"},
	{"PASV", "PASV <CRLF> : Enable \"passive\" mode for data transfer\n"},
	{"PORT", "PORT <SP> <host-port> <CRLF> : Enable active mode\n"},
	{"HELP", "HELP [<SP> <string>] <CRLF> : List available commands\n"},
	{"NOOP", "NOOP <CRLF> : Nothing\n(following are using transfer)\n"},
	{"RETR", "RETR <SP> <pathname> <CRLF> : Download file from server\n"},
	{"STOR", "STOR <SP> <pathname> <CRLF> : Upload file from client\n"},
	{"LIST", "LIST [<SP> <pathname>] <CRLF> : List files in directory\n"}};

	return (hlp);
}

t_bool help_function(int c, char *cmd, t_session *session)
{

	t_pointer_help *hlp = get_help();

	++cmd;
	if (cmd[0] == '\n' || cmd[0] == '\0')
		return (help_all_function(c, hlp, 12, session));
	for (size_t i = 0 ; i < 12 ; ++i)
		if (strcmp(cmd, hlp[i].name) == 0) {
		write(c, hlp[i].msg, strlen(hlp[i].msg));
		return (write(c, SERVER_HELP, strlen(SERVER_HELP)), true);
		}
	return (write(c, SERVER_HELP_NOT_FOUND, strlen(SERVER_HELP_NOT_FOUND)),
		true);
}

t_bool help_login_function(int c, char *cmd, t_session *session)
{
	size_t nb = 4;
	static t_pointer_help hlp[] = {
	{"USER", "USER <SP> <username> <CRLF> : user for authentication\n"},
	{"PASS", "PASS <SP> <password> <CRLF> : pass for authentication\n"},
	{"QUIT", "QUIT <CRLF> : Disconnection\n"},
	{"HELP", "HELP [<SP> <string>] <CRLF> : List available commands\n"}};

	++cmd;
	if (cmd == NULL || cmd[0] == '\n' || cmd[0] == '\0')
		return (help_all_function(c, hlp, nb, session));
	for (size_t i = 0 ; i < nb ; ++i)
		if (strcmp(cmd, hlp[i].name) == 0) {
		write(c, hlp[i].msg, strlen(hlp[i].msg));
		return (write(c, SERVER_HELP, strlen(SERVER_HELP)), true);
		}
	return (write(c, SERVER_HELP_NOT_FOUND, strlen(SERVER_HELP_NOT_FOUND)),
		true);
}

void help(const char *b)
{
	printf("USAGE: %s port path\n\tport is the"
	"port is the number on which the server socket listens\n\t"
	"path is the path to the home directory for the Anonymous user\n", b);
}
