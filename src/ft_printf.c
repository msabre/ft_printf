/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/11 22:56:09 by msabre            #+#    #+#             */
/*   Updated: 2019/10/17 20:16:50 by msabre           ###   ########.fr       */
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
	l->cut_s = 0;
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

static int					get_buffer(t_list *l, char *new_str)
{
	char					*ptr;
	
	if (!new_str)
		return (1);
	if (!(ptr = l->buffer_for_write))
		ptr = "";
	l->buffer_for_write = ft_strjoin(ptr, new_str);
	// (ft_strcmp(new_str, "%") != 0) ? free(new_str) : 1;
	if (!(l->buffer_for_write))
		return (-1);
	return (1);
}

static int					fill_output(t_list *l, char *result)
{
	int						j;
	int						i;
	int						minus;
	int						length;
	int						prec;

	i = (l->spase == ' ' && l->fplus > 0 && l->length > 0) ? l->length - l->out_length - 1 : 0;
	j = 0;
	minus = 0;
	if (*(l->out) == '-' && (l->spase == '0' || l->precision > 0))
	{
		result[i++] = '-';
		j++;
		minus++;
		l->length--;
		l->out_length--;
	}
	(l->fplus > 0) ? result[i++] = '+' : 0;
	(l->hash && *(l->out) != '0') ? ft_strcat(&(result[(l->dop > 0 ? l->dop : 0)]), l->hash) : 0;
	i = minus + (l->spase == ' ' && (l->length > 0 || l->fzero) ? 0 : l->fplus) + l->sp + ((l->length < 0) ? l->out_length : 0);
	if (l->length != 0)
		length = (l->length * (l->length < 0 ? -1 : 1)) - l->out_length - l->dop_count - l->sp - (l->fplus && l->spase != '0' ? 1 : 0);
	else
		length = 0;
	(l->dop >= 0 && (l->spase == '0' || l->fminus)) ? i += l->sp + l->dop_count : 1;
	while (length > 0)
	{
		result[i++] = l->spase;
		length--;
	}
	if (l->precision != 0)
	{
		l->spase = (l->precision > 0) ? '0' : ' ';
		prec = l->precision * (l->precision < 0 ? -1 : 1);
		i = (l->spase == '0' && l->length <= 0 ? l->fplus : 0) + l->sp + minus + l->dop_count + ((l->length > 0) ? l->length - prec : 0);
		if (l->precision < 0)
			i += l->out_length;
		length = prec - l->out_length + l->fplus;
		while (length-- > 0)
			result[i++] = l->spase;
	}
	if (l->cut_s == 1 && l->format[l->flag] == 's' && l->length > 0)
		i = l->fplus + l->sp + (l->length - l->out_length);
	if (l->length <= 0 && l->precision == 0)
		i = minus + l->fplus + l->sp + ((l->dop >= 0 && *(l->out) != 48) ? l->dop + l->dop_count : 0);
	else
		i += l->sp + ((l->dop >= 0 && l->spase != '0') ? l->dop_count : 0);
	if (l->dop != -2)
		while (l->out[j])
			result[i++] = (l)->out[j++];
	else
		result[l->length] = '\0';
	l->count += ft_strlen(result);
	get_buffer(l, result);
	return (1);
}

static char					*flag_inicializatian(t_list *l)
{
	int						count_space;
	char					*result;

	count_space = 0;
	if (l->precision < 0)
		l->length = 0;
	if (l->fzero && l->fminus)
		l->fzero = -1;
	if (l->fzero && l->dot > 0)
		l->fzero = 0;
	if (l->fzero > 0)
		l->spase = '0';
	if (!(l->fplus && ft_memchr("dioxX", l->format[l->flag], 5)))
		l->fplus = 0;
	if (l->fplus > 0 && l->sp < 1)
		l->out_length++;
	if (mod_compair(l->precision, l->length) == 1 )
		l->length = 0;
	if (mod_compair(l->out_length, l->precision) == 1)
		l->precision = 0;
	if (l->length > l->out_length)
		l->sp = 0;
	if (l->fhash && (*(l->out) != '0' || l->format[l->flag] == 'o'))
	{
		if (l->format[l->flag] == 'o')
			l->hash = "0";
		else
			l->hash = (l->format[l->flag] != 'X' ) ? "0x" : "0X";
		l->dop_count = (l->format[l->flag] == 'o') ? 1 : 2;
		if (l->spase == ' ' && l->length > 0 )
			l->dop = l->length - l->out_length - l->dop_count;
		else
			l->dop = 0;
	}
	else if (l->precision == 0 && l->dot && *(l->out) == '0')
		l->dop = -2;
	if (mod_compair(l->precision, l->length) == 1)
		count_space = mod_minus(l->precision, l->out_length);
	else if (mod_compair(l->length, l->out_length) == 1)
	{
		count_space = mod_minus(l->length, l->out_length);
		count_space = (l->dop >= 0) ? mod_minus(count_space, l->dop_count) : count_space;
	}
	count_space *= (count_space < 0) ? -1 : 1;
	if (l->sp && l->fplus <= 0 && *(l->out) != '-')
		l->out_length++;
	else
		l->sp = 0;
	if (!(result = ft_memalloc((l->out_length + count_space + l->dop_count))))
		return (NULL);
	(l->dop >= 0) ? result[l->dop] = '\0' : 1;
	return (result);
}

