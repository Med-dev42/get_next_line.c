/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlazar <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 12:46:52 by mlazar            #+#    #+#             */
/*   Updated: 2026/11/03 14:54:26 by mlazar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

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
	static char	*stash[FD_MAX];
	char		*line;

	if (fd < 0 || fd >= FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	stash[fd] = read_to_stash(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = search_line(stash[fd]);
	if (!line)
	{
		free(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	stash[fd] = clean_stash(stash[fd]);
	return (line);
}
