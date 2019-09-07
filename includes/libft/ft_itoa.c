/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 23:43:03 by msabre            #+#    #+#             */
/*   Updated: 2019/08/29 19:56:54 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_count(long long n)
{
	int			count;

	count = 0;
	if (n == 0)
		return (1);
	(n < 0) ? count++ : count;
	(n < 0) ? n *= (-1) : n;
	while (n > 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

char			*ft_itoa(long long n)
{
	char		*ptr;
	int			count;
	int			i;

	i = 0;
	if (n == 0)
		return "0";
	count = ft_count(n);
	if (!(ptr = (char*)malloc(sizeof(char) * (count + 1))))
		return (NULL);
	(n < 0) ? *ptr = '-' : *ptr;
	(n < 0) ? i = 1 : i;
	(n < 0) ? n = -n : n;
	ptr[count--] = '\0';
	while (count >= i)
	{
		ptr[count--] = (n % 10) + 48;
		n = n / 10;
	}
	return (ptr);
}
