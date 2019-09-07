/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnequ.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 22:09:11 by andrejskobe       #+#    #+#             */
/*   Updated: 2019/04/15 08:27:25 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			ft_strnequ(char const *s1, char const *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (s1 && s2)
	{
		while (*s1 != '\0' && *s2 != '\0' && i < n)
		{
			if (*s1 != *s2)
				return (0);
			s1++;
			s2++;
			i++;
		}
	}
	else
		return (0);
	return (1);
}
