/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chideyuk <chideyuk@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 12:43:21 by chideyuk          #+#    #+#             */
/*   Updated: 2022/07/29 19:00:22 by chideyuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t	counter;

	counter = 0;
	if (!s)
		return (0);
	while (s[counter] != '\0')
		counter++;
	return (counter);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*s3;
	size_t	len1;
	size_t	len2;
	size_t	counter;
	size_t	counter2;

	len1 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	s3 = malloc(len1 + len2 + 1);
	if (s3 == NULL)
		return (NULL);
	counter = 0;
	counter2 = 0;
	if (s1 != 0)
		while (counter2 < len1)
			s3[counter++] = s1[counter2++];
	counter2 = 0;
	while (counter2 < len2)
		s3[counter++] = s2[counter2++];
	free(s1);
	s3[counter] = '\0';
	return (s3);
}

int	hasline(char *ptr)
{
	size_t	counter;

	if (!ptr)
		return (0);
	counter = 0;
	while (ptr[counter])
	{
		if (ptr[counter] == '\n')
			return (1);
		counter++;
	}
	return (0);
}
