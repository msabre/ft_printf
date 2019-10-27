/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_long_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 20:21:20 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 21:27:13 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static int					*by_rank(unsigned long long int num)
{
	int					*result;
	int					i;

	i = 0;
	if (!(result = (int*)malloc(sizeof(int) * (integer_size(num) + 1))))
		return (NULL);
	if (num == 18446744073709551615UL)
	{
		result[i++] = 6;
		num /= 10;
	}
	while (num > 0)
	{
		result[i++] = num % 10;
		num /= 10;
	}
	result[i] = -1;
	return (result);
}

static t_num_parts			*creat_num_part(int e)
{
	t_num_parts				*ptr;
	int						count;

	count = 0;
	if (e / 64 <= 0)
		count = 2;
	else if (e / 64 > 0)
		count = e / 64 + 1;
	ptr = (t_num_parts*)malloc(sizeof(t_num_parts));
	ptr->num_part = (int**)malloc(sizeof(int*) * (count + 1));
	if (!(ptr) || !(ptr->num_part))
		return (NULL);
	return (ptr);
}

static t_num_parts			*mantis_part_to_mult(int e)
{
	t_num_parts				*ptr;
	int						i;

	i = 0;
	if (!(ptr = creat_num_part(e)))
		return (NULL);
	while (e > 0)
	{
		ptr->num_part[i++] = (e >= 64)
			? by_rank(18446744073709551615UL) : by_rank(to_power(2, e));
		e -= (e >= 64) ? 64 : e;
		if (!ptr->num_part[i - 1])
		{
			free_doub_lvl_mass((void**)(ptr->num_part));
			free(ptr);
			return (NULL);
		}
	}
	ptr->size = i;
	ptr->num_part[i] = NULL;
	return (ptr);
}

t_num_parts					**get_and_fill_numparts(t_num_parts **mant_exp,
													char *mantis, int e)
{
	int						i;
	int						j;

	j = 0;
	i = 0;
	if (!mant_exp)
	{
		free(mantis);
		return (NULL);
	}
	while (i <= 63)
	{
		if (mantis[i] == '1')
		{
			if (!(mant_exp[j++] = mantis_part_to_mult(e)))
			{
				free(mantis);
				return (NULL);
			}
		}
		e--;
		i++;
	}
	free(mantis);
	return (mant_exp);
}
