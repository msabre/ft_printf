/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:54:49 by msabre            #+#    #+#             */
/*   Updated: 2019/04/17 18:08:57 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*new;
	t_list	*ptr;
	t_list	*begin;

	begin = NULL;
	if (lst && (*f))
	{
		begin = (t_list*)malloc(sizeof(t_list));
		begin = (*f)(lst);
		ptr = begin;
		lst = lst->next;
		while (lst != NULL)
		{
			new = (t_list*)malloc(sizeof(t_list));
			new = (*f)(lst);
			ptr->next = new;
			ptr = new;
			lst = lst->next;
		}
	}
	return (begin);
}
