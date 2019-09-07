/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabre <msabre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 21:05:20 by msabre            #+#    #+#             */
/*   Updated: 2019/09/05 19:51:17 by msabre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINFT_H
# define FT_PRINFT_H
# include "libft/libft.h"
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>

int			ft_printf(const char *format, ...);

#endif