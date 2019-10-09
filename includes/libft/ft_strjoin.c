/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 22:47:52 by andrejskobe       #+#    #+#             */
/*   Updated: 2019/10/08 20:03:56 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	int		len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (s1 && s2)
	{
		len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
		ptr = (char*)malloc(sizeof(char) * (len + 2));
		if (!(ptr))
			return (NULL);
		while (s1[j] != '\0')
			ptr[i++] = s1[j++];
		j = 0;
		while (s2[j] != '\0')
			ptr[i++] = s2[j++];
		ptr[i] = '\0';
	}
	else
		return (NULL);
	return (ptr);
}
