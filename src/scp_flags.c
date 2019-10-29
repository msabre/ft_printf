/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scp_flags.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 19:49:32 by msabre            #+#    #+#             */
/*   Updated: 2019/10/28 19:12:18 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int					output_p_flags(va_list args, t_list *l)
{
	unsigned long	p;
	char			*integer;
	void			*adres;

	adres = va_arg(args, void*);
	p = (long)(adres);
	integer = decimy_to_any((unsigned long)p, 16, 'x');
	if (!integer)
		return (-1);
	l->out_length = ft_strlen(integer);
	l->fhash = 1;
	l->out = integer;
	if (l->dot > 0 && l->precision == 0 && *(l->out) == '0')
	{
		l->out_length = 0;
		*(l->out) = '\0';
	}
	return (chr_output(l));
}

static void			s_flag_config(t_list *l, va_list args)
{
	l->out = va_arg(args, char*);
	l->out = (l->out == NULL) ? "(null)" : l->out;
	l->out_length = ft_strlen(l->out);
	if (l->dot && l->precision < l->out_length)
	{
		l->out = ft_strndup(l->out, 0, l->precision - 1);
		l->out_length = l->precision;
		l->cut_s = 1;
		l->free_block = 0;
	}
	else if (l->precision == 0 && l->dot != 0)
		l->out_length = 0;
}

int					output_cs_flags(va_list args, t_list *l)
{
	char			c;

	l->free_block = 1;
	if (l->sp)
		l->sp = 0;
	if (l->format[l->i] == 's')
		s_flag_config(l, args);
	else
	{
		if (!(l->out = ft_memalloc(1)))
			return (-1);
		c = va_arg(args, int);
		*(l->out) = c;
		l->free_block = (c == 0) ? 1 : 0;
		l->out_length = 1;
	}
	if (ft_strcmp(l->out, "") == 0 && l->format[l->flag] != 'c')
	{
		if (l->length == 0)
			return (1);
		if (l->length != 0)
			l->precision = 0;
	}
	chr_output(l);
	return (1);
}

int					output_b_flags(va_list args, t_list *l, char *type)
{
	long long		f;
	char			*result;

	f = va_arg(args, long long);
	if (!(*type))
		result = decimy_to_any((int)f, 2, 'b');
	else
		result = choose_length_chr(type, decimy_to_any, f, 'b');
	l->out = result;
	l->out_length = ft_strlen(result);
	chr_output(l);
	return (1);
}
