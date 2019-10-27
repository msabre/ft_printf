/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_str_fr_int_mass.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 20:17:57 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 20:46:07 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

char			*str_fr_intmass(int *a, int size, t_list *l, int sign)
{
	char		*str;
	int			i;

	i = 0;
	if (!a)
		return (NULL);
	size = (l->fhash && l->precision == 0) ? size++ : size;
	sign = (l->precision == 0) ? sign : 0;
	if (!(str = (char*)malloc(sizeof(char) * (size + sign + 1))))
		return (NULL);
	size -= (l->fhash && l->precision == 0) ? 2 : 1;
	if (sign == 1)
		str[i++] = '-';
	while (size >= 0)
		str[i++] = a[size--] + 48;
	if (l->fhash && l->precision == 0)
		str[i++] = '.';
	str[i] = '\0';
	return (str);
}
