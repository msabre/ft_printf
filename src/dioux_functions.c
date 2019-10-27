/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dioux_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 19:58:50 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 20:46:25 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static char				*ft_str_rev(char **str)
{
	char				*str_reverse;
	int					size;
	int					i;
	int					stop;

	i = 0;
	stop = 0;
	size = ft_strlen(*str);
	if (!(str_reverse = (char*)malloc(sizeof(char) * (size + 1))))
	{
		free(*str);
		return (NULL);
	}
	size--;
	if (**str == '-')
	{
		str_reverse[i++] = '-';
		stop = 1;
	}
	while (size >= stop)
		str_reverse[i++] = str[0][size--];
	str_reverse[i] = '\0';
	free(*str);
	return (str_reverse);
}

static char				*get_newbase_res(unsigned long long num_integer,
								int num_system, char *result, int alp_register)
{
	int					digit;
	int					index;

	index = 0;
	while (num_integer)
	{
		digit = num_integer % num_system;
		digit = (digit > 9) ? alp_register + (digit % 10) - 48 : digit;
		result[index++] = digit + 48;
		num_integer /= num_system;
	}
	result[index] = '\0';
	return (result);
}

char					*putnbr_di(long long a)
{
	long long			n;
	char				*integer;
	int					i;

	i = 0;
	n = a;
	if (!(integer = (char*)malloc(sizeof(char) * 20)))
		return (NULL);
	(n == 0) ? integer[i++] = '0' : 1;
	if (n < 0)
	{
		integer[i++] = '-';
		n = -n;
	}
	while (n > 0)
	{
		integer[i] = n % 10 + 48;
		i++;
		n /= 10;
	}
	integer[i] = '\0';
	return (ft_str_rev(&integer));
}

char					*ft_itoa_usigned(unsigned long long n,
										int count, char flag)
{
	unsigned long long	save;
	char				*ptr;

	save = n;
	(n == 0) ? count = 1 : count;
	while (save > 0)
	{
		save = save / 10;
		count++;
	}
	if (!(ptr = (char*)malloc(sizeof(char) * (count + 1))))
		return (NULL);
	(n == 0) ? ptr[0] = '0' : 1;
	ptr[count] = '\0';
	while (count > 0)
	{
		ptr[--count] = (n % 10) + 48;
		n = n / 10;
	}
	return (ptr);
}

char					*decimy_to_any(unsigned long long num_integer,
										int num_system, char flag)
{
	char				*result;
	int					alp_register;

	alp_register = 65;
	(flag == 'x') ? alp_register = 97 : alp_register;
	if (!(result = (char*)malloc(sizeof(char) * 300)))
		return (NULL);
	if (num_integer == 0)
	{
		*result = '0';
		return (result);
	}
	get_newbase_res(num_integer, num_system, result, alp_register);
	if (!(result = ft_str_rev(&result)))
		return (NULL);
	return (result);
}
