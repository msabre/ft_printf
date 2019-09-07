/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 17:56:09 by msabre            #+#    #+#             */
/*   Updated: 2019/04/17 18:00:11 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void		ft_strfree(char **ptr, int len)
{
	if (ptr)
	{
		while (len >= 0)
		{
			free(ptr[len]);
			len--;
		}
	}
	else
		return ;
}

static char		**ft_magic(char **ptr, char const *s, char c, int len)
{
	int			i;
	int			j;
	char		*a;

	i = 0;
	j = 0;
	while (len-- > 0)
	{
		while (*s == c)
			s++;
		a = (char*)s;
		if (*a == '\0')
			break ;
		while (*a != c)
			a++;
		if (!(ptr[i] = (char*)malloc(sizeof(char) * ((a - s) + 1))))
			return (NULL);
		while (*s != c && *s != '\0')
			ptr[i][j++] = *(s++);
		ptr[i][j++] = '\0';
		i++;
		j = 0;
	}
	ptr[i] = 0;
	return (ptr);
}

static int		ft_calcul_rows(char const *s, char c)
{
	int			len;
	char		*str;

	str = (char *)s;
	len = 0;
	while (str)
	{
		while (*str == c)
		{
			if (*str == '\0')
				return (len);
			str++;
		}
		len++;
		while (*str != c)
		{
			if (*str == '\0')
				return (len);
			str++;
		}
	}
	return (len);
}

char			**ft_strsplit(char const *s, char c)
{
	char		**ptr;
	int			len;

	if (!s)
		return (NULL);
	len = ft_calcul_rows(s, c);
	if (!(ptr = (char**)malloc(sizeof(char*) * (len + 1))))
	{
		ft_strfree(ptr, len);
		return (NULL);
	}
	if (!(ft_magic(ptr, s, c, len)))
	{
		ft_strfree(ptr, len);
		return (NULL);
	}
	return (ptr);
}
