/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integer_size.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 20:53:59 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 20:56:48 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		integer_size(unsigned long long num)
{
	int	count;

	count = 0;
	while (num > 0)
	{
		num /= 10;
		count++;
	}
	return (count);
}
