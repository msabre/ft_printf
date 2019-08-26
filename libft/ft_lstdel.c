/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 20:35:13 by msabre            #+#    #+#             */
/*   Updated: 2019/08/11 17:14:16 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_lstdel(t_lf **alst)
{
	t_lf	*ptr;
	t_lf	*ptr_next;
	int		i;

	ptr = *alst;
	while (ptr != NULL)
	{
		i = 0;
		ptr_next = ptr->next;
		while (ptr->figure[i])
		{
			free(ptr->figure[i]);
			i++;
		}
		free(ptr->figure[i]);
		free(ptr->figure);
		ptr->figure = NULL;
		free(ptr);
		ptr = ptr_next;
	}
	*alst = NULL;
}
