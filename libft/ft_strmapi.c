/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 02:56:50 by andrejskobe       #+#    #+#             */
/*   Updated: 2019/04/15 08:25:52 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	int		len;
	char	*ptr;

	len = 0;
	i = 0;
	if (s && f)
	{
		while (s[len] != '\0')
			len++;
		if (!(ptr = (char *)malloc(sizeof(char) * len + 1)))
			return (NULL);
		while (s[i] != '\0')
		{
			ptr[i] = f(i, s[i]);
			i++;
		}
		ptr[i] = '\0';
	}
	else
		return (NULL);
	return (ptr);
}
