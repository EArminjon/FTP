/*
** EPITECH PROJECT, 2018
** login
** File description:
** login
*/

#include "server.h"

int my_strcmp(char *s1, char *s2)
{
	if ((s1 == NULL && (s2[0] == '\0' || s2[0] == '\r' || s2[0] == '\n'))
	|| (s2 == NULL && (s1[0] == '\0' || s1[0] == '\r' || s2[0] == '\n')))
		return (0);
	return (strcmp(s1, s2));
}

t_bool check_credential(t_session *session)
{
	FILE *fp = NULL;
	size_t size = 0;
	char *buffer = NULL, *path = NULL;

	asprintf(&path, "%s/%s", session->root_directory, ACCOUNT_FILE);
	fp = fopen (path, "r");
	free(path);
	if (fp == NULL)
		return (fprintf(stderr, "Account not found.\n"), false);
	while (getline(&buffer, &size, fp) != -1) {
		if (strchr(buffer, ':') != NULL &&
		((my_strcmp(strtok(buffer, ":"), session->login) == 0)
		&& (my_strcmp(strtok(NULL, "\n"), session->pass) == 0)))
			return (fclose(fp), free(buffer), true);
		else if (buffer != NULL)
			free(buffer);
		buffer = NULL;
	}
	return (buffer != NULL ? free(buffer), false : false);
}

t_bool authentification(t_session *session)
{
	char *last = NULL, *path = NULL;

	if (check_credential(session) == false)
		return (false);
	else if (strcmp(session->login, DEFAULT_ACCOUNT) == 0)
		return (true);
	if (session->home_directory != NULL)
		free(session->home_directory);
	last = getcwd(NULL, 0);
	if (session->root_directory != NULL)
		chdir(session->root_directory);
	path = getcwd(NULL, 0);
	asprintf(&session->home_directory, "%s/%s/%s", path,
		ACCOUNT_PATH, session->login);
	free(path);
	if (chdir(session->home_directory) != -1)
		return (free(last), true);
	chdir(last);
	free(last);
	return (fprintf(stderr, "Account directory not found.\n"), false);
}
