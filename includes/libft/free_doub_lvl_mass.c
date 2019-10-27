/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_doub_lvl_mass.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 20:53:13 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 20:57:16 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		*free_doub_lvl_mass(void **mas)
{
	char	**ptr;
	int		i;

	i = 0;
	if (!mas)
		return (NULL);
	ptr = (char**)mas;
	while (ptr[i] != NULL)
		free(ptr[i++]);
	free(ptr);
	return (NULL);
}
