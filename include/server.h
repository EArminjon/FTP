/*
** EPITECH PROJECT, 2018
** ftp
** File description:
** ftp
*/

#ifndef MY_SERVER_H_
# define MY_SERVER_H_
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "color.h"
#include "ftp.h"

typedef struct s_ftp {
	char *root_directory;
	char *home_directory;
	int socket_fd;
	int port;
	struct sockaddr_in s_in;
} t_ftp;

typedef struct s_session {
	char *login;
	char *pass;
	t_bool authent;
	char *home_directory;
	char *root_directory;
	t_data_chanel data;
	char *client_ip;
} t_session;

typedef struct s_pointer_cmd {
	char *name;
	int size;
	t_bool (*ptr_function)(int client_fd, char *cmd, t_session *session);
} t_pointer_cmd;

typedef struct s_pointer_help {
	char *name;
	char *msg;
} t_pointer_help;

t_bool my_break(t_bool state);
t_bool ftp_destroy(t_ftp *ftp);
void help(const char *str);
int my_random(int min, int max);
t_bool wait_connection(t_session *session);
t_bool create_socket(t_ftp *ftp, char *proto_name);
t_bool connect_socket(t_session *session, char *proto_name);
t_bool quit_function(int client_fd, char *cmd, t_session *session);
t_bool list_function(int client_fd, char *cmd, t_session *session);
t_bool noop_function(int client_fd, char *cmd, t_session *session);
t_bool dele_function(int client_fd, char *cmd, t_session *session);
t_bool download_function(int client_fd, char *cmd, t_session *session);
t_bool upload_function(int client_fd, char *cmd, t_session *session);
t_bool pasv_function(int client_fd, char *cmd, t_session *session);
t_bool port_function(int client_fd, char *cmd, t_session *session);
t_bool help_function(int client_fd, char *cmd, t_session *session);
t_bool cwd_function(int client_fd, char *cmd, t_session *session);
t_bool cdup_function(int client_fd, char *cmd, t_session *session);
t_bool pwd_function(int client_fd, char *cmd, t_session *session);
t_bool help_login_function(int client_fd, char *cmd, t_session *session);
t_bool authentification(t_session *session);
t_bool create_client(t_ftp *ftp);
t_bool login(int client_fd, char *buffer, t_session *session);
void create_session(t_session *session, char *root, char *home, char *ip);
t_bool delete_session(t_session *session);
t_bool manage_cmds(int client_fd, char *buffer, t_session *session);
int my_strncmp(char *s1, char *s2, int len);

#define SERVER_OPEN "150 " GREEN "File status okay; about to open data \
connection." RESET "\n"
#define SERVER_CLOSE "226 " YELLOW "Closing data connection." RESET "\n"
#define SERVER_DATA_TRANSFERT_FAIL "226 " YELLOW "File alreay exist" RESET ".\n"
#define SERVER_DATA_TRANSFERT_EMPTY "226 " RED "No file given as \
argument." RESET "\n"
#define SERVER_CMD_NOT_FOUND "500 " RED "Command not found." RESET "\n"
#define SERVER_HELP_NOT_FOUND "214 " RED "Help not found." RESET "\n"
#define SERVER_HELP "214 " GREEN "Help ok." RESET "\n"
#define SERVER_NO_FILE "226 " RED "No file given as argument." RESET "\n"
#define SERVER_DELE_NO_FILE "550 " RED "No file given as argument." RESET "\n"
#define SERVER_CODE_READY "220 " YELLOW "Service ready for new user." RESET "\n"
#define SERVER_LOGIN "331 " CYAN "User name okay, need password." RESET "\n"
#define SERVER_LOGIN_BAD "421 " RED "Bad login." RESET "\n"
#define SERVER_LOGIN_UNKNOWN "332 " RED "Need account for login" RESET ".\n"
#define SERVER_PASS "230 " CYAN "User logged in, proceed." RESET "\n"
#define SERVER_PASS_BAD "530 " RED "Wrong password." RESET "\n"
#define SERVER_ERROR "xxx Error (RFC compliant)\n"
#define SERVER_QUIT "221 " YELLOW "Service closing control \
connection." RESET "\n"
#define SERVER_LOGOUT "221 " YELLOW "Logged out. " RESET "\n"
#define SERVER_OK "200 " GREEN "Command okay." RESET "\n"
#define SERVER_REQUEST "250 " GREEN "Requested file action okay, \
completed" RESET ".\n"
#define SERVER_CHANGE_DIR "250 " GREEN "Directory successfully \
changed." RESET "\n"
#define SERVER_PERMISSION_DENIED "530 " RED "Permission denied." RESET "\n"
#define SERVER_LOGIN_FIRST "530 " MAGENTA "Please login with USER and \
PASS." RESET "\n"
#define SERVER_CHANGE_DIR_FAIL "550 " RED "Failed to change \
directory." RESET "\n"
#define SERVER_USER_BEFORE "503 " MAGENTA "Login with \
USER first." RESET "\n"
#define SERVER_PASV "227 Entering Passive Mode"
#define SERVER_PORT "425 " MAGENTA "Use PORT or PASV first." RESET "\n"
#define SERVER_FILE_NOT_FOUND "550 " RED "No such file or directory." RESET "\n"
#define SERVER_MISSING_ARGUMENT "226 " RED "This action requiert one \
argument." RESET "\n"
#define MY_EOL "\r\n"
#define ACCOUNT_PATH "ftp"
#define ACCOUNT_FILE "ftp/account.txt"
#define DEFAULT_ACCOUNT "Anonymous"
#endif /* !MY_SERVER_H_ */
