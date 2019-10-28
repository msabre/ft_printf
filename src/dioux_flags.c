/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dioux_flags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 20:02:34 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 22:15:35 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int						output_di_flags(va_list args, char *type, t_list *l)
{
	long long			d;
	char				*d_chr;
	int					out_length;

	d = va_arg(args, long long);
	(ft_strcmp(type, "h") && d == 32768) ? d = d * (-1) : d;
	if (!(*type))
		d_chr = putnbr_di((int)d);
	else
		d_chr = choose_length_putnbr(type, putnbr_di, d);
	if (!d_chr)
		return (-1);
	l->out_length = ft_strlen(d_chr);
	(l->fplus > 0 && *d_chr == '-') ? l->fplus = 0 : 1;
	if (l->dot && l->precision == 0 && *d_chr == '0')
	{
		*d_chr = '\0';
		l->out_length = 0;
	}
	l->out = d_chr;
	return (chr_output(l));
}

int						output_u_flags(va_list args, char *type, t_list *l)
{
	unsigned long long	u;
	char				*out;
	int					count;

	u = va_arg(args, unsigned long long);
	if (l->sp)
		l->sp = 0;
	if (!(*type))
		out = ft_itoa_usigned((unsigned int)u, 0, type[0]);
	else
		out = choose_length_chr(type, ft_itoa_usigned, u, 'u');
	if (!out)
		return (-1);
	l->out_length = ft_strlen(out);
	if (l->dot && l->precision == 0 && *out == '0')
	{
		*out = '\0';
		l->out_length = 0;
	}
	l->out = out;
	return (chr_output(l));
}

static void				zero_res_xo_flags(t_list *l)
{
	if (l->dot > 0 && l->precision == 0)
	{
		*(l->out) = '\0';
		l->out_length = 0;
		if (ft_memchr("Xx", l->format[l->flag], 2))
			l->fhash = 0;
	}
	else
		l->fhash = 0;
}

int						output_xo_flags(va_list args, t_list *l, char *type)
{
	unsigned long long	xo;

	xo = va_arg(args, unsigned long long);
	if (!(*type))
		l->out = decimy_to_any((unsigned int)xo,
		ft_num_sys(l->format[l->i]), l->format[l->i]);
	else
		l->out = choose_length_chr(type, decimy_to_any, xo, l->format[l->i]);
	if (!l->out)
		return (-1);
	l->out_length = ft_strlen(l->out);
	if (*(l->out) == '0')
		zero_res_xo_flags(l);
	else if (l->precision > l->out_length && l->format[l->flag] == 'o')
		l->fhash = 0;
	return (chr_output(l));
}
