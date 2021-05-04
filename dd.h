#ifndef DD_H
#define DD_H

typedef struct {
  double x;
  double y;
} dd;

/* add */
dd dd_add(dd a, dd b);
dd dd_add_d_dd(double a, dd b);
dd dd_add_dd_d(dd a, double b);

/* sub */
dd dd_sub(dd a, dd b);
dd dd_sub_d_dd(double a, dd b);
dd dd_sub_dd_d(dd a, double b);
dd dd_neg(dd a);

/* mul */
dd dd_mul(dd a, dd b);
dd dd_mul_d_dd(double a, dd b);
dd dd_mul_dd_d(dd a, double b);

/* div */
dd dd_div(dd a, dd b);
dd dd_div_d_dd(double a, dd b);
dd dd_div_dd_d(dd a, double b);

#endif // DD_H