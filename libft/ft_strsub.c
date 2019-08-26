/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 22:21:59 by andrejskobe       #+#    #+#             */
/*   Updated: 2019/04/15 13:09:30 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	i;

	i = 0;
	if (s)
	{
		ptr = (char *)malloc(sizeof(char) * (len + 1));
		if (!(ptr))
			return (NULL);
		while (i < len)
			ptr[i++] = s[start++];
		ptr[i] = '\0';
		return (ptr);
	}
	else
		return (NULL);
	return (ptr);
}
