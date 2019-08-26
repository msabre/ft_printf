/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 02:08:48 by andrejskobe       #+#    #+#             */
/*   Updated: 2019/04/15 09:28:29 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strtrim(char const *s)
{
	char	*ptr;
	char	*str;
	int		i;

	if (!s)
		return (NULL);
	if (*s == '\0')
		return ((char *)s);
	i = 0;
	while (*s == '\t' || *s == '\n' || *s == ' ')
		s++;
	ptr = (char *)s;
	while (*(ptr + 1) != '\0')
		ptr++;
	while (*ptr == '\t' || *ptr == '\n' || *ptr == ' ')
		ptr--;
	ptr++;
	if (!(str = (char *)malloc(sizeof(char) * (ptr - s + 1))))
		return (NULL);
	while (s != ptr)
		str[i++] = *(s++);
	str[i] = '\0';
	return (str);
}
