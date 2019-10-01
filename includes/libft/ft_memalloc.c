/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 21:30:06 by msabre            #+#    #+#             */
/*   Updated: 2019/09/25 18:00:06 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		*ft_memalloc(size_t size)
{
	char	*ptr;
	size_t	i;

	i = 0;
	if (!(ptr = (char*)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	while (i < size)
		ptr[i++] = '0';
	ptr[i] = '\0';
	return (ptr);
}
