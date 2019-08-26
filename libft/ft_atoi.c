/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 22:47:40 by msabre            #+#    #+#             */
/*   Updated: 2019/08/12 21:18:59 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

signed long long int			ft_atoi(const char *str)
{
	int							i;
	signed long long int		value;

	value = 0;
	i = 1;
	while (*str == '\t' || *str == '\v' || *str == '\n'
					|| *str == '\f' || *str == '\r' || *str == ' ')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			i = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		value = value * 10 + (*str - 48);
		if (value < 0 && i == 1)
			return (0);
		else if (value < 0 && i == -1)
			return (-1);
		str++;
	}
	return (i * value);
}
