/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flag_find.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 20:38:06 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 21:17:39 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int			ft_flag_function_find(va_list args, t_list *l, char *type)
{
	int		i;
	int		res;

	i = 0;
	res = 0;
	if (ft_memchr("di", l->format[l->i], 2))
		res = output_di_flags(args, type, l);
	else if (l->format[l->i] == 'u')
		res = output_u_flags(args, type, l);
	else if (ft_memchr("xXo", l->format[l->i], 3))
		res = output_xo_flags(args, l, type);
	else if (ft_memchr("cs", l->format[l->i], 2))
		res = output_cs_flags(args, l);
	else if (l->format[l->i] == 'p')
		res = output_p_flags(args, l, type);
	else if (l->format[l->i] == 'f')
		res = output_f_flags(args, l, type);
	else if (l->format[l->i] == 'b')
		res = output_b_flags(args, l, type);
	l->i++;
	return (res);
}
