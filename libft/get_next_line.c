/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristi <alcristi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 11:21:18 by alcristi          #+#    #+#             */
/*   Updated: 2021/12/14 11:21:35 by alcristi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*conc;
	size_t	i;
	size_t	size_s1;
	size_t	size_s2;

	i = -1;
	size_s1 = 0;
	size_s2 = 0;
	while (s1[size_s1])
		size_s1++;
	while (s2[size_s2])
		size_s2++;
	conc = malloc((size_s2 + size_s1 + 1) * sizeof(char));
	while (++i < size_s1)
		conc[i] = s1[i];
	i = -1;
	while (++i < size_s2)
		conc[i + size_s1] = s2[i];
	conc[size_s2 + size_s1] = '\0';
	free(s1);
	return (conc);
}

static void	ft_strcpy(char *dest, const char *src, size_t size_src)
{
	size_t	i;
	size_t	size_dest;

	if (!dest && !src)
		return ;
	i = 0;
	size_dest = 0;
	while (dest[size_dest])
		size_dest++;
	ft_bzero(dest, size_dest);
	while (i < size_src)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

static void	get_line(char **buffer, char **line, char *data)
{
	int		i;
	int		size_data;
	char	*tmp_data;

	i = 0;
	size_data = 0;
	tmp_data = ft_strdup("");
	while (buffer[0][i] != '\n' && buffer[0][i] != '\0')
		i++;
	if (buffer[0][i] == '\n')
	{
		line[0] = ft_substr(buffer[0], 0, i + 1);
		free(tmp_data);
		tmp_data = ft_strdup(&buffer[0][i + 1]);
	}
	else
		line[0] = ft_strdup(buffer[0]);
	while (tmp_data[size_data])
		size_data++;
	ft_strcpy(data, tmp_data, size_data);
	free(tmp_data);
}

static char	*ft_read(char *data, char **buffer, int fd)
{
	char	*line;
	int		r;
	int		total_r;

	r = 1;
	total_r = 0;
	while (buffer[0][total_r])
		total_r++;
	while (!ft_strchr(buffer[0], '\n') && r)
	{
		r = read(fd, data, BUFFER_SIZE);
		total_r += r;
		if (r <= 0)
			break ;
		buffer[0] = ft_strjoin_gnl(buffer[0], data);
		buffer[0][total_r] = '\0';
	}
	if (r < 0)
		return (NULL);
	else if (r == 0 && total_r == 0)
		return (NULL);
	get_line(buffer, &line, data);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	data[BUFFER_SIZE + 1];
	char		*line;
	char		*buffer;

	if (fd < 0 || BUFFER_SIZE < 0)
		return (0);
	if (!data[0])
		buffer = ft_strdup("");
	else
		buffer = ft_strdup(data);
	line = ft_read(data, &buffer, fd);
	free(buffer);
	return (line);
}
