/*
** EPITECH PROJECT, 2018
** random
** File description:
** random
*/

#include <stdlib.h>
#include <time.h>

int my_random(int min, int max)
{
	static int i = 0;

	if (i == 0) {
		srand(time(NULL));
		i = 1;
	}
	return (rand() % (max - min + 1) + min);
}
