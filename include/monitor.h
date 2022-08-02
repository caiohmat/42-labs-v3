/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chideyuk <chideyuk@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 20:53:37 by chideyuk          #+#    #+#             */
/*   Updated: 2022/08/02 20:45:39 by chideyuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MONITOR_H
# define MONITOR_H

#include "get_next_line.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <curl/curl.h>

extern int	g_exit;

typedef struct	s_config
{
	int					protocol;
	char				*name;
	char				*address;
	unsigned long long	interval;
	char				*DNSserver_HTTPtmethod;
	long				HTTPcode;
	unsigned long long	last_check;
	CURL				*curl_handle;
	struct s_config		*next;
}						t_config;


int					get_data(t_config *config);
void				config_init(t_config *config);
void				monitoring_loop(t_config *config, FILE *temp_log, int log_file);
int					curl_setup(t_config *config, FILE *temp_log);
char				**ft_split(char const *s, char c);
int					ft_ptrlen(char **s);
void				ft_freeptr(char	**ptr);
unsigned long long	ft_time(void);

#endif