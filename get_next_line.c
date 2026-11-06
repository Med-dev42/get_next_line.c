/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlazar <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 09:35:49 by mlazar            #+#    #+#             */
/*   Updated: 2026/11/03 11:28:59 by mlazar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_to_stash(int fd, char *stash)
{
	char	*buff;
	ssize_t	n;

	buff = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buff)
		return (NULL);
	n = 1;
	while (!ft_strchr(stash, '\n') && n > 0)
	{
		n = read(fd, buff, BUFFER_SIZE);
		if (n < 0)
			break ;
		buff[n] = '\0';
		stash = ft_strjoin(stash, buff);
		if (!stash)
			break ;
	}
	free(buff);
	if (n < 0)
	{
		free(stash);
		return (NULL);
	}
	return (stash);
}

char	*clean_stash(char *stash)
{
	char	*s;
	char	*res;
	size_t	i;

	s = ft_strchr(stash, '\n');
	if (!s || !s[1])
	{
		free(stash);
		return (NULL);
	}
	res = malloc((ft_strlen(s + 1) + 1) * sizeof(char));
	if (!res)
	{
		free(stash);
		return (NULL);
	}
	i = 0;
	while (s[1 + i])
	{
		res[i] = s[1 + i];
		i++;
	}
	res[i] = '\0';
	free(stash);
	return (res);
}

char	*search_line(char *stash)
{
	char	*res;
	size_t	i;
	size_t	j;

	if (!stash || !stash[0])
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	res = malloc((i + 1) * sizeof(char));
	if (!res)
		return (NULL);
	j = 0;
	while (j < i)
	{
		res[j] = stash[j];
		j++;
	}
	res[j] = '\0';
	return (res);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_to_stash(fd, stash);
	if (!stash)
		return (NULL);
	line = search_line(stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = clean_stash(stash);
	return (line);
}
