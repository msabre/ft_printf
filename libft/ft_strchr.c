/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 19:02:44 by msabre            #+#    #+#             */
/*   Updated: 2019/04/22 15:45:38 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strchr(const char *s, int c)
{
	int		i;

	i = 0;
	if (c == '\0')
	{
		while (s[i] != '\0')
			i++;
		return ((char*)s);
	}
	while (s[i])
	{
		if (s[i] == c)
			return ((char*)s);
		i++;
	}
	return (NULL);
}
