/*
** EPITECH PROJECT, 2018
** read
** File description:
** listen
*/

#include "client.h"

t_bool manage_code(t_client *client, char *b)
{
	size_t nb = 2;
	static t_pointer_cmd cmd[] = {
		{"227 ", strlen("227 "), pasv_function},
		{"221 ", strlen("221 "), quit_function}
	};

	for (size_t i = 0 ; i < nb ; ++i)
		if (strncmp(b, cmd[i].name, cmd[i].size) == 0)
			return (cmd[i].ptr_function(client->socket_fd,
			&b[cmd[i].size], client));
	return (true);
}

t_bool str_empty(char *buffer)
{
	if (buffer == NULL)
		return (true);
	for (size_t index = 0 ; buffer[index] != '\0' ; ++index) {
		if (buffer[index] != ' ' && buffer[index] != '\t'
		&& buffer[index] != '\n')
			return (false);
	}
	return (true);
}

t_bool stop_read(char *buffer, char delim, int nb)
{
	size_t index = 0;
	int number = 0;

	while (buffer[index] != '\0' && buffer[index] != delim) {
		if (buffer[index] >= '0' && buffer[index] <= '9')
			++number;
		else
			return (false);
		++index;
	}
	return (number == nb ? true : false);
}

t_bool listen_server(FILE *fd, t_client *client)
{
	char *buffer = NULL;
	int error = 0;
	size_t size = 0;
	socklen_t len = sizeof(error);
	t_bool res;

	getsockopt(client->socket_fd, SOL_SOCKET, SO_ERROR, &error, &len);
	if (error != 0)
		return (false);
	while (getline(&buffer, &size, fd) > 0) {
		printf("%s", buffer);
		if (stop_read(buffer, ' ', 3) == true) {
			res = manage_code(client, buffer);
			return (free(buffer), res);
		} else if (buffer != NULL)
		buffer != NULL ? free(buffer), buffer = NULL : (char *) buffer;
	}
	return (free(buffer), true);
}

t_bool read_server(t_client *client)
{
	char *buffer = NULL;
	size_t size = 0;
	t_bool res;

	if (getline(&buffer, &size, stdin) < 0)
		return (free(buffer), false);
	if (check_status(client) == false)
		return (free(buffer), false);
	else if (buffer == NULL || str_empty(buffer) == true)
		return (free(buffer), read_server(client));
	buffer = strtok(buffer, "\n");
	if (client_command(client, buffer) == true)
		return (free(buffer), read_server(client));
	buffer = realloc(buffer, 1 + strlen(buffer) + strlen(MY_EOT));
	buffer = strcat(buffer, MY_EOT);
	res = action_before_send(client, buffer);
	return (free(buffer), res);
}
