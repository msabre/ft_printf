/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_format.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 19:45:18 by msabre            #+#    #+#             */
/*   Updated: 2019/10/28 20:11:47 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static int		wildcard_config(t_list *l, va_list args)
{
	int			length;

	length = va_arg(args, int);
	if (length > 0 && l->fminus)
		length *= (-1);
	if (l->dot > 0 && length < 0)
	{
		length = 0;
		l->dot = 0;
	}
	return (length);
}

static int		length_check(t_list *l, int sign, va_list args)
{
	char		length_output[21];
	int			length;
	int			i;
	int			j;

	i = 0;
	j = l->save;
	length = 0;
	if (l->format[j] == '*')
		return (wildcard_config(l, args));
	while (!(l->format[j] >= 48 && l->format[j] <= 57))
		j++;
	(sign > 0) ? length_output[i++] = '-' : 1;
	while (l->format[j] >= 48 && l->format[j] <= 57)
		length_output[i++] = l->format[j++];
	length_output[i] = '\0';
	i ? length = ft_atoi(length_output) : length;
	l->save = j - 1;
	l->i = l->save;
	return (length);
}

static void		flag_check(t_list *l, va_list args)
{
	if (l->format[l->save] == ' ')
		l->sp++;
	else if (l->format[l->save] == '#' && !l->fhash)
		l->fhash = l->save;
	else if (l->format[l->save] == '+' && l->fplus == 0)
		l->fplus = 1;
	else if (l->format[l->save] == '0' && !l->fzero)
		l->fzero = l->save;
	else if (l->format[l->save] == '-' && !l->fminus)
		l->fminus = l->save;
	else if (ft_isnum(l->format[l->save], 0) || l->format[l->save] == '*')
		l->length = length_check(l, l->fminus, args);
	else if (l->format[l->save] == '.')
	{
		l->dot = l->save;
		if (ft_isnum(l->format[l->save + 1], 10)
			|| ft_memchr("+-", l->format[l->save + 1], 2)
			|| l->format[l->save + 1] == '*')
		{
			l->precision_minus = (l->format[l->save + 1] == '-') ? 1 : -1;
			(ft_memchr("-*", l->format[l->save + 1], 2)) ? l->save++ : l->save;
			l->precision = length_check(l, l->precision_minus, args);
		}
	}
}

int				pars_format(t_list *l, va_list args)
{
	l->save = l->i;
	while (l->format[l->save] != '%' && l->format[l->save] != '\n'
		&& l->format[l->save])
	{
		if (!ft_memchr("diouxXscpf+-_. *Llh#br", l->format[l->save], 22)
			&& !ft_isnum(l->format[l->save], 112))
			return (0);
		if (ft_memchr("diouxXscpfbr", l->format[l->save], 12) && !(l->flag))
		{
			l->flag = l->save;
			return (1);
		}
		else if (ft_memchr("Llh", l->format[l->save], 3) && !(l->type))
			l->type = l->save;
		flag_check(l, args);
		l->save++;
	}
	l->i = l->save;
	return (1);
}
