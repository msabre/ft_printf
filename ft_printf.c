/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/11 22:56:09 by msabre            #+#    #+#             */
/*   Updated: 2019/08/26 10:23:56 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int						mod_minus(int a, int count)
{
	int					b;

	b = 1;
	if (a < 0)
	{
		a = -a;
		b = -1;
	}
	a = (a - count) * b;
	return (a);
}

void				zero_flags(t_list *l)
{
	l->fhash = 0;
	l->fzero = 0;
	l->fminus = 0;
	l->fplus = 0;
	l->spase = ' ';
	l->length = 0;
}

char				*ft_str_rev(char **str)
{	
	char			*str_reverse;
	int				size;
	int				i;
	int				stop;

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
	return (str_reverse);
}

char					*decimy_to_any(unsigned long long num_integer, int num_system, char flag)
{
	char				*result;
	char				*result_reverse;
	int					alp_register;
	int					index;
	int					digit;
	
	index = 0;
	alp_register = 65;
	(flag == 'x') ? alp_register = 97 : alp_register;
	if (!(result = (char*)malloc(sizeof(char) * 300)))
		return (NULL);
	while (num_integer)
	{
		digit = num_integer % num_system;
		(digit > 9) ? digit = alp_register + (digit % 10) - 48 : digit;
		result[index] = digit + 48;
		num_integer /= num_system;
		index++;
	}
	result[index] = '\0';
	if (!(result_reverse = ft_str_rev(&result)))
		return (NULL);
	free(result);
	return (result_reverse);
}

char					*ft_itoa_usigned(unsigned long long n, int count, char flag)
{
	unsigned long long	save;
	char				*ptr;
	
	save = n;
	if (n == 0)
		return ("0");
	while (save > 0)
	{
		save = save / 10;
		count++;
	}
	if (!(ptr = (char*)malloc(sizeof(char) * (count + 1))))
		return (NULL);
	ptr[count] = '\0';
	while (count > 0)
	{
		ptr[--count] = (n % 10) + 48;
		n = n / 10;
	}
	return (ptr);
}

char					*ft_putnbr1(long long n, int count)
{
	char				*integer;
	int					i;

	i = 0;
	if (!(integer = (char*)malloc(sizeof(char) * 20)))
		return (NULL);
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

int						ft_num_sys(char flag)
{
	int num_system;

	num_system = 0;
	(flag == 'x' || flag == 'X') ? num_system = 16 : num_system;
	(flag == 'o') ? num_system = 8 : num_system;
	return (num_system);
}

char					*choose_length_int(char *type, char *(*f)(long long, int), long long d)
{
	if (ft_strcmp(type, "l") == 0)
		return (f((long)d, 0));
	else if (ft_strcmp(type, "ll") == 0)
		return (f((long long)d, 0));
	else if (ft_strcmp(type, "h") == 0)
		return (f((short int)d, 0));
	else if (ft_strcmp(type, "hh") == 0)
		return (f((char)d, 0));
	return (0);
}

char					*choose_length_chr(char *type, char *(f)(unsigned long long, int, char),
											unsigned long long u, char flag)
{
	if (ft_strcmp(type, "l") == 0)
		return (f((unsigned long)u, ft_num_sys(flag), flag));
	else if (ft_strcmp(type, "ll") == 0)
		return (f((unsigned long long)u, ft_num_sys(flag), flag));
	else if (ft_strcmp(type, "h") == 0)
		return (f((unsigned short int)u, ft_num_sys(flag), flag));
	else if (ft_strcmp(type, "hh") == 0)
		return (f((unsigned char)u, ft_num_sys(flag), flag));
	return (0);
}

void					hash_and_plus_check(t_list *l, const char *format, char *out)
{
	if (l->fplus && l->spase != '0' && out[0] != '-'
		&& (format[l->i] == 'd' || format[l->i] == 'i'))
		write(1, "+", 1);
	if (((l->fhash && ft_memchr("xX", format[l->i], 2))
		|| (l->fhash && format[l->i] == 'p' && !l->fzero)))
	{
		(format[l->i] == 'x' || format[l->i] == 'p') ? write(1, "0x", 2) : 1;
		(format[l->i] == 'X') ? write(1, "0X", 2) : 1;
	}
	else if (l->fhash && format[l->i] == 'o')
		write(1, "0", 1);
}

void					flag_initialization(t_list *l, char **out, const char *format)
{
	if (l->fzero)
	{
		l->spase = '0';
		if ((*out)[0] == '-')
		{
			write(1, "-", 1);
			(*out)++;
		}
	}
	if ((l->fhash && ft_memchr("xX", format[l->i], 2))
		|| (l->fhash && format[l->i] == 'p'))
		l->length = mod_minus(l->length, 2);
	else if (l->fhash && format[l->i] == 'o')
		l->length = mod_minus(l->length, 1);
	if (l->fplus && (format[l->i] == 'd' || format[l->i] == 'i'))
		l->length = mod_minus(l->length, 1);
}

void					chr_output(t_list *l, char *out, int out_length, const char *format)
{
	int					count;
	int					count_summ;

	flag_initialization(l, &out, format);
	count = l->length;
	if ((count_summ = count - out_length) > 0)
	{
		(l->fplus && l->spase == '0' && (format[l->i] == 'd' || format[l->i] == 'i')
			&& out[0] != '-') ? write(1, "+", 1) : 1;
		(l->spase == '0' && format[l->i] == 'p')
			? write(1, "0x", 2) : 1;
		while (count_summ-- > 0)
			write(1, &l->spase, 1);
		hash_and_plus_check(l, format, out);
		write(1, out, out_length);
	}
	else if ((count_summ = count + out_length) < 0)
	{
		hash_and_plus_check(l, format, out);
		write(1, out, out_length);
		while (count_summ++ != 0)
			write(1, &l->spase, 1);
	}
	else
	{
		hash_and_plus_check(l, format, out);
		write(1, out, out_length);
	}
}

int						output_di_flags(const char *format, va_list args,
									char *type, t_list *l)
{
	long long			d;
	char				*d_chr;
	int					out_length;

	d = va_arg(args, long long);
	if (format[l->i] == 'i' || format[l->i] == 'd')
		d_chr = ft_putnbr1((int)d, 0);
	else
		d_chr = choose_length_int(type, ft_putnbr1, d);
	if (!d_chr)
		return (-1);
	out_length = ft_strlen(d_chr);
	chr_output(l, d_chr, out_length, format);
	free(d_chr);
	return (l->count);
}

int						output_u_flags(const char *format, va_list args,
									char *type, t_list *l)
{
	unsigned long long	u;
	char				*out;
	int					count;

	u = va_arg(args, unsigned long long);
	if (type[0] == 'i')
		out = ft_itoa_usigned((unsigned int)u, 0, type[0]);
	else
		out = choose_length_chr(type, ft_itoa_usigned, u, 'u');
	if (!out)
		return (-1);
	count = ft_strlen(out);
	chr_output(l, out, count, format);
	free(out);
	return (count);
}

int						output_xo_flags(const char* format, va_list args,
									t_list *l, char *type)
{
	unsigned long long	xo;
	char				flag;
	char				*output;
	int					count;
	int					num_system;
	
	flag = type[0];
	xo = va_arg(args, unsigned long long);
	if (!(*type))
		output = decimy_to_any((unsigned int)xo,
			ft_num_sys(format[l->i]), format[l->i]);
	else
		output = choose_length_chr(type, decimy_to_any, xo, format[l->i]);
	if (!output)
		return (-1);
	count = ft_strlen(output);
	chr_output(l, output, count, format);
	free(output);
	return (1);
}

int						output_p_flags(const char* format, va_list args,
									t_list *l, char *type)
{
	unsigned long		p;
	char				*integer;
	void				*adres;
	int					count;

	adres = va_arg(args, void*);
	p = (long)(adres);
	integer = decimy_to_any((unsigned long)p, 16, 'x');
	if (!integer)
		return (-1);
	count = ft_strlen(integer);
	l->fhash = 1;
	chr_output(l, integer, count, format);
	free(integer);
	return (1);
}

int					output_cs_flags(const char *format, va_list args, t_list *l)
{
	int				count;
	char			*str;
	char			c;

	if (format[l->i] == 's')
	{
		str = va_arg(args, char*);
		count = ft_strlen(str);
	}
	else
	{
		if (!(str = (char*)malloc(sizeof(char))))
			return (-1);
		c = va_arg(args, int);
		str[0] = c;
	}
	chr_output(l, str, count, format);
	return (1);
}

int					ft_flag_function_find(const char *format, va_list args, t_list *l, char *type)
{
	int 			i;

	i = 0;
	while (format[l->i])
	{
		if (ft_memchr("di", format[l->i], 2))
			return (output_di_flags(format, args, type, l));
		else if (format[l->i] == 'u')
			return (output_u_flags(format, args, type, l));
		else if (ft_memchr("xXo", format[l->i], 3))
			return (output_xo_flags(format, args, l, type));
		else if (ft_memchr("cs", format[l->i], 2))
			return (output_cs_flags(format, args, l));
		else if (format[l->i] == 'p')
			return (output_p_flags(format, args, l, type));
	}
	return (0);
}

// void					procent_check(const char *format, t_list *l)
// {
	
// }

void					flags_check(const char *format, t_list *l)
{
	int					i;

	i = l->i;
	while (format[l->i])
	{
		if (format[l->i] == '#')
			l->fhash = 1;
		else if (format[l->i] == '+')
			l->fplus = 1;
		else if (format[l->i] == '0'&& ((format[l->i + 1] >= 48
			&& format[l->i + 1] <= 57)|| format[l->i + 1] == '#'))
			l->fzero = 1;
		else if (format[l->i] == '-')
			l->fminus = 1;
		l->i++;
	}
	l->i = i;
}

int						length_check(const char *format, t_list *l)
{
	char				*length_output;
	int					i;

	i = 0;
	l->length = 0;
	while (!(format[l->i] >= 48 && format[l->i] <= 57)
		&& !(ft_memchr("diouxXlhscfp", format[l->i], 12)) && format)
		l->i++;
	length_output = (char*)malloc(sizeof(char)	* ft_strlen(format));
	if (!length_output)
		return (-1);
	l->spase = ' ';
	if (l->fminus)
	{
		length_output[i] = '-';
		i++;
	}
	(l->fminus && l->fzero) ? l->fzero = 0 : l->fzero;
	while (format[l->i] >= 48 && format[l->i] <= 57)
		length_output[i++] = format[l->i++];
	length_output[i] = '\0';
	(i > 0) ? l->length = ft_atoi(length_output) : l->length;
	return (1);
}

int					l_h_flags(const char *format, va_list args, t_list *l)
{
	char			*type;
	
	flags_check(format, l);
	if (!(length_check(format, l)))
		return (-1);
	while (!(ft_memchr("diouxXlhscfp", format[l->i], 12)))
		l->i++;
	if (format[l->i])
	{
		if (format[l->i + 1] == 'l')
			type = "ll";
		else if (format[l->i + 1] == 'h')
			type = "hh";
		else if (format[l->i] == 'h' && format[l->i + 1] != 'h')
			type = "h";
		else if (format[l->i] == 'l')
			type = "l";
		else
			type = "";
		l->i += ft_strlen(type);
	}
	return (ft_flag_function_find(format, args, l, type));
}

t_list				*struct_init()
{
	t_list			*l;
	
	l = (t_list*)malloc(sizeof(t_list) * 1);
	if (!l)
		return (NULL);
	l->i = 0;
	l->length = 0;
	l->count = 0;
	l->fhash = 0;
	l->fzero = 0;
	l->fminus = 0;
	l->fplus = 0;
	l->start = 0;
	l->spase = ' ';
	return (l);
}

int					ft_variants(const char *format, va_list args, t_list *l)
{
	int				save_i;

	save_i = l->i;
	while (!ft_memchr("diouxXlhscfp", format[l->i], 12))
	{
		if (format[l->i] == '%' && format[l->i + 1] == '%')
		{
			write(1, "%", 1);
		}
		if (format[l->i] == '%' && !(l->start))
			l->start = 1;
		else if (format[l->i] == '%' && l->start)
			l->start = 0;
		l->i++;
	}
	l->i = save_i;
	l_h_flags(format, args, l);
	return (1);
}

int					ft_printf(const char *format, ...)
{
	va_list			args;
	t_list			*l;

	va_start(args, format);
	if (!(l = struct_init()))
		return (-1);
	while (format[l->i])
	{
		if (format[l->i] == '%')
		{
			if (!(ft_variants(format, args, l)))
				return (-1);
			zero_flags(l);
		}
		else
			write(1, &(format[l->i]), 1);
		l->i++;
	}
	free(l);
	return (l->count);
}

int					main(int argc, char **argv)
{
	int		num_system;

	num_system = 16;
	ft_printf("%%%%7p\n",  "aaasasdasc");
	printf("%%%%7p%%\n",  "aaasasdasc");
	// ft_putstr("\033[034mddf");
	return (0);
}
