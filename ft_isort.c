/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tayamamo <tayamamo@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 15:29:08 by tayamamo          #+#    #+#             */
/*   Updated: 2021/03/19 09:53:21 by tayamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isort(void *base, size_t n, size_t size, t_cmp *cmp, int swap_limit)
{
	int		swap_cnt;
	char	*pm;
	char	*pl;

	swap_cnt = 0;
	pm = (char *)base + size;
	while (pm < (char *)base + n * size)
	{
		pl = pm;
		while (pl > (char *)base && cmp(pl - size, pl) > 0)
		{
			ft_swap(pl, pl - size, size);
			if (swap_limit && ++swap_cnt > swap_limit)
				return (0);
			pl -= size;
		}
		pm += size;
	}
	return (1);
}
