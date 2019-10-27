/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_output.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 20:33:40 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 18:59:15 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static int					add_min_or_plus(t_list *l, char **result, int j)
{
	int						i;
	int						prec_length;

	prec_length = (l->precision > 0) ? l->precision - l->out_length : 0;
	i = (l->spase == ' ' && l->fplus > 0 && l->length > 0) ? l->length -
		l->out_length - l->fplus - prec_length : 0;
	if (*(l->out) == '-' && l->spase == '0')
	{
		(*result)[i++] = '-';
		l->length = mod_minus(l->length, 1);
		j++;
	}
	else if (*(l->out) == '-' && l->spase == ' '
			&& (l->length > 0 || l->precision > 0))
	{
		if (l->length > 0)
			i = l->length - l->out_length -
				(l->precision > 0 ? l->precision - l->out_length : 0)
					- (l->precision > 0 ? 1 : 0);
		(*result)[i++] = '-';
		l->length = mod_minus(l->length, 1);
		j++;
	}
	(l->fplus > 0) ? (*result)[i++] = '+' : 0;
	return (j);
}

static int					add_len_to_out(t_list *l, char **result, int minus)
{
	int						i;
	int						length;

	i = (l->spase == ' ' ? 0 : minus) + (l->spase == ' '
		&& l->length > 0 ? 0 : l->fplus) + l->sp
			+ (l->precision > 0 && l->length < 0
				? l->precision - l->out_length : 0)
				+ ((l->length < 0) ? l->out_length : 0);
	length = (l->length * (l->length < 0 ? -1 : 1)) - l->out_length
		- l->dop_count - l->sp - l->fplus -
			(l->precision > 0 ? l->precision - l->out_length : 0);
	if (l->dop >= 0 && (l->spase == '0' || l->fminus))
		i += l->sp + l->dop_count;
	while (length > 0)
	{
		(*result)[i++] = l->spase;
		length--;
	}
	return (i);
}

static int					add_precision(t_list *l, char **result, int minus)
{
	int						length;
	int						prec;
	int						i;

	prec = l->precision * (l->precision < 0 ? -1 : 1);
	i = (l->length <= 0 && l->precision > 0 ? l->fplus : 0) + l->sp + minus
		+ l->dop_count + (l->length > 0 && l->spase == '0'
			? l->length - l->precision : 0) + (l->spase == ' ' && l->length > 0
				? l->length - l->precision - l->dop_count : 0);
	if (l->precision < 0)
		i += l->out_length;
	length = prec - l->out_length - (l->precision > 0
		&& l->precision > l->length && l->spase == '0' ? l->dop_count : 0);
	l->spase = (l->precision > 0) ? '0' : ' ';
	while (length-- > 0)
		(*result)[i++] = l->spase;
	return (i);
}

static int					position_after_outadd(t_list *l, int minus, int i)
{
	if (l->cut_s == 1 && l->format[l->flag] == 's' && l->length > 0)
		i = l->fplus + l->sp + (l->length - l->out_length);
	if (l->length <= 0 && l->precision == 0)
		i = minus + l->fplus + l->sp +
			(l->dop >= 0 ? l->dop + l->dop_count : 0);
	else
		i += (l->fplus && l->spase != '0' ? 1 : 0) +
			(l->dop >= 0 && l->spase != '0' ? l->dop_count : 0)
				+ (minus > 0 && l->length > 0 && l->spase != '0' ? minus : 0);
	if (ft_strcmp(l->out, "") == 0 && l->format[l->flag] == 'c')
	{
		l->out = "0";
		l->darwin_null[l->n_count++] = ft_strlen(l->buffer_for_write) + i;
		l->darwin_null[l->n_count] = -1;
	}
	return (i);
}

int							fill_output(t_list *l, char *result)
{
	int						j;
	int						i;
	int						minus;

	minus = 0;
	j = 0;
	if (*(l->out) == '-' || l->fplus)
	{
		j = add_min_or_plus(l, &result, 0);
		if (j > 0)
		{
			l->out_length--;
			minus = 1;
		}
	}
	if (l->hash)
		ft_strcat(&(result[(l->dop >= 0 ? l->dop : 0)]), l->hash);
	if (l->length != 0)
		i = add_len_to_out(l, &result, minus);
	if (l->precision != 0)
		i = add_precision(l, &result, minus);
	i = position_after_outadd(l, minus, i);
	while (l->out[j])
		result[i++] = (l)->out[j++];
	return (!(get_buffer(l, result))) ? -1 : 1;
}
