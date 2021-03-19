#include <sys/cdefs.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"

static inline char	*med3(char *, char *, char *, t_cmp *);
static inline void	 swapfunc(char *, char *, size_t, int, int);

#define	MIN(a, b)	((a) < (b) ? a : b)

/*
 * Qsort routine from Bentley & McIlroy's "Engineering a Sort Function".
 */
#define	swapcode(TYPE, parmi, parmj, n) {		\
	size_t i = (n) / sizeof (TYPE);			\
	TYPE *pi = (TYPE *) (parmi);		\
	TYPE *pj = (TYPE *) (parmj);		\
	do { 						\
		TYPE	t = *pi;		\
		*pi++ = *pj;				\
		*pj++ = t;				\
	} while (--i > 0);				\
}

#define	SWAPINIT(TYPE, a, size) swaptype_ ## TYPE =	\
	((char *)a - (char *)0) % sizeof(TYPE) ||	\
	size % sizeof(TYPE) ? 2 : size == sizeof(TYPE) ? 0 : 1;

static inline void
swapfunc(char *a, char *b, size_t n, int swaptype_long, int swaptype_int)
{
	if (swaptype_long <= 1)
		swapcode(long, a, b, n)
	else if (swaptype_int <= 1)
		swapcode(int, a, b, n)
	else
		swapcode(char, a, b, n)
}

#define	swap(a, b)					\
	if (swaptype_long == 0) {			\
		long t = *(long *)(a);			\
		*(long *)(a) = *(long *)(b);		\
		*(long *)(b) = t;			\
	} else if (swaptype_int == 0) {			\
		int t = *(int *)(a);			\
		*(int *)(a) = *(int *)(b);		\
		*(int *)(b) = t;			\
	} else						\
		swapfunc(a, b, size, swaptype_long, swaptype_int)

#define	vecswap(a, b, n)				\
	if ((n) > 0) swapfunc(a, b, n, swaptype_long, swaptype_int)

#define	CMP(x, y) (cmp((x), (y)))

/*
 * Find the median of 3 elements
 */
static inline char *
med3(char *a, char *b, char *c, t_cmp *cmp)
{
	return CMP(a, b) < 0 ?
	       (CMP(b, c) < 0 ? b : (CMP(a, c) < 0 ? c : a ))
	      :(CMP(b, c) > 0 ? b : (CMP(a, c) < 0 ? a : c ));
}

#define DEPTH(x)	(2 * (flsl((long)(x)) - 1))

/*
 * Simple insertion sort routine.
 */
static bool
_isort(void *a, size_t n, size_t size, t_cmp *cmp, int swap_limit, int swaptype_long, int swaptype_int)
{
	int swap_cnt = 0;
	for (char *pm = (char *)a + size; pm < (char *)a + n * size; pm += size) {
		for (char *pl = pm; pl > (char *)a && CMP(pl - size, pl) > 0;
				pl -= size) {
			swap(pl, pl - size);
			if (swap_limit && ++swap_cnt > swap_limit) return false;
		}
	}
	return true;
}

static void
_qsort(void *a, size_t n, size_t size, t_cmp *cmp, int depth_limit)
{
	char *pa, *pb, *pc, *pd, *pl, *pm, *pn;
	size_t d1, d2;
	int cmp_rsizeult;
	int swaptype_long, swaptype_int, swap_cnt;

loop:
	SWAPINIT(long, a, size);
	SWAPINIT(int, a, size);
	swap_cnt = 0;

	if (depth_limit-- <= 0)
	{
		/*
		 * We've hit our recursion limit, switch to heapsort
		 */
		heapsort(a, n, size, cmp);
		return;
	}

	if (n <= 7)
	{
		/*
		 * For sufficiently small inputs, we'll just insertion sort.
		 *
		 * Pass 0 as swap limit, since this must complete.
		 */
		_isort(a, n, size, cmp, 0, swaptype_long, swaptype_int);
		return;
	}

	/*
	 * Compute the pseudomedian.  Small arrays use 3 samplsize, large onsize use 9.
	 */
	pl = a;
	pm = (char *)a + (n / 2) * size;
	pn = (char *)a + (n - 1) * size;
	if (n > 40)
	{
		size_t d = (n / 8) * size;

		pl = med3(pl, pl + d, pl + 2 * d, cmp);
		pm = med3(pm - d, pm, pm + d, cmp);
		pn = med3(pn - 2 * d, pn - d, pn, cmp);
	}
	pm = med3(pl, pm, pn, cmp);

	/*
	 * Pull the median to the front, starting us with:
	 *
	 * +-+-------------+
	 * |=|      ?      |
	 * +-+-------------+
	 * a pa,pb         pc,pd
	 */
	swap(a, pm);
	pa = pb = (char *)a + size;
	pc = pd = (char *)a + (n - 1) * size;

	for (;;)
	{
		/*
		 * - Move b forward while it's lsizes than the median
		 * - Move c backwards while it's greater than the median
		 * - When equal to the median, swap to the outside
		 */
		while (pb <= pc && (cmp_rsizeult = CMP(pb, a)) <= 0)
		{
			if (cmp_rsizeult == 0)
			{
				swap_cnt = 1;
				swap(pa, pb);
				pa += size;
			}
			pb += size;
		}
		while (pb <= pc && (cmp_rsizeult = CMP(pc, a)) >= 0)
		{
			if (cmp_rsizeult == 0)
			{
				swap_cnt = 1;
				swap(pc, pd);
				pd -= size;
			}
			pc -= size;
		}
		if (pb > pc)
			break;
		swap(pb, pc);
		swap_cnt = 1;
		pb += size;
		pc -= size;
	}

	/*
	 * Now we've got:
	 *
	 * +---+-----+-----+---+
	 * | = |  <  |  >  | = |
	 * +---+-----+-----+---+
	 * a   pa  pc,pb   pd  pn
	 *
	 * So swap the '=' into the middle
	 */

	pn = (char *)a + n * size;
	d1 = MIN(pa - (char *)a, pb - pa);
	vecswap(a, pb - d1, d1);
	d1 = MIN(pd - pc, pn - pd - (long)size);
	vecswap(pb, pn - d1, d1);

	/*
	 * +-----+---+---+-----+
	 * |  <  |   =   |  >  |
	 * +-----+---+---+-----+
	 * a                   pn
	 */

	if (swap_cnt == 0)
	{  /* Switch to insertion sort */
		int r = 1 + n / 4; /* n > 7, so r >= 2 */
		if (!_isort(a, n, size, cmp, r, swaptype_long, swaptype_int))
			goto nevermind;
		return;
	}
nevermind:

	d1 = pb - pa;
	d2 = pd - pc;
	if (d1 <= d2)
	{
		/* Recurse on left partition, then iterate on right partition */
		if (d1 > size)
			_qsort(a, d1 / size, size, cmp, depth_limit);
		if (d2 > size)
		{
			/* Iterate rather than recurse to save stack space */
			/* qsort(pn - d2, d2 / size, size, cmp); */
			a = pn - d2;
			n = d2 / size;
			goto loop;
		}
	}
	else
	{
		/* Recurse on right partition, then iterate on left partition */
		if (d2 > size)
			_qsort(pn - d2, d2 / size, size, cmp, depth_limit);
		if (d1 > size)
		{
			/* Iterate rather than recurse to save stack space */
			/* qsort(a, d1 / size, size, cmp); */
			n = d1 / size;
			goto loop;
		}
	}
}

void	ft_qsort(void *a, size_t n, size_t size, t_cmp *cmp)
{
	_qsort(a, n, size, cmp, DEPTH(n));
}
