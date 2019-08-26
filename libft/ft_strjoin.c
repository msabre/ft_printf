/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 22:47:52 by andrejskobe       #+#    #+#             */
/*   Updated: 2019/04/15 08:30:04 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	char	*str;
	int		len;

	if (s1 && s2)
	{
		len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
		ptr = (char*)malloc(sizeof(char) * (len + 2));
		str = ptr;
		if (!(ptr))
			return (NULL);
		while (*s1 != '\0')
			*(ptr++) = *(s1++);
		while (*s2 != '\0')
			*(ptr++) = *(s2++);
		*(ptr++) = '\0';
	}
	else
		return (NULL);
	return (str);
}
