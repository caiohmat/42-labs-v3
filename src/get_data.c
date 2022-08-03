/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chideyuk <chideyuk@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 21:30:57 by chideyuk          #+#    #+#             */
/*   Updated: 2022/08/03 01:30:49 by chideyuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/monitor.h"
#include "ctype.h"


void	config_init(t_config *config)
{
	config->protocol = 0;
	config->name = NULL;
	config->address = NULL;
	config->interval = 0;
	config->DNSserver_HTTPtmethod = NULL;
	config->HTTPcode = 0;
	config->next = NULL;
	config->last_check = 0;
	config->curl_handle = NULL;
}

static int validnumber(char *s)
{
	for (unsigned long i = 0; i < strlen(s); i++)
	{
		if (!isdigit(s[i]))
			return (0);
	}
	return (1);
}

static int	check_data(char **split)
{
	if (ft_ptrlen(split) < 4 || ft_ptrlen(split) > 6)
	{
		printf("Error: Incorrect number of arguments on config file\n");
		return(-1);
	}
	if (strcmp(split[1], "HTTP") && strcmp(split[1], "PING") && strcmp(split[1], "DNS"))
	{
		printf("Error: Invalid protocol\n");
		return(-1);
	}
	if (!strcmp(split[1], "HTTP"))
	{
		if (ft_ptrlen(split) != 6)
		{
			printf("Error: Incorrect number of arguments on config file\n");
			return(-1);
		}
		else if (!validnumber(split[4]) || !validnumber(split[5]))
		{
			printf("Error: %s and/or %s is/are not valid numbers\n", split[4], split[5]);
			return(-1);
		}
	}
	else
	{
		if (!validnumber(split[3]))
		{
			printf("Error: %s is not valid number\n", split[3]);
			return(-1);
		}	
		if ((!strcmp(split[1], "PING") && ft_ptrlen(split) != 4) 
			|| (!strcmp(split[1], "DNS") && ft_ptrlen(split) != 5))
		{
			printf("Error: Incorrect number of arguments on config file\n");
			return(-1);
		}
	}
	return (1);
}

static void	fill_data(t_config	*config, char **split)
{
	config->name = strdup(split[0]);
	config->address = strdup(split[2]);
	if (!strcmp(split[1], "HTTP"))
	{
		config->protocol = 1;
		config->interval = atoi(split[5]);
		config->HTTPcode = atoi(split[4]);
		config->DNSserver_HTTPtmethod = strdup(split[3]);
	}
	else
	{
		config->interval = atoi(split[3]);
		if (!strcmp(split[1], "DNS"))
		{
			config->protocol = 3;
			config->DNSserver_HTTPtmethod = strdup(split[4]);
		}
		else
			config->protocol = 2;
	}
}

int	get_data(t_config *config)
{
	int			fd;
	int			data_result;
	char		*buffer;
	char		**split;
	t_config	*temp;
	
	fd = open("monitoring.db", O_RDONLY);
	if (fd == -1)
	{
		printf("Error: Could not open file\n");
		return (1);
	}
	buffer = get_next_linemod(fd);
	if (!strcmp(buffer, "# monitoring.db"))
	{
		free(buffer);
		buffer = get_next_linemod(fd);
	}
	temp = config;
	while (buffer)
	{
		if (buffer[0] != '\0')
		{
			split = ft_split(buffer, '\t');
			data_result = check_data(split);
			if (data_result == -1)
			{
				printf("Error: Could not get data\n");
				close(fd);
				return (1);
			}
			fill_data(temp, split);
			free(buffer);
			ft_freeptr(split);
			temp->next = malloc(sizeof(*config));
			config_init(temp->next);
			temp = temp->next;
		}
		else
			free(buffer);
		buffer = get_next_linemod(fd);
	}
	close(fd);
	return (0);
}