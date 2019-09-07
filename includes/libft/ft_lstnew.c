/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 17:48:50 by msabre            #+#    #+#             */
/*   Updated: 2019/04/16 20:17:16 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstnew(void const *content, size_t content_size)
{
	t_list	*ptr;

	if (!(ptr = (t_list*)malloc(sizeof(t_list))))
		return (NULL);
	if (!content)
	{
		ptr->content = NULL;
		ptr->content_size = 0;
	}
	else
	{
		if (!(ptr->content = (void*)malloc(content_size)))
			return (NULL);
		ft_memcpy(ptr->content, content, content_size);
		ptr->content_size = content_size;
		ptr->next = NULL;
	}
	return (ptr);
}
