/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dioux_transitions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 19:50:45 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 22:09:28 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int			ft_num_sys(char flag)
{
	int		num_system;

	num_system = 0;
	(flag == 'b') ? num_system = 2 : num_system;
	(flag == 'x' || flag == 'X') ? num_system = 16 : num_system;
	(flag == 'o') ? num_system = 8 : num_system;
	return (num_system);
}

char		*choose_length_chr(char *type,
			char *(f)(unsigned long long, int, char),
			unsigned long long u, char flag)
{
	if (ft_strcmp(type, "l") == 0)
		return (f((unsigned long)u, ft_num_sys(flag), flag));
	else if (ft_strcmp(type, "ll") == 0)
		return (f((unsigned long long)u, ft_num_sys(flag), flag));
	else if (ft_strcmp(type, "h") == 0)
		return (f((unsigned short int)u, ft_num_sys(flag), flag));
	else if (ft_strcmp(type, "hh") == 0)
		return (f((unsigned char)u, ft_num_sys(flag), flag));
	return (0);
}

char		*choose_length_putnbr(char *type, char *(f)(long long), long long u)
{
	if (ft_strcmp(type, "l") == 0)
		return (f((long)u));
	else if (ft_strcmp(type, "ll") == 0)
		return (f((long long)u));
	else if (ft_strcmp(type, "h") == 0)
		return (f((short int)u));
	else if (ft_strcmp(type, "hh") == 0)
		return (f((char)u));
	return (0);
}