static int					chr_output(t_list *l)
{
	char				*result;
	int					count_summ;

	if (!(result = flag_inicializatian(l)))
		return (-1);
	fill_output(l, result);
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
	l->out = d_chr;
	chr_output(l);
	return (1);
}

static int						output_u_flags(va_list args,
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
	l->out_length = ft_strlen(out);
	l->out = out;
	chr_output(l);
	return (1);
}

static int						output_xo_flags(va_list args,
									t_list *l, char *type)
{
	unsigned long long	xo;
	char				*output;
	int					count;
	int					num_system;
	
	xo = va_arg(args, unsigned long long);
	if (!(*type))
		output = decimy_to_any((unsigned int)xo, ft_num_sys(l->format[l->i]), l->format[l->i]);
	else
		output = choose_length_chr(type, decimy_to_any, xo, l->format[l->i]);
	if (!output)
		return (-1);
	l->out_length = ft_strlen(output);
	l->out = output;
	if (ft_strcmp(l->out, "0") == 0 && l->fhash > 0 && l->precision == 0)
		l->out_length = 0;
	chr_output(l);
	if (*output)
		free(output);
	return (1);
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
	chr_output(l);
	return (1);
}

static int					output_cs_flags(va_list args, t_list *l)
{
	char			*str;
	char			c;

	l->out_length = 1;
	if (l->format[l->i] == 's')
	{
		str = va_arg(args, char*);
		if (str == NULL)
			str = "(null)";
		l->out_length = ft_strlen(str);
		if (l->dot && l->precision < l->out_length)
		{
			str = ft_strndup(str, 0, l->precision - 1);
			l->out_length -= l->precision;
			l->cut_s = 1;
		}
		else if (l->precision == 0 && l->dot != 0)
			l->out_length = 0;
	}
	else
	{
		if (!(str = (char*)malloc(sizeof(char))))
			return (-1);
		c = va_arg(args, int);
		*str = c;
	}
	l->out = str;
	if (ft_strcmp(l->out, "") == 0 && l->format[l->flag] == 'c')
	{
		if (!(get_buffer(l, "0")))
			return (-1);
		l->darwin_null[l->n_count++] = ft_strlen(l->buffer_for_write) - 1;
		l->darwin_null[l->n_count] = -1;
		return (1);
	}
	if (ft_strcmp(l->out, "") == 0)
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
	return ("gg");
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

static char					*str_fr_intmass(int	*a, int size)
{
	char					*str;
	int						i;

	i = 0;
	if (!a)
		return (NULL);
	if (!(str = (char*)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	size -= 1;
	while (size >= 0)
		str[i++] = a[size--] + 48;
	str[i] = '\0';
	return (str);
}

static char		 			*get_bn_str(int **result)
{
	int						*a;
	int						*b;
	char					*str;
	int						i;
	
	if (!result)
		return (NULL);
	if (!(a = add_ll_parts(result[0], result[1], size_int_mass(result[0]),
		size_int_mass(result[1]))))
	{
		free_doub_lvl_mass((void**)result);
		return (NULL);
	}
	i = 2;
	while (result[i] != NULL)
	{
		b = add_ll_parts(a, result[i], size_int_mass(a), size_int_mass(result[i]));
		free(a);
		if (!b)
		{
			free_doub_lvl_mass((void**)result);
			return (NULL);
		}
		a = b;
		i++;
	}
	str = str_fr_intmass(a, size_int_mass(a));
	free(a);
	if (!str)
		return (NULL);
	return (str);
}

static int					*long_multi(int *a, int *b, int a_size, int b_size)
{
	unsigned long long		cur;
	unsigned long long		cr;
	int 					*result;
	int						c[1024];
	int						i;
	int						j;

	i = 0;
	while (i < (a_size + b_size + 1))
		c[i++] = 0;
	if (!(result = creat_res(a_size + b_size + 1)))
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
	i = a_size + b_size - 1;
	j = 0;
	while (c[i] == 0)
		i--;
	while (j <= i)
	{
		result[j] = c[j];
		j++;
	}
	result[i + 1] = -1;
	return (result);
}

static int					**get_bignum(t_num_parts ***num, int count)
{
	int						**result;
	int						*a;
	int						*b;
	int						i;
	int						j;

	if (!(result = (int**)malloc(sizeof(int*) * (count + 2))))
		return (NULL);
	result[count + 1] = NULL;
	j = 0;
	while (count >= 0)
	{
		i = 0;
		a = long_multi(((*num)[count])->num_part[i], ((*num)[count])->num_part[i + 1],
			size_int_mass(((*num)[count])->num_part[i]), size_int_mass(((*num)[count])->num_part[i + 1]));
		if (!a)
		{
			free_doub_lvl_mass((void**)result);
			return (NULL);
		}
		i++;
		while (((*num)[count])->size > 2 && ((*num)[count])->num_part[i + 1] != NULL)
		{
			b = long_multi(a, ((*num)[count])->num_part[i + 1], size_int_mass(a),
				size_int_mass(((*num)[count])->num_part[i + 1]));
			free(a);
			if (!b)
			{
				free_doub_lvl_mass((void**)result);
				return (NULL);
			}
			a = b;
			i++;
		}
		result[j++] = a;
		count--;
	}
	return (result);
}

static int					*by_rank(unsigned long long int num)
{
	int					*result;
	int					i;	

	i = 0;
	if (!(result = (int*)malloc(sizeof(int) * (integer_size(num) + 1))))
		return (NULL);
	if (num == 18446744073709551615)
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

static t_num_parts			*mantis_part_to_mult(int e)
{
	t_num_parts				*ptr;
	int						count;
	int						i;

	i = 0;
	if (e / 64 <= 0)
		count = 2;
	else if (e / 64 > 0)
		count = e / 64 + 1;
	ptr = (t_num_parts*)malloc(sizeof(t_num_parts));
	ptr->num_part = (int**)malloc(sizeof(int*) * (count + 1));
	if (!(ptr) || !(ptr->num_part))
		return (NULL);
	while (e > 0)
	{
		if (e >= 64)
		{
			ptr->num_part[i++] = by_rank(18446744073709551615L);
			e -= 64;
		}
		else
		{
			ptr->num_part[i++] = by_rank(to_power(2, e));
			e = 0;
		}
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

static char					*norm_chr_ll(long double f)
{
	char					*str;
	unsigned long long int	num;
	int						i;

	num = (unsigned long long int)f;
	str = (char*)malloc(sizeof(char) * integer_size(num));
	if (!str)
		return (NULL);
	str = ft_itoa(num);
	return (str);
}

static void					free_struct(t_num_parts **mant_exp)
{
	int						i;

	i = 0;
	while (mant_exp[i] != NULL)
		free((mant_exp[i++])->num_part);
	free(mant_exp);
}

static char					*add_to_string(int e, unsigned long mantis_byte, long double f)
{	
	t_num_parts				**mant_exp;
	char					*mantis;
	int						**result;
	int						count;
	int						i;
	int						j;

	j = 0;
	i = 0;
	if (e < 64)
		return (norm_chr_ll(f));
	if (!(mantis = (char*)malloc(sizeof(char) * 65)))
		return (NULL);
	count = get_binary(&mantis, mantis_byte);
	if (!(mant_exp = (t_num_parts**)malloc(sizeof(t_num_parts*) * (count + 1))))
	{
		free(mantis);
		return (NULL);
	}
	mant_exp[count] = NULL;
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
	result = get_bignum(&mant_exp, count - 1);
	free_struct(mant_exp);
	if (result[1] == NULL)
		mantis = str_fr_intmass(*result, size_int_mass(*result));
	else
		mantis = get_bn_str(result);
	free_doub_lvl_mass((void**)result);
	return ((!mantis) ? NULL : mantis);
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
	if (!(double_chr = (char*)malloc(sizeof(char) * (l_order + l_mantis + sign + 1))))
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
	ft_strcat(double_chr, mantis);
	return (double_chr);
}

static void					after_dot_rounding(t_list *l, char **fractional)
{
	int						i;
	int						up;

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
	(*fractional)[l->precision] = '\0';
}

static char					*creat_after_dot(long double f, int precision, t_list *l, int e)
{
	char					*fractional;
	char					*ptr;
	double					f_ptr;
	int						i;

	i = 0;
	f -= (long long)f;
	if (!(fractional = (char*)malloc(sizeof(char) * (precision + 1))))
		return (NULL);
	while (i <= precision && e < 64)
	{
		f *= 10;
		ptr = ft_itoa((long long)f);
		fractional[i++] = *ptr;
		free(ptr);
		f -= (long long)f;
	}
	while (i <= precision && e >= 64)
		fractional[i++] = '0';
	fractional[i] = '\0';
	after_dot_rounding(l, &fractional);
	return (fractional);
}

static int					output_f_flags(va_list args, t_list *l, char *type)
{
	t_uni_dub				ptr;
	long double				f;
	char					*order;
	char					*fractional;
	char					*double_num;

	l->precision = (!l->precision) ? 6 : l->precision;
	if (!(*type) || *type == 'l')
		f = va_arg(args, double);
	else if (*type == 'L')
		f = va_arg(args, long double);
	ptr.val = f;
	f = (ptr.doub.sign == 1) ? -f : f;
	if (!(order = add_to_string(ptr.doub.exp - 16383, ptr.doub.mantis, f)))
		return (-1);
	fractional = creat_after_dot(f, l->precision, l, ptr.doub.exp - 16383);
	double_num = creat_double_chr(order, fractional, ptr.doub.sign);
	free(fractional);
	free(order);
	if (!double_num)
		return (-1);
	l->out_length = ft_strlen(double_num);
	l->out = double_num;
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

	if ((lon && shor) || lon > 2 || shor > 2)
		type = "ll";
	else if ((lon == -1 || shor == -1) || (lon == 2 && !shor))
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

static char				*type_parse(t_list *l)
{
	int				lon;
	int				shor;
	
	lon = 0;
	shor = 0;
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
	char				*length_output;
	int					length;
	int					i;
	int					j;

	i = 0;
	j = l->save;
	length = 0;
	while (!(l->format[j] >= 48 && l->format[j] <= 57))
		j++;
	if (!(length_output = (char*)malloc(sizeof(char) * ft_strlen(l->format))))
		return (-1);
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
	if (l->format[l->save] == ' ' && (ft_memchr("dioxX", l->format[l->save + 1], 6)
		|| ft_isnum(l->format[l->i + 1], 10)))
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
		if (!ft_memchr("diouxXscpf+-_. Llh#", l->format[l->save], 19) && !ft_isnum(l->format[l->save], 112))
			return (0);
		if (ft_memchr("diouxXscpf", l->format[l->save], 10) && !(l->flag))
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
		if (ft_memchr("Llh", l->format[l->type], 3))
			type = type_parse(l);
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
	int				save_i;

	save_i = l->i;
	while (l->format[l->i] == '%' && l->format[l->i])
	{
		if (l->format[l->i] == '%' && !(l->start))
			l->start = 1;
		else if (l->format[l->i] == '%' && l->start)
		{
			if (l->format[l->i] == '%' && l->format[l->i - 1] == '%')
			{
				get_buffer(l, "%");
				l->count++;
			}
			l->start = 0;
		}
		l->i++;
	}
	if (l->start)
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
	{
		l->i++;
		l->count++;
	}
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
	get_buffer(l, simple_text);
	return (1);
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
			{
				l->count = -1;
				break ;
			}
			zero_flags(l);
		}
		else
		{
			if (!(add_anytext_tobuff(l)))
			{
				l->count = 0;
				break ;
			}
		}
	}
	length = ft_strlen(l->buffer_for_write);
	(l->n_count > 0) ? dawrin_nulls(l) : 1;
	if (length)
		write(1, l->buffer_for_write, length);
	(l->buffer_for_write) ? free(l->buffer_for_write) : 1;
	free(l);
	return (length);
}

// int					main(int argc, char **argv)
// {
// 	int count;
// 	int	count1;

// 	ft_printf("%.2c\n", NULL);
// 	printf("%.2c\n", NULL);

// 	// printf("%d\n", count);
// 	// printf("%d", count1);
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