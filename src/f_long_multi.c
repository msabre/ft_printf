/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_long_multi.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 20:22:23 by msabre            #+#    #+#             */
/*   Updated: 2019/10/28 19:13:27 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static int				*long_to_long(int *a, int *b, int a_size,
										int b_size)
{
	unsigned long long	cur;
	unsigned long long	cr;
	int					*c;
	int					i;
	int					j;

	i = 0;
	if (!(c = creat_int_mass(a_size + b_size + 1)))
		return (NULL);
	i = 0;
	while (i < a_size)
	{
		j = 0;
		cr = 0;
		while (j < b_size || cr)
		{
			cur = c[i + j] + a[i] * (j < b_size ? b[j] : 0) + cr;
			c[i + j] = cur % 10;
			cr = cur / 10;
			j++;
		}
		i++;
	}
	return (c);
}

static int				*long_multi(int *a, int *b, int a_size, int b_size)
{
	int					*result;
	int					*c;
	int					i;
	int					j;

	i = 0;
	j = 0;
	if (!(result = creat_int_mass(a_size + b_size + 1)))
		return (NULL);
	if (!(c = long_to_long(a, b, a_size, b_size)))
		return (NULL);
	i = a_size + b_size - 1;
	while (c[i] == 0)
		i--;
	while (j <= i)
	{
		result[j] = c[j];
		j++;
	}
	result[i + 1] = -1;
	free(c);
	return (result);
}

static int				*prolific_multiplication(t_num_parts ***num, int *a,
												int i, int count)
{
	int					*b;

	while (((*num)[count])->size > 2 &&
			((*num)[count])->num_part[i + 1] != NULL)
	{
		b = long_multi(a, ((*num)[count])->num_part[i + 1],
				size_int_mass(a),
					size_int_mass(((*num)[count])->num_part[i + 1]));
		free(a);
		if (!b)
			return (NULL);
		a = b;
		i++;
	}
	return (a);
}

static int				**calculations_bn(t_num_parts ***num, int count,
										int **result, int j)
{
	int					*a;
	int					i;

	while (count >= 0)
	{
		i = 0;
		a = long_multi(((*num)[count])->num_part[i],
			((*num)[count])->num_part[i + 1],
			size_int_mass(((*num)[count])->num_part[i]),
			size_int_mass(((*num)[count])->num_part[i + 1]));
		if (!a)
			return (free_doub_lvl_mass((void**)result));
		i++;
		if (!(result[j++] = prolific_multiplication(num, a, i, count)))
			return (free_doub_lvl_mass((void**)result));
		count--;
	}
	return (result);
}

int						**get_bignum(t_num_parts ***num, int count)
{
	int					**result;
	int					j;

	j = 0;
	if (!(result = (int**)malloc(sizeof(int*) * (count + 2))))
		return (NULL);
	result[count + 1] = NULL;
	if (count == 0)
	{
		*result = ((*num)[count])->num_part[0];
		return (result);
	}
	if (!(result = calculations_bn(num, count, result, j)))
		return (NULL);
	return (result);
}
