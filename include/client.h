/*
** EPITECH PROJECT, 2018
** client
** File description:
** client
*/

#ifndef CLIENT_H_
# define CLIENT_H_
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include "ftp.h"

typedef enum s_mod {
	LISTEN,
	READ
} t_mod;

typedef struct s_client {
	int socket_fd;
	int port;
	char *ip;
	t_data_chanel data;
} t_client;

typedef struct s_pointer_cmd {
	char *name;
	int size;
	t_bool (*ptr_function)(int client_fd, char *cmd, t_client *client);
} t_pointer_cmd;

t_bool my_break(t_bool state);
t_bool create_socket(t_data_chanel *data, char *proto);
t_bool connect_socket(t_client *client, char *proto);
t_bool upload_function(int client_fd, char *cmd, t_client *client);
t_bool download_function(int client_fd, char *cmd, t_client *client);
t_bool pasv_function(int client_fd, char *buffer, t_client *client);
t_bool list_function(int client_fd, char *buffer, t_client *client);
t_bool quit_function(int client_fd, char *buffer, t_client *client);
t_bool read_server(t_client *client);
t_bool listen_server(FILE *fd, t_client *client);
t_bool check_status(t_client *client);
t_bool close_last_chanel(t_client *client);
t_bool action_before_send(t_client *client, char *cmd);
t_bool client_command(t_client *client, char *cmd);
t_bool readServer(t_client *client);
t_bool connect_socket(t_client *client, char *proto);
t_bool create_prompt(t_client *client);
int my_strncmp(char *s1, char *s2, int len);
void my_handler(int s);
int check_ip(char *ip);

#define MY_EOT "\r\n"
#define SOCKADDR (struct sockaddr *)
#endif /* !CLIENT_H_ */
