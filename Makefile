# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msabre <msabre@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/27 21:47:45 by msabre            #+#    #+#              #
#    Updated: 2019/10/28 19:52:34 by msabre           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

C = clang

NAME = libftprintf.a

FLAGS = -c -Wall -Werror -Wextra

LIBFT = ./includes/libft

DIR_S = src

DIR_O = temporary

HEADER = includes/ft_printf.h

SOURCES = ft_printf.c variants.c scp_flags.c parse_format.c output.c output_define.c output_fill.c ft_flag_find.c f_str_fr_int_mass.c f_regular_value.c f_long_value.c f_long_summ.c f_long_multi.c \
f_long_ariphmetic.c f_flag.c dioux_transitions.c dioux_functions.c dioux_flags.c \

OBJS = $(addprefix $(DIR_O)/,$(SOURCES:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@make -C $(LIBFT)
	@cp includes/libft/libft.a ./$(NAME)
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)

$(DIR_O)/%.o: $(DIR_S)/%.c
	@mkdir -p temporary
	@$(CC) $(FLAGS) -I $(HEADER) -o $@ -c $<

clean:
	@rm -f $(OBJS)
	@rm -rf $(DIR_O)
	@make clean -C $(LIBFT)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT)

re: fclean all
