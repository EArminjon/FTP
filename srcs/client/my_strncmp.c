/*
** EPITECH PROJECT, 2018
** dd
** File description:
** dd
*/

int my_strncmp(char *s1, char *s2, int len)
{
	int i = 0;

	while (s1[i] == s2[i] && i < len)
		++i;
	if (s1[i] == '\0' && (s2[i] == '\0' || s2[i] == ' '
			|| s2[i] == '\r' || s2[i] == '\n'))
		return (0);
	else if (s2[i] == '\0' && (s1[i] == '\0' || s1[i] == ' '
			|| s1[i] == '\r' || s1[i] == '\n'))
		return (0);
	return (s1[i] - s2[i]);
}
