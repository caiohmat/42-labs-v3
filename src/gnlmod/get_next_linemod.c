/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_linemod.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chideyuk <chideyuk@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 12:43:21 by chideyuk          #+#    #+#             */
/*   Updated: 2022/07/29 19:00:18 by chideyuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/get_next_line.h"

static char	*linefinder(char *str)
{
	char	*line;
	size_t	counter;

	if (*str == '\0')
		return (NULL);
	counter = 0;
	while (str[counter] != '\n' && str[counter])
		counter++;
	line = malloc((counter + 1) * sizeof(char));
	if (line == NULL)
		return (NULL);
	counter = 0;
	while (str[counter] != '\n' && str[counter])
	{
		line[counter] = str[counter];
		counter++;
	}
	line[counter] = '\0';
	return (line);
}

static char	*update_save(char *str)
{
	char	*newsaved;
	size_t	i;
	size_t	j;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] != '\n' && str[i])
		i++;
	if (!str[i])
	{
		free (str);
		return (NULL);
	}
	newsaved = malloc((ft_strlen(str) + i + 1) * sizeof(char));
	if (newsaved == NULL)
		return (NULL);
	i++;
	j = 0;
	while (str[i])
		newsaved[j++] = str[i++];
	free (str);
	newsaved[j] = '\0';
	return (newsaved);
}	

char	*get_next_linemod(int fd)
{
	static char	*saved;
	char		*buffer;
	int			readret;

	if (fd < 0)
		return (NULL);
	readret = 1;
	buffer = malloc(sizeof(char) * 2);
	if (buffer == NULL)
		return (NULL);
	while (hasline(saved) == 0 && readret != 0)
	{
		readret = read(fd, buffer, 1);
		if (readret == -1)
		{
			free (buffer);
			return (NULL);
		}
		buffer[readret] = '\0';
		saved = ft_strjoin(saved, buffer);
	}
	free(buffer);
	buffer = linefinder(saved);
	saved = update_save(saved);
	return (buffer);
}
