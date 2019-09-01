/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/11 22:56:09 by msabre            #+#    #+#             */
/*   Updated: 2019/09/01 17:29:39 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int						ft_isnum(char c, int exception)
{
	if (c >= 48 && c <= 57)
	{
		if (c != exception + 48)
			return (1);
	}
	return (0);
}

int						mod_plus(int a, int b)
{
	int					sign;

	sign = 1;
	b = (b < 0) ? -b : b;
	if (a < 0)
	{
		a = -a;
		sign = -1;
	}
	a = (a + b) * sign;
	return (a);
}

int						mod_minus(int a, int b)
{
	int					sign;

	sign = 1;
	b = (b < 0) ? -b : b;
	if (a < 0)
	{
		a = -a;
		sign = -1;
	}
	a = (a - b) * sign;
	return (a);
}

int					to_power(long long a, int power)
{
	if (power == 0)
		return (1);
	a = a * to_power(a, power - 1);
	return (a);
}

void					zero_flags(t_list *l)
{
	l->precision_minus = 0;
	l->fhash = 0;
	l->type = 0;
	l->flag = 0;
	l->start = 0;
	l->fzero = 0;
	l->fminus = 0;
	l->fplus = 0;
	l->length = 0;
	l->precision = 6;
	l->spase = ' ';
}

t_list					*struct_init()
{
	t_list					*l;
	
	l = (t_list*)malloc(sizeof(t_list) * 1);
	if (!l)
		return (NULL);
	l->i = 0;
	l->count = 0;
	zero_flags(l);
	return (l);
}

char				*ft_strndup(const char *str, int start, int end)
{
	int				i;
	int				length;
	char			*ptr;

	i = 0;
	length = end - start;
	ptr = (char*)malloc(sizeof(char) * (length + 1));
	if (!ptr)
		return (NULL);
	while (start <= end)
		ptr[i++] = str[start++];
	ptr[i] = '\0';
	return (ptr);
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

char					*ft_putnbr1(unsigned long long a, int count, char flag)
{
	long long			n;
	char				*integer;
	int					i;

	i = 0;
	n = a;
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

void					hash_and_plus_check(t_list *l, const char *format, char *out)
{
	if (l->fplus && l->spase != '0' && *out != '-' && (ft_memchr("dif", format[l->i], 3)))
	{
		write(1, "+", 1);
		l->count++;
	}
	if (((l->fhash && ft_memchr("xX", format[l->i], 2))
		|| (l->fhash && format[l->i] == 'p' && !l->fzero)))
	{
		(format[l->i] == 'x' || format[l->i] == 'p') ? write(1, "0x", 2) : 1;
		(format[l->i] == 'X') ? write(1, "0X", 2) : 1;
		l->count += 2;
	}
	else if (l->fhash && format[l->i] == 'o')
	{
		l->count += 1;
		write(1, "0", 1);
	}
}

int						flag_initialization(t_list *l, char *out, const char *format, int length)
{
	if (l->fminus && l->fzero)
		l->fzero = 0;
	if (l->fzero)
	{
		l->spase = '0';
		(*out == '-') ? write(1, "-", 1) : 1;
		(*out == '-') ? (out)++ : out;
	}
	if (l->spase == '0' && format[l->i] == 'p')
	{
		l->count += 2;
		write(1, "0x", 2);
	}
	if (l->fhash && (ft_memchr("xX", format[l->i], 2) || format[l->i] == 'p'))
		(l->length != 0) ? l->length = mod_minus(l->length, 2) : 1;
	else if (l->fhash && format[l->i] == 'o')
		(l->length != 0) ? l->length = mod_minus(l->length, 1) : 1;
	if (l->fplus && (ft_memchr("dif", format[l->i], 3)) && *out != '-')
	{
		(l->spase == '0') ? write(1, "+", 1) : 1;
		(l->spase == '0') ? l->count++ : 1;
		(l->length != 0) ? l->length = mod_minus(l->length, 1) : 1;
	}
	if (l->precision < 0)
	{		
		l->length = 0;
		return (0);
	}
	if (*out == '-')
		l->precision = mod_plus(l->precision, 1);
	if (l->precision > 0)
		return (l->length = mod_minus(l->length, l->precision));
	return (mod_minus(l->length, length));
}

int						output_with_precision(t_list *l, char *out, int out_length)
{
	char				*precision;
	char				*final_out;
	char				c;
	int					prec;
	int					length;
	int					i;

	i = 0;
	c = (l->precision > 0) ? '0' : ' ';
	prec = l->precision;
	if (l->precision > 0)
		l->precision -= out_length;
	else
		l->precision += out_length;
	(l->precision < 0) ? l->precision *= (-1) : 1;
	if (l->precision == 0)
	{
		write(1, out, out_length);
		return (1);
	}
	precision = (char*)malloc(sizeof(char) * (l->precision + 1));
	if (!precision)
		return (-1);
	if (*out == '-' && prec > 0)
	{
		out++;
		precision[i++] = '-';
	}
	if (l->precision)
		while (l->precision-- > 0)
			precision[i++] = c;
	precision[i] = '\0';
	final_out = (prec > 0) ? ft_strjoin(precision, out) : ft_strjoin(out, precision);
	length = ft_strlen(final_out);
	write(1, final_out, length);
	return (1);
}

char					*ft_spaces(t_list *l, int count_summ, int out_length)
{
	char				*spaces;
	char				space;
	int					i;

	i = 0;
	space = l->spase;
	spaces = (char*)malloc(sizeof(char) * (count_summ + 1));
	while (count_summ-- > 0)
		spaces[i++] = space;
	spaces[i] = '\0';
	return (spaces);
}

void					chr_output(t_list *l, char *out, int out_length, const char *format)
{
	int					count_summ;

	count_summ = flag_initialization(l, out, format, out_length);
	if (count_summ > 0)
	{
		write(1, ft_spaces(l, count_summ, out_length), count_summ);
		hash_and_plus_check(l, format, out);
		output_with_precision(l, out, out_length);
	}
	else if (count_summ < 0)
	{
		count_summ *= (-1);
		hash_and_plus_check(l, format, out);
		output_with_precision(l, out, out_length);
		write(1, ft_spaces(l, count_summ, out_length), count_summ);
	}
	else
	{
		hash_and_plus_check(l, format, out);
		output_with_precision(l, out, out_length);
	}
	l->count += count_summ + out_length;
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

int						output_di_flags(const char *format, va_list args,
									char *type, t_list *l)
{
	long long			d;
	char				*d_chr;
	int					out_length;

	d = va_arg(args, long long);
	if (!(*type))
		d_chr = ft_putnbr1((int)d, 0, 'a');
	else
		d_chr = choose_length_chr(type, ft_putnbr1, d, 'a');
	if (!d_chr)
		return (-1);
	out_length = ft_strlen(d_chr);
	chr_output(l, d_chr, out_length, format);
	free(d_chr);
	return (1);
}

int						output_u_flags(const char *format, va_list args,
									char *type, t_list *l)
{
	unsigned long long	u;
	char				*out;
	int					count;

	u = va_arg(args, unsigned long long);
	if (!(*type))
		out = ft_itoa_usigned((unsigned int)u, 0, type[0]);
	else
		out = choose_length_chr(type, ft_itoa_usigned, u, 'u');
	if (!out)
		return (-1);
	count = ft_strlen(out);
	chr_output(l, out, count, format);
	free(out);
	return (1);
}

int						output_xo_flags(const char* format, va_list args,
									t_list *l, char *type)
{
	unsigned long long	xo;
	char				*output;
	int					count;
	int					num_system;
	
	xo = va_arg(args, unsigned long long);
	if (!(*type))
		output = decimy_to_any((unsigned int)xo, ft_num_sys(format[l->i]), format[l->i]);
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

char				*creat_double_chr(long long order, long long mantis)
{
	char			*double_chr;
	char			*chr_order;
	char			*chr_mantis;
	int				l_order;
	int				l_mantis;

	chr_order = ft_itoa(order);
	chr_mantis = ft_itoa(mantis);
	if (!chr_order || !chr_mantis)
		return (NULL);
	l_order = ft_strlen(chr_order);
	l_mantis = ft_strlen(chr_mantis);
	if (!(double_chr = (char*)malloc(sizeof(char) * (l_order + l_mantis + 1))))
		return (NULL);
	double_chr[l_order + 1] = '\0';
	double_chr = ft_strcpy(double_chr, chr_order);
	double_chr[l_order] = '.';
	double_chr = ft_strcat(double_chr, chr_mantis);
	return (double_chr);
}

int					output_f_flags(const char *format, va_list args, t_list *l, char *type)
{
	double			f;
	long long		order;
	long long		mantis;
	char			*double_num;
	int				length;

	l->precision = (!l->precision) ? 6 : l->precision;
	f = va_arg(args, double);
	order = f;
	mantis = (f - order) * to_power(10, l->precision);
	(mantis < 0) ? mantis = -mantis : mantis;
	double_num = creat_double_chr(order, mantis);
	if (!double_num)
		return (-1);
	length = ft_strlen(double_num);
	chr_output(l, double_num, length, format);
	free(double_num);
	return (1);
	
}

int					ft_flag_function_find(const char *format, va_list args, t_list *l, char *type)
{
	int 			i;
	int				res;

	i = 0;
	res = 0;
	if (ft_memchr("di", format[l->i], 2))
		res = output_di_flags(format, args, type, l);
	else if (format[l->i] == 'u')
		res = output_u_flags(format, args, type, l);
	else if (ft_memchr("xXo", format[l->i], 3))
		res = output_xo_flags(format, args, l, type);
	else if (ft_memchr("cs", format[l->i], 2))
		res = output_cs_flags(format, args, l);
	else if (format[l->i] == 'p')
		res = output_p_flags(format, args, l, type);
	else if (format[l->i] == 'f')
		res = output_f_flags(format, args, l, type);
	l->i++;
	return (res);
}

int					unknow_output(const char *format, t_list *l)
{
	char			*out;
	int				length;

	if ((l->save - l->i) < 0)
	{
		out = "";
		length = 0;
		l->length--;
		l->i = l->save;
	}
	else
	{
		out = ft_strndup(format, l->i, l->save);
		if (!out)
			return (-1);
		length = ft_strlen(out);
		l->i = l->save;
		l->i++;
	}
	chr_output(l, out, length, format);
	return (1);
}

char				*type_define(int lon, int shor)
{
	char			*type;

	if ((lon < 0 || shor < 0) || (lon == 2 && !shor))
		type = "ll";
	else if (lon == 1 && !shor)
		type = "l";
	else if(!lon && shor == 1)
		type = "h";
	else if (!lon && shor == 2)
		type = "hh";
	return (type);
}

char				*type_parse(const char *format, t_list *l)
{
	int				lon;
	int				shor;
	
	lon = 0;
	shor = 0;
	while (ft_memchr("lh", format[l->i], 2))
	{
		if (format[l->i] == 'l')
			lon = (!lon) ? 1 : lon + 1;
		else if (format[l->i] == 'h')
			shor = (!shor) ? 1 : shor + 1;
		else if (format[l->i] == 'h' && lon)
		{
			shor = -1;
			break ;
		}
		else if (format[l->i] == 'l' && shor)
		{
			lon = -1;
			break ;
		}
		l->i++;
	}
	return (type_define(lon, shor));
}

int						length_check(const char *format, t_list *l, int sign)
{
	char				*length_output;
	int					length;
	int					i;
	int					j;

	i = 0;
	j = l->save;
	length = 0;
	while (!(format[j] >= 48 && format[j] <= 57))
		j++;
	if (!(length_output = (char*)malloc(sizeof(char) * ft_strlen(format))))
		return (-1);
	if (sign > 0)
		length_output[i++] = '-';
	while (format[j] >= 48 && format[j] <= 57)
		length_output[i++] = format[j++];
	length_output[i] = '\0';
	i ? length = ft_atoi(length_output) : length;
	ft_strlen(length_output) > 1 ? l->save = j - 1 : j;
	l->i = l->save;
	return (length);
}

void				flag_check(const char *format, t_list *l)
{
	if (format[l->save] == '#' && !l->fhash)
		l->fhash = l->save;
	else if (format[l->save] == '+' && !l->fplus)
		l->fplus = l->save;
	else if (format[l->save] == '0' && !l->fzero)
		l->fzero = l->save;
	else if (format[l->save] == '-' && !l->fminus)
		l->fminus = l->save;
	else if (ft_isnum(format[l->save], 0))
		l->length = length_check(format, l, l->fminus);
	else if (format[l->save] == '.' && (ft_isnum(format[l->save + 1], 10) || ft_memchr("+-", format[l->save + 1], 2)))
	{
		l->precision_minus = (format[l->save + 1] == '-') ? 1 : -1;
		l->precision = length_check(format, l, l->precision_minus);
	}
}

int					pars_format(const char *format, t_list *l)
{
	int					save;

	l->save = l->i;
	while (format[l->save] != '%' && format[l->save] != '\n' && format[l->save])
	{
		if (!ft_memchr("diouxXscpf+-_. lh#", format[l->save], 17) && !ft_isnum(format[l->save], 112))
			return (0);
		if (ft_memchr("diouxXscpf", format[l->save], 10) && !(l->flag))
		{
			l->flag = l->save;
			return (1);
		}
		else if (ft_memchr("lh", format[l->save], 2) && !(l->type))
			l->type = l->save;
		flag_check(format, l);
		l->save++;
	}
	return (1);
}

int					specs_and_flags_fing(const char *format, va_list args, t_list *l)
{
	char			*type;
	int				res;

	res = pars_format(format, l);
	if (res && l->flag)
	{
		l->i = l->type;
		if (format [l->i] == 'l' || format[l->i] == 'h')
			type = type_parse(format, l);
		else
			type = "";
		l->i = l->flag;
		return (ft_flag_function_find(format, args, l, type));
	}
	else
		return (unknow_output(format, l));
	return (-1);
}

int					ft_variants(const char *format, va_list args, t_list *l)
{
	int				save_i;

	save_i = l->i;
	while (format[l->i] == '%' && format[l->i])
	{
		if (format[l->i] == '%' && !(l->start))
			l->start = 1;
		else if (format[l->i] == '%' && l->start)
		{
			if (format[l->i] == '%' && format[l->i - 1] == '%')
			{
				write(1, "%", 1);
				l->count += 1;
			}
			l->start = 0;
		}
		l->i++;
	}
	if (l->start)
		return (specs_and_flags_fing(format, args, l));
	return (1);
}

int					ft_printf(const char *format, ...)
{
	va_list			args;
	t_list			*l;
	int				length;

	length = ft_strlen(format);
	va_start(args, format);
	if (!(l = struct_init()))
		return (-1);
	while (l->i < length)
	{
		if (format[l->i] == '%')
		{
			if (!(ft_variants(format, args, l)))
				return (-1);
			zero_flags(l);
		}
		else
		{
			write(1, &(format[l->i++]), 1);
			l->count++;
		}
	}
	free(l);
	return (l->count);
}

int					main(int argc, char **argv)
{
	int count;
	int	count1;

	count1 = ft_printf("%-23.19d\n",  -11234567, "aaasasdasc", 1234567.1234567890);
	count = printf("%-23.19d\n", -11234567, 1234567.1234567890);
	// printf("%d\n", count);
	return (0);
}

//Строки для теста
//"1%%2%3%4%5%%%%%70pmamkapvoya\n",  "aaasasdasc"
//"123#%45d%%%%%%%#-70pmamkapvoya\n",  "aaasasdasc"
//"123#%45d%%%%%%%#+0+70pmamkapvoya\n",  -11234567, "aaasasdasc"
//"%0134jkwwww65ytcyxutrcvhbj.4+064f\n",  -11234567.123456789100
//"%123y70d-065pmamkapvoya%.3f\n",  -11234567, "aaasasdasc", 1234567.1234567890

//Если все идут hhhhhhhh то вывод будет на ll
//Если все идут llllllll то вывод будет на ll
//Если все идут hhhlhllhh и все в этом роде то вывод будет на ll