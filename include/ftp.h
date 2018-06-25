/*
** EPITECH PROJECT, 2018
** ftp
** File description:
** ftp
*/

#ifndef MY_FTP_H_
# define MY_FTP_H_
typedef enum s_bool {
	false,
	true
} t_bool;

typedef enum e_ftp_mod {
	NONE,
	ACTIF,
	PASSIF
} t_ftp_mod;

typedef struct s_data_chanel {
	t_ftp_mod mod;
	int socket_fd;
	int client_fd;
	int port;
	char *ip;
} t_data_chanel;
#endif /* !MY_FTP_H_ */
