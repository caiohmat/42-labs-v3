/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   curl_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chideyuk <chideyuk@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 22:14:33 by chideyuk          #+#    #+#             */
/*   Updated: 2022/08/03 01:29:24 by chideyuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/monitor.h"
#include <curl/curl.h>

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
	(void)buffer;
	(void)userp;

	return (size * nmemb);
}

int	curl_setup(t_config *config, FILE *temp_log)
{
	t_config	*temp;
	
	temp = config;
	curl_global_init(CURL_GLOBAL_ALL);
	while (temp->next)
	{
		temp->curl_handle = curl_easy_init();
		if (temp->curl_handle)
		{
			curl_easy_setopt(temp->curl_handle, CURLOPT_URL, temp->address);
			curl_easy_setopt(temp->curl_handle, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(temp->curl_handle, CURLOPT_VERBOSE, 1L);
			curl_easy_setopt(temp->curl_handle, CURLOPT_STDERR, temp_log);
			if (temp->protocol == 1)
				curl_easy_setopt(temp->curl_handle, CURLOPT_CUSTOMREQUEST, temp->DNSserver_HTTPtmethod);
		}
		else
		{
			printf("Error: Could not initiate curl for: %s\n", temp->name);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}