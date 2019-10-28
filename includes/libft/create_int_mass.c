/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_int_mass.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 20:55:02 by msabre            #+#    #+#             */
/*   Updated: 2019/10/28 18:25:07 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		*creat_int_mass(int size)
{
	int	*result;
	int	i;

	i = 0;
	result = (int*)malloc(sizeof(int) * size);
	if (!result)
		return (NULL);
	while (i < size)
		result[i++] = 0;
	return (result);
}
