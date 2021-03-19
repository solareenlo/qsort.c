/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 15:43:02 by tayamamo          #+#    #+#             */
/*   Updated: 2021/03/19 09:57:44 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static inline void	swap_char_multiple(char *a, char *b, size_t size)
{
	size_t	i;
	char	*pi;
	char	*pj;
	char	tmp;

	i = 0;
	pi = a;
	pj = b;
	while (i < size / sizeof(char))
	{
		tmp = pi[i];
		pi[i] = pj[i];
		pj[i] = tmp;
		i++;
	}
}

static inline void	swap_int_multiple(int *a, int *b, size_t size)
{
	size_t	i;
	int		*pi;
	int		*pj;
	int		tmp;

	i = 0;
	pi = a;
	pj = b;
	while (i < size / sizeof(int))
	{
		tmp = pi[i];
		pi[i] = pj[i];
		pj[i] = tmp;
		i++;
	}
}

static inline void	swap_long_multiple(long *a, long *b, size_t size)
{
	size_t	i;
	long	*pi;
	long	*pj;
	long	tmp;

	i = 0;
	pi = a;
	pj = b;
	while (i < size / sizeof(long))
	{
		tmp = pi[i];
		pi[i] = pj[i];
		pj[i] = tmp;
		i++;
	}
}

void	ft_swap(void *a, void *b, size_t size)
{
	if (a == b)
		return ;
	if (size == sizeof(long))
	{
		*(long *)a = *(long *)a - *(long *)b;
		*(long *)b = *(long *)a + *(long *)b;
		*(long *)a = *(long *)b - *(long *)a;
	}
	else if (size == sizeof(int))
	{
		*(int *)a = *(int *)a - *(int *)b;
		*(int *)b = *(int *)a + *(int *)b;
		*(int *)a = *(int *)b - *(int *)a;
	}
	else if (size % sizeof(long) == 0)
		swap_long_multiple(a, b, size);
	else if (size % sizeof(int) == 0)
		swap_int_multiple(a, b, size);
	else
		swap_char_multiple(a, b, size);
}
