/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/11 22:56:09 by msabre            #+#    #+#             */
/*   Updated: 2019/10/26 18:53:14 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <float.h>
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

static unsigned long long		to_power(unsigned long long a, int power)
{
	if (power == 0)
		return (1);
	a = a * to_power(a, power - 1);
	return (a);
}

static void						zero_flags(t_list *l)
{
	l->precision_minus = 0;
	l->free_block = 0;
	l->start = -1;
	l->fhash = 0;
	l->sp = 0;
	l->dot = 0;
	l->type = 0;
	l->flag = 0;
	l->start = -1;
	l->fzero = 0;
	l->fminus = 0;
	l->fplus = 0;
	l->length = 0;
	l->precision = 0;
	l->cut_s = 0;
	l->free_block = 0;
	l->dop = -1;
	l->dop_count = 0;
	l->hash = NULL;
	l->out = NULL;
	l->spase = ' ';
}

static t_list					*struct_init(const char *format)
{
	t_list					*l;
	
	l = (t_list*)malloc(sizeof(t_list) * 1);
	if (!l)
		return (NULL);
	l->i = 0;
	l->count = 0;
	l->n_count = 0;
	l->format = format;
	l->buffer_for_write = (char*)malloc(sizeof(char));
	if (!(l->buffer_for_write))
		return (NULL);
	*(l->buffer_for_write) = '\0';
	zero_flags(l);
	return (l);
}

static char					*ft_strndup(const char *str, int start, int end)
{
	int				i;
	int				length;
	char			*ptr;

	i = 0;
	length = end - start;
	ptr = (char*)malloc(sizeof(char) * (length + 1));
	if (!ptr)
		return (NULL);
	while (start <= end && str[start] != '\0')
	{
		ptr[i] = str[start];
		i++;
		start++;
	}
	ptr[i] = '\0';
	return (ptr);
}

static char					*ft_str_rev(char **str)
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
	free(*str);
	return (str_reverse);
}

static char					*get_newbase_res(unsigned long long num_integer,
								int num_system, char *result, int alp_register)
{
	int						digit;
	int						index;

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

static char					*decimy_to_any(unsigned long long num_integer,
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
	(flag == 'b') ? num_system = 2 : num_system;
	(flag == 'x' || flag == 'X') ? num_system = 16 : num_system;
	(flag == 'o') ? num_system = 8 : num_system;
	return (num_system);
}

static int					get_buffer(t_list *l, char *new_str)
{
	char					*ptr;
	
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

static void					flag_config(t_list *l)
{
	if (!ft_memchr("Xxop", l->format[l->flag], 4))
		l->fhash = 0;
	if (l->precision < 0)
		l->length = 0;
	if (l->fzero && l->fminus)
		l->fzero = -1;
	if (l->fzero && l->dot > 0 && l->format[l->flag] != 'f')
		l->fzero = 0;
	if (l->fzero > 0)
		l->spase = '0';
	if (l->fplus > 0 && l->sp > 0)
		l->sp = 0;
	if (l->fplus && (!ft_memchr("dioxXf", l->format[l->flag], 6)
		|| *(l->out) == '-'))
		l->fplus = 0;
	if (mod_compair(l->out_length, l->precision) == 1
		|| ft_memchr("cs", l->format[l->flag], 2))
		l->precision = 0;
	if (mod_compair(l->out_length, l->length) == 1
		|| mod_compair(l->out_length, l->length) == 0)
		l->length = 0;
	if (mod_compair(l->precision, l->length) == 1)
		l->length = 0;
	if (l->sp > 0 && *(l->out) == '-')
		l->sp = 0;
}

static void					fhash_config(t_list * l)
{
	if (l->format[l->flag] == 'o')
		l->hash = "0";
	else
		l->hash = (l->format[l->flag] != 'X' ) ? "0x" : "0X";
	l->dop_count = (l->format[l->flag] == 'o') ? 1 : 2;
	if (l->spase == ' ' && l->length > 0 && l->length - l->out_length >= l->dop_count)
		l->dop = l->length - (l->precision > l->out_length ? l->precision : l->out_length) - l->dop_count;
	else
		l->dop = 0;
}

static int					get_count_spaces(t_list *l)
{
	int						count_space;

	count_space = 0;
	if (mod_compair(l->precision, l->length) == 1)
		count_space = mod_minus(l->precision, l->out_length);
	if (mod_compair(l->length, l->out_length) == 1)
	{
		count_space = mod_minus(l->length, l->out_length);
		count_space = (l->fplus >= 0) ? mod_minus(count_space, l->fplus) : count_space;
	}
	if (mod_compair(l->dop_count, count_space) == 1)
		count_space = 0;
	count_space = (l->dop >= 0 && mod_compair(count_space, l->dop_count) == 1 && l->length)
					? mod_minus(count_space, l->dop_count) : count_space;
	count_space *= (count_space < 0) ? -1 : 1;
	return (count_space);
}
static char					*flag_inicializatian(t_list *l)
{
	int						count_space;
	char					*result;

	flag_config(l);
	if (l->fhash)
		fhash_config(l);
	count_space = get_count_spaces(l);
	if (*(l->out) == '-' && l->precision > 0 && l->length == 0)
		count_space++;
	if (l->sp > 0 && l->precision >= l->length && l->length >= 0)
		count_space++;
	else if (l->sp > 0 && l->precision && l->length >= 0)
		l->sp = 0;
	if (!(result = ft_memalloc((l->out_length + count_space + l->dop_count + l->fplus))))
	{
		free(l->out);
		return (NULL);
	}
	(l->dop >= 0) ? result[l->dop] = '\0' : 1;
	return (result);
}

static int					add_min_or_plus(t_list *l, char **result)
{
	int						i;
	int						j;
	
	j = 0;
	i = (l->spase == ' ' && l->fplus > 0 && l->length > 0)
		? l->length - l->out_length - l->fplus - (l->precision > 0 ? l->precision - l->out_length : 0) : 0;
	if (*(l->out) == '-' && l->spase == '0')
	{
		(*result)[i++] = '-';
		l->length = mod_minus(l->length, 1);
		l->out_length--;
		j++;
	}
	else if (*(l->out) == '-' && l->spase == ' ' && (l->length > 0 || l->precision > 0))
	{
		if (l->length > 0)
			i = l->length - l->out_length -
				(l->precision > 0 ? l->precision - l->out_length : 0) - (l->precision > 0 ? 1 : 0);
		(*result)[i++] = '-';
		l->length = mod_minus(l->length, 1);
		l->out_length--;
		j++;
	}
	else
		(l->fplus > 0) ? (*result)[i++] = '+' : 0;
	return (j);
}

static int					add_len_to_out(t_list *l, char **result, int minus)
{
	int						i;
	int						length;
	
	i = (l->spase == ' ' ? 0 : minus) + (l->spase == ' ' && l->length > 0 ? 0 : l->fplus)
		+ l->sp + (l->precision > 0 && l->length < 0 ? l->precision - l->out_length : 0)
			+ ((l->length < 0) ? l->out_length : 0);
	length = (l->length * (l->length < 0 ? -1 : 1)) - l->out_length - l->dop_count - l->sp
			- l->fplus - (l->precision > 0 ? l->precision - l->out_length : 0);
	(l->dop >= 0 && (l->spase == '0' || l->fminus)) ? i += l->sp + l->dop_count : 1;
	while (length > 0)
	{
		(*result)[i++] = l->spase;
		length--;
	}
	return (i);
}

static int					add_precision(t_list *l, char **result, int minus)
{
	int						length;
	int						prec;
	int						i;

	prec = l->precision * (l->precision < 0 ? -1 : 1);
	i = (l->length <= 0 && l->precision > 0 ? l->fplus : 0) + l->sp + minus + l->dop_count
		+ (l->length > 0 && l->spase == '0' ? l->length - l->precision : 0) +
			(l->spase == ' ' && l->length > 0 ? l->length - l->precision - l->dop_count : 0);
	if (l->precision < 0)
		i += l->out_length;
	length = prec - l->out_length - (l->precision > 0
		&& l->precision > l->length && l->spase == '0' ? l->dop_count : 0);
	l->spase = (l->precision > 0) ? '0' : ' ';
	while (length-- > 0)
		(*result)[i++] = l->spase;
	return (i);
}

static int					position_after_outadd(t_list *l, int minus, int i)
{
	if (l->cut_s == 1 && l->format[l->flag] == 's' && l->length > 0)
		i = l->fplus + l->sp + (l->length - l->out_length);
	if (l->length <= 0 && l->precision == 0)
		i = minus + l->fplus + l->sp + (l->dop >= 0 ? l->dop + l->dop_count : 0);
	else
		i += (l->fplus && l->spase != '0' ? 1 : 0) +
			(l->dop >= 0 && l->spase != '0' ? l->dop_count : 0)
				+ (minus > 0 && l->length > 0 && l->spase != '0' ? minus : 0);
	if (ft_strcmp(l->out, "") == 0 && l->format[l->flag] == 'c')
	{
		l->out = "0";
		l->darwin_null[l->n_count++] = ft_strlen(l->buffer_for_write) + i;
		l->darwin_null[l->n_count] = -1;
	}
	return (i);
}

static int					fill_output(t_list *l, char *result)
{
	int						j;
	int						i;
	int						minus;

	minus = 0;
	j = 0;
	if (*(l->out) == '-' || l->fplus)
	{
		j = add_min_or_plus(l, &result);
		if (j > 0)
			minus = 1;
	}
	if (l->hash)
		ft_strcat(&(result[(l->dop >= 0 ? l->dop : 0)]), l->hash);
	if (l->length != 0)
		i = add_len_to_out(l, &result, minus);
	if (l->precision != 0)
		i = add_precision(l, &result, minus);
	i = position_after_outadd(l, minus, i);
	while (l->out[j])
		result[i++] = (l)->out[j++];
	return (!(get_buffer(l, result))) ? -1 : 1;
}

static int					chr_output(t_list *l)
{
	char				*result;

	if (!(result = flag_inicializatian(l)))
		return (-1);
	if (!(fill_output(l, result)))
		return (-1);
	(l->free_block == 0) ? free(l->out) : 1;
	return (1);
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

static int						output_di_flags(va_list args,
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
	l->out_length = ft_strlen(d_chr);
	(l->fplus > 0 && *d_chr == '-') ? l->fplus = 0 : 1;
	if (l->dot && l->precision == 0 && *d_chr == '0')
	{
		*d_chr = '\0';
		l->out_length = 0;
	}
	l->out = d_chr;
	return (chr_output(l));
}

static int						output_u_flags(va_list args,
									char *type, t_list *l)
{
	unsigned long long	u;
	char				*out;
	int					count;

	u = va_arg(args, unsigned long long);
	if (l->sp)
		l->sp = 0;
	if (!(*type))
		out = ft_itoa_usigned((unsigned int)u, 0, type[0]);
	else
		out = choose_length_chr(type, ft_itoa_usigned, u, 'u');
	if (!out)
		return (-1);
	l->out_length = ft_strlen(out);
	if (l->dot && l->precision == 0 && *out == '0')
	{
		*out = '\0';
		l->out_length = 0;
	}
	l->out = out;
	return (chr_output(l));
}

static void						zero_res_xo_flags(t_list *l)
{
	if (l->dot > 0 && l->precision == 0)
	{
		*(l->out) = '\0';
		l->out_length = 0;
		if (l->format[l->flag] == 'x')
			l->fhash = 0;
	}
	else
		l->fhash = 0;
}

static int						output_xo_flags(va_list args,
									t_list *l, char *type)
{
	unsigned long long	xo;

	xo = va_arg(args, unsigned long long);
	if (!(*type))
		l->out = decimy_to_any((unsigned int)xo, ft_num_sys(l->format[l->i]), l->format[l->i]);
	else
		l->out = choose_length_chr(type, decimy_to_any, xo, l->format[l->i]);
	if (!l->out)
		return (-1);
	l->out_length = ft_strlen(l->out);
	if (*(l->out) == '0')
		zero_res_xo_flags(l);
	else if (l->precision > l->out_length && l->format[l->flag] == 'o')
		l->fhash = 0;
	return (chr_output(l));
}

static int						output_p_flags(va_list args,
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
	l->out_length = ft_strlen(integer);
	l->fhash = 1;
	l->out = integer;
	if (l->dot > 0 && l->precision == 0 && *(l->out) == '0') 
	{
		l->out_length = 0;
		*(l->out) = '\0';
	}
	return (chr_output(l));
}

static void					s_flag_config(t_list *l, va_list args)
{
	l->out = va_arg(args, char*);
	l->out = (l->out == NULL) ? "(null)" : l->out;
	l->out_length = ft_strlen(l->out);
	if (l->dot && l->precision < l->out_length)
	{
		l->out = ft_strndup(l->out, 0, l->precision - 1);
		l->out_length = l->precision;
		l->cut_s = 1;
		l->free_block = 0;
	}
	else if (l->precision == 0 && l->dot != 0)
		l->out_length = 0;
}

static int					output_cs_flags(va_list args, t_list *l)
{
	char			c;

	l->free_block = 1;
	if (l->sp)
			l->sp = 0;
	if (l->format[l->i] == 's')
		s_flag_config(l, args);
	else
	{
		if (!(l->out = ft_memalloc(1)))
			return (-1);
		c = va_arg(args, int);
		*(l->out) = c;
		l->free_block = (c == 0) ? 1 : 0;
		l->out_length = 1;
	}
	if (ft_strcmp(l->out, "") == 0 && l->format[l->flag] != 'c')
	{
		if (l->length == 0)
			return (1);
		if (l->length != 0)
			l->precision = 0;	
	}
	chr_output(l);
	return (1);
}

static void					*free_doub_lvl_mass(void **mas)
{
	char					**ptr;
	int						i;

	i = 0;
	if (!mas)
		return (NULL);
	ptr = (char**)mas;
	while (ptr[i] != NULL)
		free(ptr[i++]);
	free(ptr);
	return (NULL);
}

static int					size_int_mass(int *a)
{
	int						i;

	i = 0;
	while (a[i] >= 0)
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
	while (i < size)
		result[i++] = 0;
	return (result);
}

static int					*add_ll_parts(int *a, int *b, int a_size, int b_size)
{
	int						*result;
	int						carry;
	int						count;
	int						i;

	i = 0;
	carry = 0;
	if (a_size > b_size)
		count = a_size;
	else
		count = b_size;
	if (!(result = creat_res(count + 2)))
		return (NULL);
	while (i < count || carry)
	{
		carry += (i < a_size ? a[i] : 0) + (i < b_size ? b[i] : 0);
		result[i] = carry % 10;
		carry /= 10;
		i++;
	}
	result[i] = -1;
	return (result);
}

static char					*str_fr_intmass(int	*a, int size, t_list *l, int sign)
{
	char					*str;
	int						i;

	i = 0;
	if (!a)
		return (NULL);
	size = (l->fhash && l->precision == 0) ? size++ : size;
	sign = (l->precision == 0) ? sign : 0;
	if (!(str = (char*)malloc(sizeof(char) * (size + sign + 1))))
		return (NULL);
	size -= (l->fhash && l->precision == 0) ? 2 : 1;
	if (sign == 1)
		str[i++] = '-';
	while (size >= 0)
		str[i++] = a[size--] + 48;
	if (l->fhash && l->precision == 0)
		str[i++] = '.';
	str[i] = '\0';
	return (str);
}

static char		 			*get_bn_str(int **result, t_list *l, int sign)
{
	int						*a;
	int						*b;
	char					*str;
	int						i;
	
	if (!(a = add_ll_parts(result[0], result[1], size_int_mass(result[0]),
		size_int_mass(result[1]))))
		return (free_doub_lvl_mass((void**)result));
	i = 2;
	while (result[i] != NULL)
	{
		b = add_ll_parts(a, result[i], size_int_mass(a), size_int_mass(result[i]));
		free(a);
		if (!b)
			return (free_doub_lvl_mass((void**)result));
		a = b;
		i++;
	}
	str = str_fr_intmass(a, size_int_mass(a), l, sign);
	free(a);
	if (!str)
		return (NULL);
	return (str);
}

static int					*long_to_long(int *a, int *b, int a_size, int b_size)
{
	unsigned long long		cur;
	unsigned long long		cr;
	int						*c;
	int						i;
	int						j;
	
	i = 0;
	if (!(c = creat_res(a_size + b_size + 1)))
		return (NULL);
	i = 0;
	while (i < a_size)
	{
		j = 0;
		cr = 0;
		while (j < b_size || cr)
		{
			cur = c[i + j] + a[i] * (j < b_size ? b[j] : 0) + cr;
			c[i + j] = cur % 10;
			cr = cur / 10;
			j++;
		}
		i++;
	}
	return (c);
}

static int					*long_multi(int *a, int *b, int a_size, int b_size)
{
	int 					*result;
	int						*c;
	int						i;
	int						j;

	i = 0;
	j = 0;
	if (!(result = creat_res(a_size + b_size + 1)))
		return (NULL);
	if (!(c = long_to_long(a, b, a_size, b_size)))
		return (NULL);
	i = a_size + b_size - 1;
	while (c[i] == 0)
		i--;
	while (j <= i)
	{
		result[j] = c[j];
		j++;
	}
	result[i + 1] = -1;
	free(c);
	return (result);
}

static int					**calculations_bn(t_num_parts ***num, int count, int **result, int j)
{
	int						*a;
	int						*b;
	int						i;

	while (count >= 0)
	{
		i = 0;
		if (!(a = long_multi(((*num)[count])->num_part[i], ((*num)[count])->num_part[i + 1],
			size_int_mass(((*num)[count])->num_part[i]), size_int_mass(((*num)[count])->num_part[i + 1]))))
				return (free_doub_lvl_mass((void**)result));
		i++;
		while (((*num)[count])->size > 2 && ((*num)[count])->num_part[i + 1] != NULL)
		{
			b = long_multi(a, ((*num)[count])->num_part[i + 1], size_int_mass(a),
				size_int_mass(((*num)[count])->num_part[i + 1]));
			free(a);
			if (!b)
				return (free_doub_lvl_mass((void**)result));
			a = b;
			i++;
		}
		result[j++] = a;
		count--;
	}
	return (result);
}

static int					**get_bignum(t_num_parts ***num, int count)
{
	int						**result;
	int						j;

	j = 0;
	if (!(result = (int**)malloc(sizeof(int*) * (count + 2))))
		return (NULL);
	result[count + 1] = NULL;
	if (count == 0)
	{
		*result = ((*num)[count])->num_part[0];
		return (result);
	}
	if (!(result = calculations_bn(num, count, result, j)))
		return (NULL);
	return (result);
}

static int					*by_rank(unsigned long long int num)
{
	int					*result;
	int					i;	

	i = 0;
	if (!(result = (int*)malloc(sizeof(int) * (integer_size(num) + 1))))
		return (NULL);
	if (num == 18446744073709551615UL)
	{
		result[i++] = 6;
		num /= 10;
	}
	while (num > 0)
	{
		result[i++] = num % 10;
		num /= 10;
	}
	result[i] = -1;
	return(result);
}

static t_num_parts			*creat_num_part(int	e)
{
	t_num_parts				*ptr;
	int						count;

	count = 0;
	if (e / 64 <= 0)
		count = 2;
	else if (e / 64 > 0)
		count = e / 64 + 1;
	ptr = (t_num_parts*)malloc(sizeof(t_num_parts));
	ptr->num_part = (int**)malloc(sizeof(int*) * (count + 1));
	if (!(ptr) || !(ptr->num_part))
		return (NULL);
	return (ptr);
}

static t_num_parts			*mantis_part_to_mult(int e)
{
	t_num_parts				*ptr;
	int						i;

	i = 0;
	if (!(ptr = creat_num_part(e)))
		return (NULL);
	while (e > 0)
	{
		ptr->num_part[i++] = (e >= 64)
			? by_rank(18446744073709551615UL) : by_rank(to_power(2, e));
		e -= (e >= 64) ? 64 : e;
		if (!ptr->num_part[i - 1])
		{
			free_doub_lvl_mass((void**)(ptr->num_part));
			free(ptr);
			return (NULL);
		}
	}
	ptr->size = i;
	ptr->num_part[i] = NULL;
	return (ptr);
}

static int					get_binary(char **src, unsigned long a)
{
	int						i;
	int						count;

	i = 63;
	(*src)[0] = '1';
	count = 1;
	while (i > 0)
	{
		if (a & 1)
		{
			(*src)[i] = '1';
			count++;
		}
		else
			(*src)[i] = '0';
		a >>= 1;
		i--;
	}
	(*src)[63] = '\0';
	return (count);
}

static char					*norm_chr_ll(long double f, t_list *l, int sign)
{
	char					*str;
	double long				num;
	int						i;

	i = 0;
	num = (unsigned long long int)f;
	if (num == 0 && sign == 1 && l->precision  == 0)
	{
		str = ft_memalloc(2);
		str[i++] = '-';
		str[i] = '0';
		return (str);
	}
	if (l->precision == 0)
	{
		f -= (long long)f;
		f *= 10;
		num = (f >= 5) ? num + 1 : num;
		num = (sign == 1) ? -num : num;
		if (l->fhash > 0 && l->precision == 0 && l->dot)
			num *= 10;
	}
	str = ft_itoa(num);
	(l->fhash && l->precision == 0) ? str[ft_strlen(str) - 1] = '.' : 1;
	return (str);
}

static void					free_struct(t_num_parts **mant_exp)
{
	int						i;
	int						j;

	j = 0;
	i = 0;
	while (mant_exp[i] != NULL)
	{
		free_doub_lvl_mass((void*)(mant_exp[i])->num_part);
		free(mant_exp[i]);
		i++;
	}
	free(mant_exp);
}

static t_num_parts			**get_and_fill_numparts(t_num_parts **mant_exp, char *mantis, int e)
{
	int						i;
	int						j;

	j = 0;
	i = 0;
	while (i <= 63)
	{
		if (mantis[i] == '1')
		{
			if (!(mant_exp[j++] = mantis_part_to_mult(e)))
			{
				free(mantis);
				return (NULL);
			}
		}
		e--;
		i++;
	}
	free(mantis);
	return (mant_exp);
}

static char					*add_to_string(t_uni_dub *ptr, long double f, t_list *l)
{	
	t_num_parts				**mant_exp;
	char					*mantis;
	int						**result;
	int						count;

	if (!(mantis = (char*)malloc(sizeof(char) * 65)))
		return (NULL);
	count = get_binary(&mantis, ptr->doub.mantis);
	if (!(mant_exp = (t_num_parts**)malloc(sizeof(t_num_parts*) * (count + 1))))
	{
		free(mantis);
		return (NULL);
	}
	if (!(mant_exp = get_and_fill_numparts(mant_exp, mantis, ptr->doub.exp - 16383)))
		return (NULL);
	mant_exp[count] = NULL;
	(count == 64) ? count-- : 1;
	result = get_bignum(&mant_exp, count - 1);
	free_struct(mant_exp);
	if (count == 1)
		mantis = str_fr_intmass(*result, size_int_mass(*result), l, ptr->doub.sign);
	else
		mantis = get_bn_str(result, l, ptr->doub.sign);
	free_doub_lvl_mass((void**)result);
	return (!mantis ? NULL : mantis);
}

static char				*creat_double_chr(char *chr_order, char *mantis, int sign)
{
	char			*double_chr;
	int				l_order;
	int				l_mantis;
	int				i;

	i = 0;
	l_order = ft_strlen(chr_order);
	l_mantis = ft_strlen(mantis);
	if (!(double_chr = (char*)malloc(sizeof(char) * (l_order + l_mantis + sign + 2))))
		return (NULL);
	if (sign == 1)
	{
		double_chr[0] = '-';
		double_chr[++i] = '\0';
		l_order++;
	}
	double_chr[i] = '\0';
	ft_strcat(double_chr, chr_order);
	double_chr[l_order] = '.';
	double_chr[l_order + 1] = '\0';
	ft_strlcat(double_chr, mantis, l_mantis + l_order + 1);
	free(chr_order);
	free(mantis);
	return (double_chr);
}

static void					after_dot_rounding(t_list *l, char **fractional)
{
	double long				up;
	int						i;
	
	// up = ((*fractional)[l->precision] % 2 == 0)
	// 	? (*fractional)[l->precision] += 1 : 0;
	up = ((*fractional)[l->precision] >= 53) ? 1 : 0;
	i = l->precision - 1;
	while (up > 0)
	{
		if ((*fractional)[i] >= 48 && (*fractional)[i] < 57)
		{
			(*fractional)[i] += 1;
			up = 0;
		}
		else if ((*fractional)[i] == 57)
		{
			(*fractional)[i] = 48;
			up++;
		}
		i--;
	}
}

static char					*creat_after_dot(long double f, int precision, t_list *l, int e)
{
	char					*fractional;
	char					ptr;
	int						i;

	i = 0;
	f -= (long long)f;
	if (!(fractional = (char*)malloc(sizeof(char) * (precision + 2))))
		return (NULL);
	if (e < 64)
	{
		while (i <= precision)
		{
			f *= 10;
			ptr = (int)f + 48;
			fractional[i++] = ptr;
			f -= (long long)f;
		}
	}
	if (e >= 64)
		while (i <= precision)
			fractional[i++] = '0';
	fractional[i] = '\0';
	after_dot_rounding(l, &fractional);
	return (fractional);
}

static int					check_inf_or_nan(long double f, t_list *l, t_uni_dub *ptr, char *type)
{
	if (f != f)
	{
		l->sp = 0;
		l->fplus = 0;
		l->out = "nan";
	}
	else if (ptr->doub.exp == 32767 && ptr->doub.mantis == 9223372036854775808UL)
	{
		if (ptr->doub.sign == 1)
			l->out = "-inf";
		else
			l->out = "inf";
	}
	if (l->out != NULL)
	{
		l->fzero = 0;
		l->free_block = 1;
		l->out_length = ft_strlen(l->out);
		l->precision = 0;
	}
	return (l->out != NULL ? chr_output(l) : -1);
}

static int					output_f_flags(va_list args, t_list *l, char *type)
{
	t_uni_dub				ptr;
	long double				f;
	char					*order;
	char					*fractional;

	(!l->dot) ? l->precision = 6 : 1;
	(!(*type) || *type == 'l') ? f = va_arg(args, double) : 1;
	(*type == 'L') ? f = va_arg(args, long double) : 1;
	ptr.val = f;
	f = (ptr.doub.sign == 1) ? -f : f;
	if (check_inf_or_nan(f, l, &ptr, type) > 0)
		return (1);
	if (ptr.doub.exp - 16383 < 64)
		order = norm_chr_ll(f, l, (int) ptr.doub.sign);
	else if (!(order = add_to_string(&ptr, f, l)))
		return (-1);
	if (l->precision > 0)
	{
		fractional = creat_after_dot(f, l->precision, l, ptr.doub.exp - 16383);
		l->out = creat_double_chr(order, fractional, ptr.doub.sign);
	}
	else
		l->out = order;
	l->out_length = ft_strlen(l->out);
	return (!l->out ? -1 : chr_output(l));
}

static int					output_b_flags(va_list args, t_list *l, char *type)
{
	long long				f;
	char					*result;

	f = va_arg(args, long long);
	if (!(*type))
		result = decimy_to_any((int)f, 2, 'b');
	else
		result = choose_length_chr(type, decimy_to_any, f, 'b');
	l->out = result;
	l->out_length = ft_strlen(result);
	chr_output(l);
	return (1);
}

static int					ft_flag_function_find(va_list args, t_list *l, char *type)
{
	int 			i;
	int				res;

	i = 0;
	res = 0;
	if (ft_memchr("di", l->format[l->i], 2))
		res = output_di_flags(args, type, l);
	else if (l->format[l->i] == 'u')
		res = output_u_flags(args, type, l);
	else if (ft_memchr("xXo", l->format[l->i], 3))
		res = output_xo_flags(args, l, type);
	else if (ft_memchr("cs", l->format[l->i], 2))
		res = output_cs_flags(args, l);
	else if (l->format[l->i] == 'p')
		res = output_p_flags(args, l, type);
	else if (l->format[l->i] == 'f')
		res = output_f_flags(args, l, type);
	else if (l->format[l->i] == 'b')
		res = output_b_flags(args, l, type);
	l->i++;
	return (res);
}

static int					unknow_output(t_list *l)
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
		out = ft_strndup(l->format, l->save, l->save);
		if (!out)
			return (-1);
		l->out_length = ft_strlen(out);
		l->i = l->save;
		l->i++;
	}
	l->out = out;
	chr_output(l);
	return (1);
}

static char				*type_define(int lon, int shor, char flag)
{
	char			*type;

	if ((lon && shor) || lon > 2 || shor > 2 
		|| lon == -1 || shor == -1 || (lon == 2 && !shor))
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

static char				*type_parse(t_list *l, int lon, int shor)
{
	while (ft_memchr("Llh", l->format[l->type], 3))
	{
		if (l->format[l->type] == 'l')
			lon = (!lon) ? 1 : lon + 1;
		else if (l->format[l->type] == 'h')
			shor = (!shor) ? 1 : shor + 1;
		else if (l->format[l->type] == 'h' && lon)
		{
			shor = -1;
			break ;
		}
		else if (l->format[l->type] == 'l' && shor)
		{
			lon = -1;
			break ;
		}
		else if (l->format[l->type] == 'L' && l->format[l->flag] == 'f')
		{
			lon = -2;
			break ;
		}
		l->type++;
	}
	return (type_define(lon, shor, l->flag));
}

static int						length_check(t_list *l, int sign)
{
	char				length_output[21];
	int					length;
	int					i;
	int					j;

	i = 0;
	j = l->save;
	length = 0;
	while (!(l->format[j] >= 48 && l->format[j] <= 57))
		j++;
	if (sign > 0)
		length_output[i++] = '-';
	while (l->format[j] >= 48 && l->format[j] <= 57)
		length_output[i++] = l->format[j++];
	length_output[i] = '\0';
	i ? length = ft_atoi(length_output) : length;
	l->save = j - 1;
	l->i = l->save;
	return (length);
}

static void				flag_check(t_list *l)
{
	if (l->format[l->save] == ' ')
		l->sp++;
	else if (l->format[l->save] == '#' && !l->fhash)
		l->fhash = l->save;
	else if (l->format[l->save] == '+' && l->fplus == 0)
		l->fplus = 1;
	else if (l->format[l->save] == '0' && !l->fzero)
		l->fzero = l->save;
	else if (l->format[l->save] == '-' && !l->fminus)
		l->fminus = l->save;
	else if (ft_isnum(l->format[l->save], 0))
			l->length = length_check(l, l->fminus);
	else if (l->format[l->save] == '.')
	{
		l->dot = l->save;
		if ((ft_isnum(l->format[l->save + 1], 10)
			|| ft_memchr("+-", l->format[l->save + 1], 2)))
		{
			l->precision_minus = (l->format[l->save + 1] == '-') ? 1 : -1;
			(l->format[l->save + 1] == '-') ? l->save++ : l->save;
			l->precision = length_check(l, l->precision_minus);
		}
	}
}

static int					pars_format(t_list *l)
{
	int					save;

	l->save = l->i;
	while (l->format[l->save] != '%' && l->format[l->save] != '\n' && l->format[l->save])
	{
		if (!ft_memchr("diouxXscpf+-_. Llh#br", l->format[l->save], 21)
			&& !ft_isnum(l->format[l->save], 112))
			return (0);
		if (ft_memchr("diouxXscpfbr", l->format[l->save], 12) && !(l->flag))
		{
			l->flag = l->save;
			return (1);
		}
		else if (ft_memchr("Llh", l->format[l->save], 3) && !(l->type))
			l->type = l->save;
		flag_check(l);
		l->save++;
	}
	l->i = l->save;
	return (1);
}

static int					specs_and_flags_fing(va_list args, t_list *l)
{
	char			*type;
	int				res;

	res = pars_format(l);
	if (res && l->flag)
	{
		if (ft_memchr("Llh", l->format[l->type], 3)
			&& !ft_memchr("csp", l->format[l->flag], 3))
			type = type_parse(l, 0, 0);
		else
			type = "";
		l->i = l->flag;
		return (ft_flag_function_find(args, l, type));
	}
	else
		return (unknow_output(l));
	return (-1);
}

static int					ft_variants(va_list args, t_list *l)
{
	while (ft_memchr("% ", l->format[l->i], 2) && l->format[l->i])
	{
		if (l->format[l->i] == ' ')
			l->sp = 1;
		if (l->format[l->i] == '%' && l->start < 0)
			l->start = l->i;
		else if (l->format[l->i] == '%' && l->start >= 0)
		{
			get_buffer(l, "%");
			l->start = -1;
			if (l->format[l->i + 1] == ' ')
			{
				l->i++;
				break ;
			}
		}
		l->i++;
	}
	if (l->start >= 0 && l->format[l->i])
		return (specs_and_flags_fing(args, l));
	return (1);
}

static int			add_anytext_tobuff(t_list *l)
{
	char			*simple_text;
	int				count;
	int				i;
	int				j;
	
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

static void			dawrin_nulls(t_list *l)
{
	int				i;
	int				j;

	i = 0;
	j = 0;
	while (l->darwin_null[i] >= 0)
	{
		j = l->darwin_null[i];
		l->buffer_for_write[j] = '\0';
		i++;
	}
}

static int			print_output(int length, t_list *l)
{
	length = (l->i == length) ? ft_strlen(l->buffer_for_write) : 0;
	(l->n_count > 0) ? dawrin_nulls(l) : 1;
	(length > 0) ? write(1, l->buffer_for_write, length) : 1;
	free(l->buffer_for_write);
	free(l);
	return (length);
}

int					ft_printf(const char *format, ...)
{
	va_list			args;
	t_list			*l;
	int				length;

	length = ft_strlen(format);
	va_start(args, format);
	if (!(l = struct_init(format)))
		return (-1);
	while (l->i < length)
	{
		if (format[l->i] == '%')
		{
			if (!(ft_variants(args, l)))
				break ;
			zero_flags(l);
		}
		else
			if (!(add_anytext_tobuff(l)))
				break ;
	}
	length = print_output(length, l);
	va_end(args);
	return (length);
}

// int					main(int argc, char **argv)
// {
// 	int				count;
// 	int				count1;
// 	char			*c;
// 	double f = 123124.0;
// 	f = f + (f / 0.0);

// 	c = "!!!!!!!!";
// 	count1 = ft_printf("%llx", 212123123123123);
// 	printf("\n");
// 	count = printf("%llx", 212123123123123);
// 	printf("\n");

// 	printf("%d\n", count);
// 	printf("%d", count1);
// 	return (0);
// }

//Строки для теста
//1844674483947593847598347957384759834387465872348795602837645876324875683624575987394579837459873947598347598379485798374598374985793874598739457938745983749857398475938745987394857983759374507.8736583687468934685763487658346534347686847864784687460
//"1%%2%3%4%5%%%%%70pmamkapvoya\n",  "aaasasdasc"
//"123#%45d%%%%%%%#-70pmamkapvoya\n",  "aaasasdasc"
//"123#%45d%%%%%%%#+0+70pmamkapvoya\n",  -11234567, "aaasasdasc"
//"%0134kwwww65ytcyxutrcvhbj.4+064f\n",  -11234567.123456789100
//"%123y70d-065pmamkapvoya%.3f\n",  -11234567, "aaasasdasc", 1234567.1234567890

//Если все идут hhhhhhhh то вывод будет на ll
//Если все идут llllllll то вывод будет на ll
//Если все идут hhhlhllhh и все в этом роде то вывод будет на ll