/*
** EPITECH PROJECT, 2018
** global
** File description:
** global
*/

#include "client.h"

t_bool my_break(t_bool state)
{
	static t_bool stop = false;

	if (state == true)
		stop = state;
	return (stop);
}
