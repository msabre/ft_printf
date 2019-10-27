/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/11 22:56:09 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 21:15:51 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static int			print_output(int length, t_list *l)
{
	length = (l->i == length) ? ft_strlen(l->buffer_for_write) : 0;
	(l->n_count > 0) ? dawrin_nulls(l) : 1;
	(length > 0) ? write(1, l->buffer_for_write, length) : 1;
	free(l->buffer_for_write);
	free(l);
	return (length);
}

static void			zero_flags(t_list *l)
{
	l->precision_minus = 0;
	l->free_block = 0;
	l->start = -1;
	l->fhash = 0;
	l->sp = 0;
	l->dot = 0;
	l->type = 0;
	l->flag = 0;
	l->start = -1;
	l->fzero = 0;
	l->fminus = 0;
	l->fplus = 0;
	l->length = 0;
	l->precision = 0;
	l->cut_s = 0;
	l->free_block = 0;
	l->dop = -1;
	l->dop_count = 0;
	l->hash = NULL;
	l->out = NULL;
	l->spase = ' ';
}

static t_list		*struct_init(const char *format)
{
	t_list			*l;

	l = (t_list*)malloc(sizeof(t_list) * 1);
	if (!l)
		return (NULL);
	l->i = 0;
	l->count = 0;
	l->n_count = 0;
	l->format = format;
	l->buffer_for_write = (char*)malloc(sizeof(char));
	if (!(l->buffer_for_write))
		return (NULL);
	*(l->buffer_for_write) = '\0';
	zero_flags(l);
	return (l);
}

static void			dawrin_nulls(t_list *l)
{
	int				i;
	int				j;

	i = 0;
	j = 0;
	while (l->darwin_null[i] >= 0)
	{
		j = l->darwin_null[i];
		l->buffer_for_write[j] = '\0';
		i++;
	}
}

int					ft_printf(const char *format, ...)
{
	va_list			args;
	t_list			*l;
	int				length;

	length = ft_strlen(format);
	va_start(args, format);
	if (!(l = struct_init(format)))
		return (-1);
	while (l->i < length)
	{
		if (format[l->i] == '%')
		{
			if (!(ft_variants(args, l)))
				break ;
			zero_flags(l);
		}
		else
		{
			if (!(add_anytext_tobuff(l)))
				break ;
		}
	}
	length = print_output(length, l);
	va_end(args);
	return (length);
}
