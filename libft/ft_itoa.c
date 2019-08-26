/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 23:43:03 by msabre            #+#    #+#             */
/*   Updated: 2019/08/21 16:18:38 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_count(int n)
{
	int			count;

	count = 0;
	if (n == 0)
		return (1);
	(n < 0 ? count++ : count);
	(n < 0 ? n = -n : n);
	while (n > 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

static char		*ft_check_number(int n)
{
	char		*ptr;
	char		*min;
	int			i;

	i = 0;
	min = "-2147483648";
	if (n == -2147483648)
	{
		if (!(ptr = (char*)malloc(sizeof(char) * 12)))
			return (NULL);
		while (min[i] != '\0')
		{
			ptr[i] = min[i];
			i++;
		}
		ptr[i] = '\0';
		return (ptr);
	}
	else
		return ("-Not min int value");
	return (NULL);
}

char			*ft_itoa(int n)
{
	char		*ptr;
	int			count;
	int			i;

	if (n == -2147483648)
		return (ft_check_number(n));
	i = 0;
	count = ft_count(n);
	if (!(ptr = (char*)malloc(sizeof(char) * (count + 1))))
		return (NULL);
	(n < 0 ? ptr[0] = '-' : *ptr);
	(n < 0 ? i = 1 : i);
	(n < 0 ? n = -n : n);
	ptr[count--] = '\0';
	if (n == 0)
	{
		ptr[count] = '0';
		return (ptr);
	}
	while (count >= i)
	{
		ptr[count--] = (n % 10) + 48;
		n = n / 10;
	}
	return (ptr);
}
