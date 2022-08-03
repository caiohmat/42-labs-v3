/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_loop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chideyuk <chideyuk@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 20:21:46 by chideyuk          #+#    #+#             */
/*   Updated: 2022/08/03 02:07:35 by chideyuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/monitor.h"
#include <unistd.h>
#include <time.h>
#include <ctype.h>


void	generate_http_log(char *buffer, int log_file)
{
	if (buffer[0] != '*' && !isspace(buffer[0]))
	{
		if (buffer[0] == '<' || buffer[0] == '>')
			buffer += 2;
		dprintf(log_file, "%s", buffer);
		if (strstr(buffer, "HTTP") || strstr(buffer, "Host:") || strstr(buffer, "Accept:"))
			printf("%s", buffer);
	}
}

void	run_DNS_test(t_config *config)
{
	(void)config;
}

void	run_PING_test(t_config *config, FILE *temp_log, int log_file)
{
	time_t	t;
	struct	tm tm;
	char	buffer[100];
	int		response;

	printf("\n%s ", config->name);
	dprintf(log_file, "\nLOG: %s ", config->name);
	config->last_check = ft_time();
	t = time(NULL);
  	tm = *localtime(&t);
	dprintf(log_file, "[%d-%02d-%02d %02d:%02d:%02d]\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  	printf("[%d-%02d-%02d %02d:%02d:%02d]\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	temp_log = freopen(NULL, "w", temp_log);
	response = curl_easy_perform(config->curl_handle);
	temp_log = freopen(NULL, "r", temp_log);
	if (response == CURLE_OK)
	{
		while (fgets(buffer, 100, temp_log))
		{
			if (strstr(buffer, "Connected"))
			{
				printf("SUCCESS: %s", buffer + 2);
				dprintf(log_file, "SUCCESS: %s\n", buffer + 2);
				break ;
			}
		}
	}
	else
	{
		fgets(buffer, 100, temp_log);
		printf("FAILED: %s", buffer + 2);
		dprintf(log_file, "FAILED: %s\n", buffer + 2);
	}
}

void	run_HTTP_test(t_config *config, FILE *temp_log, int log_file)
{
	time_t	t;
	struct	tm tm;
	char	buffer[100];
	int		response;
	long 	response_code;

	printf("\n%s ", config->name);
	dprintf(log_file, "LOG: %s ", config->name);
	config->last_check = ft_time();
	t = time(NULL);
  	tm = *localtime(&t);
	dprintf(log_file, "[%d-%02d-%02d %02d:%02d:%02d]\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  	printf("[%d-%02d-%02d %02d:%02d:%02d]\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	temp_log = freopen(NULL, "w", temp_log);
	response = curl_easy_perform(config->curl_handle);
	temp_log = freopen(NULL, "r", temp_log);
	if (response == CURLE_OK)
	{
		while (fgets(buffer, 100, temp_log))
			generate_http_log(buffer, log_file);
		curl_easy_getinfo(config->curl_handle, CURLINFO_RESPONSE_CODE, &response_code);
		if (response_code == config->HTTPcode)
		{
			printf("SUCCESS: expected %ld, got %ld\n", config->HTTPcode, response_code);
			dprintf(log_file, "SUCCESS: expected %ld, got %ld\n\n", config->HTTPcode, response_code);
		}
		else
		{
			printf("FAILED: expected %ld, got %ld\n", config->HTTPcode, response_code);
			dprintf(log_file, "FAILED: expected %ld, got %ld\n\n", config->HTTPcode, response_code);
		}
	}
	else
	{
		fgets(buffer, 100, temp_log);
		printf("Error: %s", buffer + 2);
		dprintf(log_file, "Error: %s", buffer + 2);
	}
}

void	monitoring_loop(t_config *config, FILE *temp_log, int log_file)
{
	t_config			*temp;
	
	g_exit = 1;
	while (g_exit)
	{
		temp = config;
		while (temp->next)
		{
			if ((ft_time() - temp->last_check) > temp->interval || !temp->last_check)
			{
				printf("\n--------------------------------------\n");
				dprintf(log_file, "\n--------------------------------------\n");
				if (temp->protocol == 1)
					run_HTTP_test(temp, temp_log, log_file);
				else if (temp->protocol == 2)
					run_PING_test(temp, temp_log, log_file);
				else
					run_DNS_test(temp);
				printf("\n--------------------------------------\n");
				dprintf(log_file, "\n--------------------------------------\n");
			}
			temp = temp->next;
		}
		usleep(100);
	}
}
