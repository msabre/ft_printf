/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_long_summ.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 20:23:20 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 21:26:05 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static int		*long_summary(int *a, int *b, int a_size, int b_size)
{
	int			*result;
	int			carry;
	int			count;
	int			i;

	i = 0;
	carry = 0;
	if (a_size > b_size)
		count = a_size;
	else
		count = b_size;
	if (!(result = creat_int_mass(count + 2)))
		return (NULL);
	while (i < count || carry)
	{
		carry += (i < a_size ? a[i] : 0) + (i < b_size ? b[i] : 0);
		result[i] = carry % 10;
		carry /= 10;
		i++;
	}
	result[i] = -1;
	return (result);
}

char			*get_bn_str(int **result, t_list *l, int sign)
{
	int			*a;
	int			*b;
	char		*str;
	int			i;

	if (!(a = long_summary(result[0], result[1], size_int_mass(result[0]),
		size_int_mass(result[1]))))
		return (free_doub_lvl_mass((void**)result));
	i = 2;
	while (result[i] != NULL)
	{
		b = long_summary(a, result[i],
			size_int_mass(a), size_int_mass(result[i]));
		free(a);
		if (!b)
			return (free_doub_lvl_mass((void**)result));
		a = b;
		i++;
	}
	str = str_fr_intmass(a, size_int_mass(a), l, sign);
	free(a);
	if (!str)
		return (NULL);
	return (str);
}
