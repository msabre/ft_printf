/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_regular_value.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 20:16:59 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 20:44:45 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

char				*norm_chr_ll(long double f, t_list *l, int sign)
{
	char			*str;
	double long		num;
	int				i;

	i = 0;
	num = (unsigned long long int)f;
	if (num == 0 && sign == 1 && l->precision == 0)
	{
		str = ft_memalloc(2);
		str[i++] = '-';
		str[i] = '0';
		return (str);
	}
	if (l->precision == 0)
	{
		f -= (long long)f;
		f *= 10;
		num = (f >= 5) ? num + 1 : num;
		num = (sign == 1) ? -num : num;
		if (l->fhash > 0 && l->precision == 0 && l->dot)
			num *= 10;
	}
	str = ft_itoa(num);
	(l->fhash && l->precision == 0) ? str[ft_strlen(str) - 1] = '.' : 1;
	return (str);
}

void				after_dot_rounding(t_list *l, char **fractional)
{
	double long		up;
	int				i;

	up = ((*fractional)[l->precision] >= 53) ? 1 : 0;
	i = l->precision - 1;
	while (up > 0)
	{
		if ((*fractional)[i] >= 48 && (*fractional)[i] < 57)
		{
			(*fractional)[i] += 1;
			up = 0;
		}
		else if ((*fractional)[i] == 57)
		{
			(*fractional)[i] = 48;
			up++;
		}
		i--;
	}
}

char				*creat_after_dot(long double f, int precision,
											t_list *l, int e)
{
	char			*fractional;
	char			ptr;
	int				i;

	i = 0;
	f -= (long long)f;
	if (!(fractional = (char*)malloc(sizeof(char) * (precision + 2))))
		return (NULL);
	if (e < 64)
	{
		while (i <= precision)
		{
			f *= 10;
			ptr = (int)f + 48;
			fractional[i++] = ptr;
			f -= (long long)f;
		}
	}
	if (e >= 64)
		while (i <= precision)
			fractional[i++] = '0';
	fractional[i] = '\0';
	after_dot_rounding(l, &fractional);
	return (fractional);
}
