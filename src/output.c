/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 20:36:55 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 22:01:44 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int			get_buffer(t_list *l, char *new_str)
{
	char	*ptr;

	if (new_str == NULL)
		return (1);
	ptr = l->buffer_for_write;
	l->buffer_for_write = ft_strjoin(ptr, new_str);
	(ft_strcmp(new_str, "%") != 0) ? free(new_str) : 1;
	free(ptr);
	if (!(l->buffer_for_write))
		return (-1);
	return (1);
}

int			add_anytext_tobuff(t_list *l)
{
	char	*simple_text;
	int		count;
	int		i;
	int		j;

	i = l->i;
	j = 0;
	while (l->format[l->i] != '%' && l->format[l->i] != '\0')
		l->i++;
	count = l->i - i;
	simple_text = (char*)malloc(sizeof(char) * (count + 1));
	if (!simple_text)
		return (-1);
	while (count > 0)
	{
		simple_text[j++] = l->format[i++];
		count--;
	}
	simple_text[j] = '\0';
	return (get_buffer(l, simple_text) ? 1 : -1);
}

int			chr_output(t_list *l)
{
	char	*result;

	if (!(result = flag_inicializatian(l)))
		return (-1);
	if (!(fill_output(l, result)))
		return (-1);
	(l->free_block == 0) ? free(l->out) : 1;
	return (1);
}
