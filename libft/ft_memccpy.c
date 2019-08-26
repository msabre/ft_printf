/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 18:29:28 by msabre            #+#    #+#             */
/*   Updated: 2019/04/17 18:15:28 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	size_t	i;
	void	*tpr;

	tpr = dst;
	i = 0;
	while (i < n)
	{
		if (((unsigned char*)src)[i] == (unsigned char)c)
		{
			tpr++;
			((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
			return (tpr);
		}
		((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
		tpr++;
		i++;
	}
	return (NULL);
}
