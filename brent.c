#include <math.h>

#define ITMAX 100
#define CGOLD 0.3819660
#define ZEPS 1.0e-10
#define SHFT(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);
#define SIGN(a,b) (((b) >= 0.0)? fabs(a) : -fabs(a))

/*
 * Brent algorithm
 */
double brent(double ax, double bx, double cx, double (*f)(double), double tol, double *xmin)
{
	int iter;
	double a, b, d, etemp, fu, fv, fw, fx, p, q, r, tol1, tol2, u, v, w, x, xm;
	double e; /* step length */

	d = e = 0.0;
	a = (ax < cx)? ax : cx;
	b = (ax > cx)? ax : cx;
	x = w = v = bx;
	fw = fv = fx = (*f)(x);
	for (iter = 1; iter < ITMAX; ++iter) {
		xm = 0.5 * (a + b);
		tol2 = 2.0 * (tol1 = tol * fabs(x) + ZEPS);
		if (fabs(x - xm) <= tol2 - 0.5 * (b-a)) {
			*xmin = x;
			return fx; /* done */
		}
		if (fabs(e) > tol1) { /* construct a trial parabolic fit */
			r = (x - w) * (fx - fv);
			q = (x - v) * (fx - fw);
			p = (x - v) * q - (x - w) * r;
			q = 2.0 * (q - r);
			if (q > 0.0) p = -p;
			q = fabs(q);
			etemp = e;
			e = d;
			if (fabs(p) >= fabs(0.5 * q * etemp) || p <= q * (a - x) || p >= q * (b - x))
				/* This condition determine the acceptability of parabolic fit. Here we
				 * take golden section step into the larger of the two segments. */
				d = CGOLD * (e = (x >= xm)? a - x : b - x);
			else {
				d = p / q;
				u = x + d;
				if (u - a < tol2 || b - u < tol2)
					d = SIGN(tol1, xm - x);
			}
		} else d = CGOLD * (e = (x >= xm)? a - x : b - x);
		u = (fabs(d) >= tol1)? x + d : x + SIGN(tol1, d);
		fu = (*f)(u);
		/* This is the one function evaluation per iteration. */
		if (fu <= fx) {
			if (u >= x) a = x;
			else b = x;
			SHFT(v, w, x, u);
			SHFT(fv, fw, fx, fu);
		} else {
			if (u < x) a = u;
			else b = u;
			if (fu <= fw || w == x) {
				v = w; w = u;
				fv = fw; fw = fu;
			} else if (fu <= fv || v == x || v == w) {
				v = u;
				fv = fu;
			}
		}
	}
	*xmin = x;
	return fx;
}

#ifdef DEBUG
double f(double x)
{
	return (x-1) * (x-1);
}
int main()
{
	double x, y;
	y = brent(-10, -5, 0, f, 1e-5, &x);
	printf("x=%f, y=%f\n", x, y);
	return 0;
}
#endif
