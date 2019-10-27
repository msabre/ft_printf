/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_long_ariphmetic.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 20:26:44 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 20:46:19 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static int			get_binary(char **src, unsigned long a)
{
	int				i;
	int				count;

	i = 63;
	(*src)[0] = '1';
	count = 1;
	while (i > 0)
	{
		if (a & 1)
		{
			(*src)[i] = '1';
			count++;
		}
		else
			(*src)[i] = '0';
		a >>= 1;
		i--;
	}
	(*src)[63] = '\0';
	return (count);
}

static void			free_struct(t_num_parts **mant_exp)
{
	int				i;
	int				j;

	j = 0;
	i = 0;
	while (mant_exp[i] != NULL)
	{
		free_doub_lvl_mass((void*)(mant_exp[i])->num_part);
		free(mant_exp[i]);
		i++;
	}
	free(mant_exp);
}

char				*add_to_string(t_uni_dub *ptr, long double f, t_list *l)
{
	t_num_parts		**mant_exp;
	char			*mantis;
	int				**result;
	int				count;

	if (!(mantis = (char*)malloc(sizeof(char) * 65)))
		return (NULL);
	count = get_binary(&mantis, ptr->doub.mantis);
	mant_exp = (t_num_parts**)malloc(sizeof(t_num_parts*) * (count + 1));
	if (!(mant_exp = get_and_fill_numparts(mant_exp, mantis,
		ptr->doub.exp - 16383)))
		return (NULL);
	mant_exp[count] = NULL;
	(count == 64) ? count-- : 1;
	result = get_bignum(&mant_exp, count - 1);
	free_struct(mant_exp);
	if (count == 1)
		mantis = str_fr_intmass(*result, size_int_mass(*result),
			l, ptr->doub.sign);
	else
		mantis = get_bn_str(result, l, ptr->doub.sign);
	free_doub_lvl_mass((void**)result);
	return (!mantis ? NULL : mantis);
}
