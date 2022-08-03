/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chideyuk <chideyuk@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 20:48:19 by chideyuk          #+#    #+#             */
/*   Updated: 2022/08/03 02:03:30 by chideyuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/monitor.h"
#include <signal.h>

int	g_exit;

void ft_exit(t_config *config, FILE *temp_log, int log_file)
{
	t_config	*temp;
	t_config	*temp2;
	
	temp = config;
	while(temp)
	{
		if (temp->name)
			free(temp->name);
		if (temp->address)
			free(temp->address);
		if (temp->DNSserver_HTTPtmethod)
			free(temp->DNSserver_HTTPtmethod);
		if (temp->curl_handle)
		curl_easy_cleanup(temp->curl_handle);
		temp2 = temp;
		temp = temp->next;
		free(temp2);
	}
	curl_global_cleanup();
	fclose(temp_log);
	close(log_file);
	unlink("temp_log.txt");
	exit (0);
}

void	ft_stop(int signum)
{
	g_exit = 0;
	(void)signum;
}

void	simplify_log(void)
{
	FILE		*log_file;
	char		buffer[100];

	log_file = fopen("monitoring.log", "r");
	if (log_file)
	{
		while (fgets(buffer, 100, log_file))
		{
			if (strstr(buffer, "LOG"))
			{
				printf("\n--------------------------------------\n\n");
				printf("%s", buffer);
			}
			else if (strstr(buffer, "HTTP") || strstr(buffer, "Host:") || strstr(buffer, "Accept:"))
				printf("%s", buffer);
			else if (strstr(buffer, "SUCCESS") || strstr(buffer, "FAILED") || strstr(buffer, "Error"))
			{
				printf("%s", buffer);
				printf("\n--------------------------------------\n");
			}
		}
	}
	fclose(log_file);
}

int	main(int argc, char **argv)
{
	t_config	*config;
	int			log_file;
	FILE		*temp_log;
	
	if (argc == 2 && strstr(argv[1], "--simplify"))
		simplify_log();
	else if (argc == 1)
	{
		config = malloc(sizeof(*config));
		if (access("monitoring.log", F_OK) == 0)
			log_file = open("monitoring.log", O_APPEND | O_WRONLY | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		else
			log_file = open("monitoring.log", O_CREAT | O_WRONLY | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		temp_log = fopen("temp_log.txt", "w");
		config_init(config);
		if (get_data(config) || curl_setup(config, temp_log))
			ft_exit(config, temp_log, log_file);
		signal(SIGINT, ft_stop);
		monitoring_loop(config, temp_log, log_file);
		ft_exit(config, temp_log, log_file);
	}
	else
		printf("Error: Invalid arguments\n");
	return (0);
}
