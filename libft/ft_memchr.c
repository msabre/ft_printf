/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 20:13:05 by msabre            #+#    #+#             */
/*   Updated: 2019/04/15 18:15:45 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	void *a;

	a = (void*)s;
	while (n > 0)
	{
		if (*((unsigned char *)a) == (unsigned char)c)
			return (a);
		a++;
		n--;
	}
	return (NULL);
}
