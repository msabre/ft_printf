/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_define.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 20:35:24 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 19:04:30 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static void		flag_config(t_list *l)
{
	if (!ft_memchr("Xxop", l->format[l->flag], 4))
		l->fhash = 0;
	if (l->precision < 0)
		l->length = 0;
	if (l->fzero && l->fminus)
		l->fzero = -1;
	if (l->fzero && l->dot > 0 && l->format[l->flag] != 'f')
		l->fzero = 0;
	if (l->fzero > 0)
		l->spase = '0';
	if (l->fplus > 0 && l->sp > 0)
		l->sp = 0;
	if (l->fplus && (!ft_memchr("dioxXf", l->format[l->flag], 6)
		|| *(l->out) == '-'))
		l->fplus = 0;
	if (mod_compair(l->out_length, l->precision) == 1
		|| ft_memchr("cs", l->format[l->flag], 2))
		l->precision = 0;
	if (mod_compair(l->out_length, l->length) == 1
		|| mod_compair(l->out_length, l->length) == 0)
		l->length = 0;
	if (mod_compair(l->precision, l->length) == 1)
		l->length = 0;
	if (l->sp > 0 && *(l->out) == '-')
		l->sp = 0;
}

static void		fhash_config(t_list *l)
{
	int			real_length;

	real_length = l->length - l->out_length;
	if (l->format[l->flag] == 'o')
		l->hash = "0";
	else
		l->hash = (l->format[l->flag] != 'X') ? "0x" : "0X";
	l->dop_count = (l->format[l->flag] == 'o') ? 1 : 2;
	if (l->spase == ' ' && l->length > 0 && real_length >= l->dop_count)
		l->dop = l->length - (l->precision > l->out_length
			? l->precision : l->out_length) - l->dop_count;
	else
		l->dop = 0;
}

static int		get_count_spaces(t_list *l)
{
	int			count_space;

	count_space = 0;
	if (mod_compair(l->precision, l->length) == 1)
		count_space = mod_minus(l->precision, l->out_length);
	if (mod_compair(l->length, l->out_length) == 1)
	{
		count_space = mod_minus(l->length, l->out_length);
		count_space = (l->fplus >= 0)
			? mod_minus(count_space, l->fplus) : count_space;
	}
	if (mod_compair(l->dop_count, count_space) == 1)
		count_space = 0;
	count_space = (l->dop >= 0 &&
		mod_compair(count_space, l->dop_count) == 1 && l->length)
			? mod_minus(count_space, l->dop_count) : count_space;
	count_space *= (count_space < 0) ? -1 : 1;
	return (count_space);
}

char			*flag_inicializatian(t_list *l)
{
	char		*result;
	int			count_space;

	flag_config(l);
	if (l->fhash)
		fhash_config(l);
	count_space = get_count_spaces(l);
	if (*(l->out) == '-' && l->precision > 0 && l->length == 0)
		count_space++;
	if (l->sp > 0 && l->precision >= l->length && l->length >= 0)
		count_space++;
	else if (l->sp > 0 && l->precision && l->length >= 0)
		l->sp = 0;
	if (!(result = ft_memalloc((l->out_length + count_space
		+ l->dop_count + l->fplus))))
	{
		free(l->out);
		return (NULL);
	}
	(l->dop >= 0) ? result[l->dop] = '\0' : 1;
	return (result);
}
