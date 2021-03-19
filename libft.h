/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 09:54:06 by tayamamo          #+#    #+#             */
/*   Updated: 2021/03/19 09:57:09 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>

typedef int	t_cmp(const void *a, const void *b);
int		ft_isort(void *base, size_t n, size_t size, t_cmp *cmp, int swap_limit);
void	ft_qsort(void *a, size_t n, size_t size, t_cmp *cmp);
void	ft_swap(void *a, void *b, size_t size);

#endif
