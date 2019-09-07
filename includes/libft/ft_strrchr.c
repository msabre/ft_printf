/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 19:23:46 by msabre            #+#    #+#             */
/*   Updated: 2019/04/15 08:34:22 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strrchr(const char *s, int c)
{
	char	*last_ptr;

	if (c == '\0')
	{
		while (*s != '\0')
			s++;
		return ((char*)s);
	}
	last_ptr = 0;
	while (*s != '\0')
	{
		if (*s == c)
			last_ptr = (char *)s;
		s++;
	}
	if (last_ptr > 0)
		return ((char*)last_ptr);
	return (NULL);
}
