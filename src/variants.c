/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variants.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 19:47:35 by msabre            #+#    #+#             */
/*   Updated: 2019/10/28 18:18:19 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static int			unknow_output(t_list *l)
{
	char			*out;
	int				length;

	if ((l->save - l->i) < 0)
	{
		out = "";
		length = 0;
		l->length--;
		l->i = l->save;
	}
	else
	{
		out = ft_strndup(l->format, l->save, l->i);
		l->out_length = ft_strlen(out);
		l->i = l->save;
		l->i++;
	}
	l->out = out;
	chr_output(l);
	return (1);
}

static char			*type_define(int lon, int shor, char flag)
{
	char			*type;

	if ((lon && shor) || lon > 2 || shor > 2
		|| lon == -1 || shor == -1 || (lon == 2 && !shor))
		type = "ll";
	else if (lon == 1 && !shor)
		type = "l";
	else if (!lon && shor == 1)
		type = "h";
	else if (!lon && shor == 2)
		type = "hh";
	else if (lon == -2)
		type = "L";
	return (type);
}

static char			*type_parse(t_list *l, int lon, int shor)
{
	while (ft_memchr("Llh", l->format[l->type], 3))
	{
		if (l->format[l->type] == 'l')
			lon = (!lon) ? 1 : lon + 1;
		else if (l->format[l->type] == 'h')
			shor = (!shor) ? 1 : shor + 1;
		else if (l->format[l->type] == 'h' && lon)
		{
			shor = -1;
			break ;
		}
		else if (l->format[l->type] == 'l' && shor)
		{
			lon = -1;
			break ;
		}
		else if (l->format[l->type] == 'L' && l->format[l->flag] == 'f')
		{
			lon = -2;
			break ;
		}
		l->type++;
	}
	return (type_define(lon, shor, l->flag));
}

static int			specs_and_flags_fing(va_list args, t_list *l)
{
	char			*type;
	int				res;

	res = pars_format(l, args);
	if (res && l->flag)
	{
		if (ft_memchr("Llh", l->format[l->type], 3)
			&& !ft_memchr("csp", l->format[l->flag], 3))
			type = type_parse(l, 0, 0);
		else
			type = "";
		l->i = l->flag;
		return (ft_flag_function_find(args, l, type));
	}
	else
		return (unknow_output(l));
	return (-1);
}

int					ft_variants(va_list args, t_list *l)
{
	while (ft_memchr("% ", l->format[l->i], 2) && l->format[l->i])
	{
		if (l->format[l->i] == ' ')
			l->sp = 1;
		if (l->format[l->i] == '%' && l->start < 0)
			l->start = l->i;
		else if (l->format[l->i] == '%' && l->start >= 0)
		{
			get_buffer(l, "%");
			l->start = -1;
			if (l->format[l->i + 1] == ' ')
			{
				l->i++;
				break ;
			}
		}
		l->i++;
	}
	if (l->start >= 0 && l->format[l->i])
		return (specs_and_flags_fing(args, l));
	return (1);
}
