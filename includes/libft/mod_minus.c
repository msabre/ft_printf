/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod_minus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 20:49:05 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 20:58:18 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			mod_minus(int a, int b)
{
	int		sign;

	sign = 1;
	b = (b < 0) ? -b : b;
	if (a < 0)
	{
		a = -a;
		sign = -1;
	}
	a = (a - b) * sign;
	return (a);
}