/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/11 22:56:09 by msabre            #+#    #+#             */
/*   Updated: 2019/10/02 20:56:24 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static int						ft_isnum(char c, int exception)
{
	if (c >= 48 && c <= 57)
	{
		if (c != exception + 48)
			return (1);
	}
	return (0);
}

static int						mod_compair(int a, int b)
{
	a = (a < 0) ? -a : a;
	b = (b < 0) ? -b : b;
	if (a > b)
		return (1);
	else if (b > a)
		return (2);
	else
		return (0);
}

static int						mod_plus(int a, int b)
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

static int						mod_minus(int a, int b)
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

static unsigned long long					to_power(unsigned long long a, int power)
{
	if (power == 0)
		return (1);
	a = a * to_power(a, power - 1);
	return (a);
}

static void					zero_flags(t_list *l)
{
	l->precision_minus = 0;
	l->fhash = 0;
	l->sp = 0;
	l->dot = 0;
	l->type = 0;
	l->flag = 0;
	l->start = 0;
	l->fzero = 0;
	l->fminus = 0;
	l->fplus = 0;
	l->length = 0;
	l->precision = 0;
	l->spase = ' ';
}

static t_list					*struct_init()
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

static char				*ft_strndup(const char *str, int start, int end)
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

static char				*ft_str_rev(char **str)
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

static char					*decimy_to_any(unsigned long long num_integer, int num_system, char flag)
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
	if (num_integer == 0)
	{
		*result = '0';
		return (result);
	}
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

static char					*ft_itoa_usigned(unsigned long long n, int count, char flag)
{
	unsigned long long		save;
	char					*ptr;
	
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

static char					*ft_putnbr1(long long a)
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

static int						ft_num_sys(char flag)
{
	int num_system;

	num_system = 0;
	(flag == 'x' || flag == 'X') ? num_system = 16 : num_system;
	(flag == 'o') ? num_system = 8 : num_system;
	return (num_system);
}

static void					hash_and_plus_check(t_list *l, const char *format, char *out)
{
	if (!(*out) && (l->precision == 0) && !ft_memchr("op", format[l->i], 2))
		return ;
	if (l->fplus && l->spase != '0' && *out != '-'
		&& (ft_memchr("dif", format[l->i], 3)))
	{
		write(1, "+", 1);
		l->count++;
	}
	if (l->fhash && !l->fzero && ((ft_memchr("xX", format[l->i], 2)
		&& *out != '0') || (format[l->i] == 'p')))
	{
		(format[l->i] == 'x' || format[l->i] == 'p') ? write(1, "0x", 2) : 1;
		(format[l->i] == 'X') ? write(1, "0X", 2) : 1;
		l->count += 2;
	}
	else if (l->fhash && format[l->i] == 'o' && *out != '0')
	{
		l->count += 1;
		(l->precision != 0) ? l->precision = mod_minus(l->precision, 1) : 1;
		write(1, "0", 1);
	}
}

static void					f_zero(t_list *l, char ***out, const char *format)
{
	if (l->fzero && l->precision == 0)
	{
		l->spase = '0';
		(***out == '-') ? write(1, "-", 1) : 1;
		(***out == '-') ? (**out)++ : **out;
	}
	if (l->fplus && (ft_memchr("dif", format[l->i], 3)) && ***out != '-')
	{
		(l->spase == '0') ? write(1, "+", 1) : 1;
		(l->spase == '0') ? l->count++ : 1;
		(l->length != 0) ? l->length = mod_minus(l->length, 1) : 1;
	}
	if (l->spase == '0' && (format[l->i] == 'p'
		|| (ft_memchr("Xx", format[l->i], 2) && l->fhash)))
	{
		l->count += 2;
		write(1, "0x", 2);
	}
}

static int						define_countsumm(t_list *l, int length, const char *format)
{
	if (l->length != 0)
	{
		if (format[l->i] == 's')
		{
			length = mod_minus(length, l->precision);
			return (mod_minus(l->length, length));
		}
		if (l->precision > 0 && l->precision > length)
			return (l->length = mod_minus(l->length, l->precision));
		return (mod_minus(l->length, length));
	}
	return (0);
}

static int						flag_initialization(t_list *l, char **out, const char *format, int length)
{
	(**out == '-' && l->fplus) ? l->fplus = 0 : 1;
	if (l->sp && !l->fplus && **out != '-')
	{
		write(1, " ", 1);
		l->length--;
		l->count++;	
	}
	(mod_compair(l->precision, l->length) == 1) ? l->length = 0 : 1;
	if (**out == '0' && l->dot != 0 && l->precision == 0)
	{
		// free(*out);
		*out = "";
		return (l->length);
	}
	(**out == '-' && l->precision > 0 && !l->length) ? l->precision++ : 1;
	if (mod_compair(l->length, length) != 1 || (format[l->i] == 'p' && mod_compair(l->length, length + 2) != 1))
		return (0);
	(l->fminus && l->fzero) ? l->fzero = 0 : 1;
	if (l->fhash && (ft_memchr("xX", format[l->i], 2) || format[l->i] == 'p'))
		(l->length != 0) ? l->length = mod_minus(l->length, 2) : 1;
	else if (l->fhash && format[l->i] == 'o')
		(l->length != 0) ? l->length = mod_minus(l->length, 1) : 1;
	f_zero(l, &out, format);
	if (l->precision < 0)
	{
		l->length = 0;
		return (0);
	}
	return (**out || (!(**out) && format[l->i] == 'c'))
		? define_countsumm(l, length, format) : l->length;
}

static char						*precision_config(t_list *l, char **out, int out_length, int prec)
{
	char						*precision;
	char						c;
	int							i;

	i = 0;
	c = (l->precision > 0) ? '0' : ' ';
	l->precision = mod_minus(l->precision, out_length);
	l->precision = (l->precision > 0) ? l->precision : l->precision * (-1);
	if (!(precision = (char*)malloc(sizeof(char) * (l->precision + 1))))
		return (NULL);
	if (**out == '-' && prec > 0)
	{
		(*out)++;
		precision[i++] = '-';
	}
	while (l->precision-- > 0)
		precision[i++] = c;
	precision[i] = '\0';
	return (precision);
}

static int						output_with_precision(t_list *l, char *out, int out_length, const char *format)
{
	char				*precision;
	char				*final_out;
	char				c;
	int					prec;

	if (!(*out) && format[l->i] != 'c')
		return (0);
	final_out = out;
	prec = l->precision;
	if (l->precision != 0 && l->precision > out_length
		&& !ft_memchr("cs", format[l->i], 2))
	{
		if (!(precision = precision_config(l, &out, out_length, prec)))
			return (-1);
		final_out = (prec > 0) ? ft_strjoin(precision, out) : ft_strjoin(out, precision);
		out_length = ft_strlen(final_out);
	}
	write(1, final_out, out_length);
	return (out_length);
}

static char					*ft_spaces(t_list *l, int count_summ, int out_length)
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

static void					chr_output(t_list *l, char *out, int out_length, const char *format)
{
	int					count_summ;

	count_summ = flag_initialization(l, &out, format, out_length);
	if (!(*out) && format[l->i] != 'c')
		out_length = 0;
	if (count_summ > 0)
	{
		write(1, ft_spaces(l, count_summ, out_length), count_summ);
		hash_and_plus_check(l, format, out);
		out_length = output_with_precision(l, out, out_length, format);
	}
	else if (count_summ < 0)
	{
		count_summ *= (-1);
		hash_and_plus_check(l, format, out);
		out_length = output_with_precision(l, out, out_length, format);
		write(1, ft_spaces(l, count_summ, out_length), count_summ);
	}
	else
	{
		hash_and_plus_check(l, format, out);
		out_length = output_with_precision(l, out, out_length, format);
	}
	l->count += count_summ + out_length;
}

static char					*choose_length_chr(char *type, char *(f)(unsigned long long, int, char),
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

static char					*choose_length_putnbr(char *type, char *(f)(long long), long long u)
{
	if (ft_strcmp(type, "l") == 0)
		return (f((long)u));
	else if (ft_strcmp(type, "ll") == 0)
		return (f((long long)u));
	else if (ft_strcmp(type, "h") == 0)
		return (f((short int)u));
	else if (ft_strcmp(type, "hh") == 0)
		return (f((char)u));
	return (0);
}

static int						output_di_flags(const char *format, va_list args,
									char *type, t_list *l)
{
	long long			d;
	char				*d_chr;
	int					out_length;

	d = va_arg(args, long long);
	(ft_strcmp(type, "h") && d == 32768) ? d = d * (-1) : d;
	if (!(*type))
		d_chr = ft_putnbr1((int)d);
	else
		d_chr = choose_length_putnbr(type, ft_putnbr1, d);
	if (!d_chr)
		return (-1);
	out_length = ft_strlen(d_chr);
	if ((l->fzero || (mod_compair(l->length, l->precision) == 1
		&& mod_compair(l->precision, out_length) == 1)) && l->length > out_length
			&& *d_chr == '-')
	{
		out_length--;
		l->length--;
		(l->precision == 0) ? l->count++ : l->count;
	}
	chr_output(l, d_chr, out_length, format);
	free(d_chr);
	return (1);
}

static int						output_u_flags(const char *format, va_list args,
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

static int						output_xo_flags(const char* format, va_list args,
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
	if (*output)
		free(output);
	return (1);
}

static int						output_p_flags(const char* format, va_list args,
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

static int					output_cs_flags(const char *format, va_list args, t_list *l)
{
	int				count;
	char			*str;
	char			c;

	count = 1;
	if (format[l->i] == 's')
	{
		str = va_arg(args, char*);
		if (str == NULL)
			str = "(null)";
		count = ft_strlen(str);
		if (l->precision != 0)
		{
			if (l->precision < count)
				count = l->precision;
			l->precision = 0;
		}
		else if (l->precision == 0 && l->dot != 0)
			count = 0;
	}
	else
	{
		if (!(str = (char*)malloc(sizeof(char))))
			return (-1);
		c = va_arg(args, int);
		*str = c;
	}
	chr_output(l, str, count, format);
	return (1);
}

// static char				*creat_double_chr(long long order, char *mantis, int sign)
// {
// 	char			*double_chr;
// 	char			*chr_order;
// 	int				l_order;
// 	int				l_mantis;

// 	chr_order = ft_itoa(order);
// 	if (!chr_order)
// 		return (NULL);
// 	l_order = ft_strlen(chr_order);
// 	l_mantis = ft_strlen(mantis);
// 	if (!(double_chr = (char*)malloc(sizeof(char) * (l_order + l_mantis + sign + 1))))
// 		return (NULL);
// 	double_chr[l_order + 1] = '\0';
// 	if (sign == 1)
// 	{
// 		*double_chr = '-';
// 		double_chr[1] = '\0';
// 		l_order++;
// 	}
// 	double_chr = ft_strcat(double_chr, chr_order);
// 	double_chr[l_order] = '.';
// 	double_chr[l_order + 1] = '\0';
// 	double_chr = ft_strcat(double_chr, mantis);
// 	return (double_chr);
// }

// static char					*creat_mantis(long double f, int precision)
// {
// 	char					*mantis;
// 	char					*ptr;
// 	double					f_ptr;
// 	int						i;

// 	i = 0;
// 	f -= (long long)f;
// 	mantis = (char*)malloc(sizeof(char) * (precision + 1));
// 	while (i <= precision)
// 	{
// 		f *= 10;
// 		ptr = ft_itoa((long long)f);
// 		mantis[i++] = *ptr;
// 		free(ptr);
// 		f -= (long long)f;
// 	}
// 	mantis[i] = '\0';
// 	return (mantis);
// }

// static void					mantis_rounding(t_list *l, char **mantis)
// {
// 	int						i;
// 	int						up;

// 	up = ((*mantis)[l->precision] >= 53) ? 1 : 0;
// 	i = l->precision - 1;
// 	while (up > 0)
// 	{
// 		if ((*mantis)[i] >= 48 && (*mantis)[i] < 57)
// 		{
// 			(*mantis)[i] += 1;
// 			up = 0;
// 		}
// 		else if ((*mantis)[i] == 57)
// 		{
// 			(*mantis)[i] = 48;
// 			up++;
// 		}
// 		i--;
// 	}
// 	(*mantis)[l->precision] = '\0';
// }

// static long long			*long_long_mult(t_num_parts *digits)
// {
// 	long long				*a;

// 	a = (long long*)malloc(sizeof(long long));
// 	return (a);
// }

// static t_num_parts			*new_short(int e, char *mantis)
// {
// 	t_num_parts				*ptr;
// 	int						i;

// 	i = 0;
// 	ptr = (t_num_parts*)malloc(sizeof(t_num_parts));
// 	if (!ptr)
// 		return (NULL);
// 	ptr->num_part = (unsigned long long*)malloc(sizeof(unsigned long long));
// 	if (!(ptr->num_part))
// 		return (NULL);
// 	while (e >= 0)
// 	{
// 		if (mantis[i++] == '1')
// 			*(ptr->num_part) += to_power(2, e--);
// 	}
// 	return (ptr);
// }

// static t_num_parts			*new_ll(int e)
// {
// 	t_num_parts				*l;
// 	const char				*ll_int;
// 	int						last_power;
// 	int						count;
// 	int						i;

// 	i = 0;
// 	ptr = NULL;
// 	ll_int = "4294967296";
// 	l = (t_num_parts*)malloc(sizeof(t_num_parts));
// 	count = 0;
// 	while (e >= 32)
// 	{
// 		e -= 32;
// 		count++;
// 	}
// 	last_power = (mod_compair(e, 0) != 0) ? 1 : 0;
// 	l->num_part = (unsigned long long*)malloc(sizeof(unsigned long long) * (count + last_power));
// 	while(i < count)
// 	{
// 		while (ll_int[i])
// 		{
			
// 			l->num_part[i++] = to_power(2, 32);
// 		}
// 	}
// 	if (e > 0)
// 		l->num_part[i] = to_power(2, e);
// 	return (l);
// }

static int					size_int_mass(int *a)
{
	int						i;

	i = 0;
	while (a[i])
		i++;
	return (i);
}

static int					integer_size(unsigned long long num)
{
	int						count;

	count = 0;
	while (num > 0)
	{
		num /= 10;
		count++;
	}
	return (count);
}

static int					*creat_res(int size)
{
	int						*result;
	int						i;

	i = 0;
	result = (int*)malloc(sizeof(int) * size);
	if (!result)
		return (NULL);
	while (i <= size)
		result[i++] = 0;
	return (result);
}

static int					*long_multi(int *a, int *b, int a_size, int b_size)
{
	int 					*result;
	unsigned long long		cr;
	int						k;
	int						i;
	int						j;

	i = 0;
	if (!(result = creat_res(a_size + b_size)))
		return (NULL);
	while (i < a_size)
	{
		j = 0;
		while (j < b_size)
		{
			cr = a[i] * b[j];
			k = i + j;
			while (cr > 0)
			{
				cr += result[k];
				result[k] = cr % (10000);
				cr /= (10000);
				k++;
			}
			j++;
		}
		i++;
	}
	return (result);
}

static int					multi_parts_num(t_num_parts ***num, int count)
{
	int						*a;
	int						*b;
	int						i;
	int						size;

	while (count >= 0)
	{
		i = 0;
		a = long_multi(((*num)[count])->num_part[i], ((*num)[count])->num_part[i + 1],
			size_int_mass(((*num)[count])->num_part[i]), size_int_mass(((*num)[count])->num_part[i + 1]));
		i++;
		while (((*num)[count])->size > 2 && ((*num)[count])->num_part[i + 1] != NULL)
		{
			b = long_multi(a, ((*num)[count])->num_part[i + 1], size_int_mass(a),
				size_int_mass(((*num)[count])->num_part[i + 1]));
			free(a);
			a = b;
			i++;
		}
		size = size_int_mass(a) - 1;
		while (size >= 0)
			printf("%d", a[size--]);
		printf("\n");
		free(a);
		count--;
	}
	return (1);
}

static int					*by_rank(unsigned long long num)
{
	int					*result;
	int					i;	

	i = 0;
	result = (int*)malloc(sizeof(int) * integer_size(num));
	while (num > 0)
	{
		result[i++] = num % 10000;
		num /= 10000;
	}
	return(result);
}

static t_num_parts			*mantis_part_to_mult(int e)
{
	t_num_parts				*ptr;
	int						i;

	i = 0;
	ptr = (t_num_parts*)malloc(sizeof(t_num_parts));
	ptr->num_part = (int**)malloc(sizeof(int*) * (e / 64 + 1));
	if (!(ptr) || !(ptr->num_part))
		return (NULL);
	while (e > 0)
	{
		if (e >= 64)
		{
			ptr->num_part[i++] = by_rank(18446744073709551615);
			e -= 64;
		}
		else
		{
			ptr->num_part[i++] = by_rank(to_power(2, e));
			e = 0;
		}
	}
	ptr->size = i;
	return (ptr);
}

static int					power_count(e)
{
	int						count;

	count = 0;
	while (e >= 64)
	{
		e -= 64;
		count++;
	}
	if (e > 0)
		count++;
	return (count);
}

static void					add_to_string(int e, unsigned mantis_byte)
{	
	t_num_parts				**mant_exp;
	char					*mantis;
	int						count;
	int						i;
	int						j;

	i = 63;
	j = 0;
	count = 0;
	mantis = (char*)malloc(sizeof(char) * 65);
	while (i >= 0)
	{
		if (mantis_byte & 1)
		{
			mantis[i] = '1';
			count++;
		}
		else
			mantis[i] = '0';
		mantis_byte >>= 1;
		i--;
	}
	mantis[64] = '\0';
	i = 0;
	mant_exp = (t_num_parts**)malloc(sizeof(t_num_parts*) * (count + 1));
	mant_exp[count] = NULL;
	while (i < 63)
	{
		if (mantis[i] == '1')
			mant_exp[j++] = mantis_part_to_mult(e);
		e--;
		i++;
	}
	multi_parts_num(&mant_exp, j - 1);
}

static int					output_f_flags(const char *format, va_list args, t_list *l, char *type)
{
	t_uni_dub				ptr;
	long double				f;
	char					*mantis;
	char					*double_num;
	int						length;
	int						e;
	int						sign;
	int						i;

	l->precision = (!l->precision) ? 6 : l->precision;
	i = l->precision;
	if (!(*type) || *type == 'l')
		f = va_arg(args, double);
	else if (*type == 'L')
		f = va_arg(args, long double);
	ptr.val = f;
	sign = ptr.doub.sign;
	e = ptr.doub.exp - 16383;
	if (e >= 0)
	{
		add_to_string(e, ptr.doub.mantis);
			// return (-1);
	}
	// f = (sign == 1) ? -f : f;
 	// mantis = creat_mantis(f, l->precision);
	// mantis_rounding(l, &mantis);
	// double_num = creat_double_chr((long long)f, mantis, sign);
	// if (!double_num)
	// 	return (-1);
	// length = ft_strlen(double_num);
	// chr_output(l, double_num, length, format);
	// free(double_num);
	// return (1);
}

static int					ft_flag_function_find(const char *format, va_list args, t_list *l, char *type)
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

static int					unknow_output(const char *format, t_list *l)
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
		out = ft_strndup(format, l->save, l->save);
		if (!out)
			return (-1);
		length = ft_strlen(out);
		l->i = l->save;
		l->i++;
	}
	chr_output(l, out, length, format);
	return (1);
}

static char				*type_define(int lon, int shor)
{
	char			*type;

	if ((lon == -1 || shor == -1) || (lon == 2 && !shor))
		type = "ll";
	else if (lon == 1 && !shor)
		type = "l";
	else if(!lon && shor == 1)
		type = "h";
	else if (!lon && shor == 2)
		type = "hh";
	else if (lon == -2)
		type = "L";
	return (type);
}

static char				*type_parse(const char *format, t_list *l)
{
	int				lon;
	int				shor;
	
	lon = 0;
	shor = 0;
	while (ft_memchr("Llh", format[l->type], 3))
	{
		if (format[l->type] == 'l')
			lon = (!lon) ? 1 : lon + 1;
		else if (format[l->type] == 'h')
			shor = (!shor) ? 1 : shor + 1;
		else if (format[l->type] == 'h' && lon)
		{
			shor = -1;
			break ;
		}
		else if (format[l->type] == 'l' && shor)
		{
			lon = -1;
			break ;
		}
		else if (format[l->type] == 'L' && format[l->flag] == 'f')
		{
			lon = -2;
			break ;
		}
		l->type++;
	}
	return (type_define(lon, shor));
}

static int						length_check(const char *format, t_list *l, int sign)
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
	l->save = j - 1;
	l->i = l->save;
	return (length);
}

static void				flag_check(const char *format, t_list *l)
{
	if (format[l->save] == ' ' && (ft_memchr("dioxX", format[l->save + 1], 6)
		|| ft_isnum(format[l->i + 1], 10)))
		l->sp = 1;
	else if (format[l->save] == '#' && !l->fhash)
		l->fhash = l->save;
	else if (format[l->save] == '+' && !l->fplus)
		l->fplus = l->save;
	else if (format[l->save] == '0' && !l->fzero)
		l->fzero = l->save;
	else if (format[l->save] == '-' && !l->fminus)
		l->fminus = l->save;
	else if (ft_isnum(format[l->save], 0))
			l->length = length_check(format, l, l->fminus);
	else if (format[l->save] == '.')
	{
		l->dot = l->save;
		if ((ft_isnum(format[l->save + 1], 10)
			|| ft_memchr("+-", format[l->save + 1], 2)))
		{
			l->precision_minus = (format[l->save + 1] == '-') ? 1 : -1;
			(format[l->save + 1] == '-') ? l->save++ : l->save;
			l->precision = length_check(format, l, l->precision_minus);
		}
	}
}

static int					pars_format(const char *format, t_list *l)
{
	int					save;

	l->save = l->i;
	while (format[l->save] != '%' && format[l->save] != '\n' && format[l->save])
	{
		if (!ft_memchr("diouxXscpf+-_. Llh#", format[l->save], 19) && !ft_isnum(format[l->save], 112))
			return (0);
		if (ft_memchr("diouxXscpf", format[l->save], 10) && !(l->flag))
		{
			l->flag = l->save;
			return (1);
		}
		else if (ft_memchr("Llh", format[l->save], 3) && !(l->type))
			l->type = l->save;
		flag_check(format, l);
		l->save++;
	}
	l->i = l->save;
	return (1);
}

static int					specs_and_flags_fing(const char *format, va_list args, t_list *l)
{
	char			*type;
	int				res;

	res = pars_format(format, l);
	if (res && l->flag)
	{
		if (ft_memchr("Llh", format[l->type], 3))
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

static int					ft_variants(const char *format, va_list args, t_list *l)
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
	int	count1 = 1;

	count = ft_printf("%f\n", 1223498756823465892364875832476582734658763542353465.232834583764857235);
	// printf("%d\n", ft_strlen("00010010001101000101011001111000100101110010011001110011010001010010100101100011010101001001001000110101010000100011100001001000001001010011010001100101.001000110101"));
	// printf("{%f}\n", 12345678972673452963549235423848253465.235);
	return (0);
}

//Строки для теста
//"1%%2%3%4%5%%%%%70pmamkapvoya\n",  "aaasasdasc"
//"123#%45d%%%%%%%#-70pmamkapvoya\n",  "aaasasdasc"
//"123#%45d%%%%%%%#+0+70pmamkapvoya\n",  -11234567, "aaasasdasc"
//"%0134kwwww65ytcyxutrcvhbj.4+064f\n",  -11234567.123456789100
//"%123y70d-065pmamkapvoya%.3f\n",  -11234567, "aaasasdasc", 1234567.1234567890

//Если все идут hhhhhhhh то вывод будет на ll
//Если все идут llllllll то вывод будет на ll
//Если все идут hhhlhllhh и все в этом роде то вывод будет на ll