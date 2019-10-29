/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_flag.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 20:10:13 by msabre            #+#    #+#             */
/*   Updated: 2019/10/28 19:16:07 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static char		*creat_double_chr(char *chr_order, char *mantis,
										int sign)
{
	char		*double_chr;
	int			l_order;
	int			l_mantis;
	int			i;

	i = 0;
	l_order = ft_strlen(chr_order);
	l_mantis = ft_strlen(mantis);
	if (!(double_chr = (char*)malloc(sizeof(char) *
		(l_order + l_mantis + sign + 2))))
		return (NULL);
	if (sign == 1)
	{
		double_chr[0] = '-';
		double_chr[++i] = '\0';
		l_order++;
	}
	double_chr[i] = '\0';
	ft_strcat(double_chr, chr_order);
	double_chr[l_order] = '.';
	double_chr[l_order + 1] = '\0';
	ft_strlcat(double_chr, mantis, l_mantis + l_order + 1);
	free(chr_order);
	free(mantis);
	return (double_chr);
}

static int		check_inf_or_nan(long double f, t_list *l, t_uni_dub *ptr)
{
	if (f != f)
	{
		l->sp = 0;
		l->fplus = 0;
		l->out = "nan";
	}
	else if (ptr->doub.exp == 32767 &&
			ptr->doub.mantis == 9223372036854775808UL)
	{
		if (ptr->doub.sign == 1)
			l->out = "-inf";
		else
			l->out = "inf";
	}
	if (l->out != NULL)
	{
		l->fzero = 0;
		l->free_block = 1;
		l->out_length = ft_strlen(l->out);
		l->precision = 0;
	}
	return (l->out != NULL ? chr_output(l) : -1);
}

int				output_f_flags(va_list args, t_list *l, char *type)
{
	t_uni_dub	ptr;
	long double	f;
	char		*order;
	char		*fractional;

	(!l->dot) ? l->precision = 6 : 1;
	(!(*type) || *type == 'l') ? f = va_arg(args, double) : 1;
	(*type == 'L') ? f = va_arg(args, long double) : 1;
	ptr.val = f;
	f = (ptr.doub.sign == 1) ? -f : f;
	if (check_inf_or_nan(f, l, &ptr) > 0)
		return (1);
	if (ptr.doub.exp - 16383 < 64)
		order = norm_chr_ll(f, l, (int)ptr.doub.sign);
	else if (!(order = add_to_string(&ptr, l)))
		return (-1);
	if (l->precision > 0)
	{
		fractional = creat_after_dot(f, l->precision, l, ptr.doub.exp - 16383);
		l->out = creat_double_chr(order, fractional, ptr.doub.sign);
	}
	else
		l->out = order;
	l->out_length = ft_strlen(l->out);
	return (!l->out ? -1 : chr_output(l));
}
