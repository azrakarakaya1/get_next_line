/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azkaraka <azkaraka@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 11:51:30 by azkaraka          #+#    #+#             */
/*   Updated: 2025/08/16 13:08:14 by azkaraka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	get_forward_bytes(char *src)
{
	int	i;

	i = 0;
	while (src[i] && src[i] != '\n')
		i++;
	return (i);
}

static char	*get_line(char *src)
{
	char	*line;
	int		i;
	int		j;

	if (!src || !src[0])
		return (NULL);
	i = get_forward_bytes(src);
	line = ft_calloc(i + (src[i] == '\n') + 1, sizeof(char));
	if (!line)
		return (NULL);
	j = 0;
	while (j < i)
	{
		line[j] = src[j];
		j++;
	}
	if (src[i] == '\n')
		line[j] = '\n';
	return (line);
}

static char	*get_src(char *src)
{
	char	*new_src;
	int		i;
	int		j;

	i = get_forward_bytes(src);
	if (!src[i])
	{
		free(src);
		return (NULL);
	}
	i++;
	new_src = ft_calloc(ft_strlen(src) - i + 1, sizeof(char));
	if (!new_src)
		return (NULL);
	j = 0;
	while (src[i])
		new_src[j++] = src[i++];
	free(src);
	return (new_src);
}

static char	*read_forward(int fd, char *src)
{
	char	*buf;
	char	*tmp;
	int		bytes_read;

	if (!src)
		src = ft_strdup("");
	buf = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	bytes_read = 1;
	if (!buf)
		return (NULL);
	while (!ft_strchr(src, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buf);
			return (NULL);
		}
		buf[bytes_read] = '\0';
		tmp = ft_strjoin(src, buf);
		free(src);
		src = tmp;
	}
	free(buf);
	return (src);
}

char	*get_next_line(int fd)
{
	static char	*buffer_src;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	buffer_src = read_forward(fd, buffer_src);
	if (!buffer_src)
		return (NULL);
	line = get_line(buffer_src);
	buffer_src = get_src(buffer_src);
	return (line);
}
