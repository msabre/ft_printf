/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 20:47:40 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 20:58:41 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strndup(const char *str, int start, int end)
{
	int		i;
	int		length;
	char	*ptr;

	i = 0;
	length = end - start;
	ptr = (char*)malloc(sizeof(char) * (length + 1));
	if (!ptr)
		return (NULL);
	while (start <= end && str[start] != '\0')
	{
		ptr[i] = str[start];
		i++;
		start++;
	}
	ptr[i] = '\0';
	return (ptr);
}
