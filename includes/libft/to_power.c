/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_power.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 20:48:23 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 20:58:55 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned long long	to_power(unsigned long long a, int power)
{
	if (power == 0)
		return (1);
	a = a * to_power(a, power - 1);
	return (a);
}
