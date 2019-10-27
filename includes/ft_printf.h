/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 21:05:20 by msabre            #+#    #+#             */
/*   Updated: 2019/10/27 22:10:08 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINFT_H
# define FT_PRINFT_H
# include "libft/libft.h"
# include <stdarg.h>

typedef struct			s_list
{
	int					count;
	int					i;
	int					sp;
	int					dot;
	int					save;
	int					type;
	int					flag;
	int					start;
	int					fhash;
	int					fplus;
	int					fzero;
	int					fminus;
	int					precision_minus;
	int					length;
	int					precision;
	int					out_length;
	int					dop;
	int					dop_count;
	int					cut_s;
	int					n_count;
	int					free_block;
	char				*out;
	char				darwin_null[15];
	char				spase;
	char				*hash;
	char				*buffer_for_write;
	const char			*format;
	void				*content;
	size_t				content_size;
	struct s_list		*next;
}						t_list;

typedef union			s_uni_dub
{
	long double			val;
	struct
	{
		unsigned long	mantis : 64;
		unsigned		exp : 15;
		unsigned		sign : 1;
	}					doub;
}						t_uni_dub;

int						ft_printf(const char *format, ...);
int						ft_variants(va_list args, t_list *l);
int						pars_format(t_list *l);
int						ft_flag_function_find(va_list args, t_list *l,
						char *type);
int						output_b_flags(va_list args, t_list *l, char *type);
int						output_cs_flags(va_list args, t_list *l);
int						output_p_flags(va_list args, t_list *l, char *type);
int						output_di_flags(va_list args, char *type, t_list *l);
int						output_u_flags(va_list args, char *type, t_list *l);
int						output_xo_flags(va_list args, t_list *l, char *type);
int						output_f_flags(va_list args, t_list *l, char *type);
char					*putnbr_di(long long a);
char					*ft_itoa_usigned(unsigned long long n, int count,
						char flag);
char					*decimy_to_any(unsigned long long num_integer,
						int num_system, char flag);
char					*choose_length_putnbr(char *type, char *(f)(long long),
						long long u);
char					*choose_length_chr(char *type,
						char *(f)(unsigned long long, int, char),
						unsigned long long u, char flag);
char					*add_to_string(t_uni_dub *ptr, long double f,
						t_list *l);
int						**get_bignum(t_num_parts ***num, int count);
char					*get_bn_str(int **result, t_list *l, int sign);
t_num_parts				**get_and_fill_numparts(t_num_parts **mant_exp,
						char *mantis, int e);
char					*creat_after_dot(long double f, int precision,
						t_list *l, int e);
void					after_dot_rounding(t_list *l, char **fractional);
char					*norm_chr_ll(long double f, t_list *l, int sign);
char					*str_fr_intmass(int *a, int size, t_list *l, int sign);
int						fill_output(t_list *l, char *result);
char					*flag_inicializatian(t_list *l);
int						add_anytext_tobuff(t_list *l);
int						chr_output(t_list *l);
int						get_buffer(t_list *l, char *new_str);
int						ft_num_sys(char flag);

#endif