#include "dd.h"

/*---------------------------HELPERS------------------------------*/

#define _QD_SPLITTER 134217729.0                // = 2^27 + 1
#define _QD_SPLIT_THRESH 6.69692879491417e+299  // = 2^996

double quick_two_sum(double a, double b, double *err) {
    double s = a + b;
    *err = b - (s - a);
    return s;
}

/* Computes fl(a+b) and err(a+b).  */
double two_sum(double a, double b, double *err) {
    double s = a + b;
    double bb = s - a;
    *err = (a - (s - bb)) + (b - bb);
    return s;
}

/* double-double + double-double */
dd ieee_add(dd a, dd b) {
    /* This one satisfies IEEE style error bound,
       due to K. Briggs and W. Kahan.                   */
    double s1, s2, t1, t2;

    s1 = two_sum(a.x, b.x, &s2);
    t1 = two_sum(a.y, b.y, &t2);
    s2 += t1;
    s1 = quick_two_sum(s1, s2, &s2);
    s2 += t2;
    s1 = quick_two_sum(s1, s2, &s2);
    dd res = {s1,s2};
    return res;
}

dd sloppy_add(dd a, dd b) {
    /* This is the less accurate version ... obeys Cray-style
       error bound. */
    double s, e;
    s = two_sum(a.x, b.x, &e);
    e += (a.y + b.y);
    s = quick_two_sum(s, e, &e);
    dd res = {s,e};
    return res;
}

/* Computes fl(a-b) and err(a-b).  */
double two_diff(double a, double b, double *err) {
    double s = a - b;
    double bb = s - a;
    *err = (a - (s - bb)) - (b + bb);
    return s;
}

/* Computes fl(a-b) and err(a-b).  Assumes |a| >= |b| */
double quick_two_diff(double a, double b, double *err) {
    double s = a - b;
    *err = (a - s) - b;
    return s;
}

/* Computes high word and lo word of a */
void split(double a, double *hi, double *lo) {
  double temp;
  if (a > _QD_SPLIT_THRESH || a < -_QD_SPLIT_THRESH) {
    a *= 3.7252902984619140625e-09;  // 2^-28
    temp = _QD_SPLITTER * a;
    *hi = temp - (temp - a);
    *lo = a - *hi;
    *hi *= 268435456.0;  // 2^28
    *lo *= 268435456.0;  // 2^28
  } else {
    temp = _QD_SPLITTER * a;
    *hi = temp - (temp - a);
    *lo = a - *hi;
  }
}

/* Computes fl(a*b) and err(a*b) */
double two_prod(double a, double b, double *err) {
#ifdef QD_FMS
    double p = a * b;
    err = QD_FMS(a, b, p);
    return p;
#else
    double a_hi, a_lo, b_hi, b_lo;
    double p = a * b;
    split(a, &a_hi, &a_lo);
    split(b, &b_hi, &b_lo);
    *err = ((a_hi * b_hi - p) + a_hi * b_lo + a_lo * b_hi) + a_lo * b_lo;
    return p;
#endif
}

dd sloppy_div(dd a, dd b) {
  double s1, s2;
  double q1, q2;
  dd r;

  q1 = a.x / b.x; /* approximate quotient */

  /* compute  this - q1 * dd */
  r = dd_mul_dd_d(b, q1);
  s1 = two_diff(a.x, r.x, &s2);
  s2 -= r.y;
  s2 += a.y;

  /* get next approximation */
  q2 = (s1 + s2) / b.x;

  /* renormalize */
  r.x = quick_two_sum(q1, q2, &r.y);
  return r;
}

dd accurate_div(dd a, dd b) {
  double q1, q2, q3;
  dd r;

  q1 = a.x / b.x; /* approximate quotient */

  r = dd_sub(a, dd_mul_d_dd(q1, b));

  q2 = r.x / b.x;
  r = dd_sub(r, dd_mul_d_dd(q2, b));

  q3 = r.x / b.x;

  q1 = quick_two_sum(q1, q2, &q2);
  dd q12 = {q1, q2};
  r = dd_add_dd_d(q12, q3);
  return r;
}
/* ------------ END HELPERS --------------------- */

/* -----------------additions----------------- */
dd dd_add(dd a, dd b) {
#ifdef QD_IEEE_ADD
    return ieee_add(a,b);
#else
    return sloppy_add(a,b);
#endif
}

dd dd_add_dd_d(dd a, double b) {
    double s1, s2;
    s1 = two_sum(a.x, b, &s2);
    s2 += a.y;
    s1 = quick_two_sum(s1, s2, &s2);
    dd res = {s1, s2};
    return res;
}

dd dd_add_d_dd(double a, dd b){
    return dd_add_dd_d(b, a);
}

/* ----------------- END additions----------------- */

/* -----------------subtractions----------------- */

/* double-double - double-double */
dd dd_sub(dd a, dd b) {
#ifndef QD_IEEE_ADD
    double s, e;
    s = two_diff(a.x, b.x, &e);
    e += a.y;
    e -= b.y;
    s = quick_two_sum(s, e, &e);
    dd res = {s,e};
#else
    double s1, s2, t1, t2;
    s1 = two_diff(a.x, b.x, &s2);
    t1 = two_diff(a.y, b.y, &t2);
    s2 += t1;
    s1 = quick_two_sum(s1, s2, &s2);
    s2 += t2;
    s1 = quick_two_sum(s1, s2, &s2);
    dd res = {s1,s2};
#endif
return res;
}

/* double-double - double */
dd dd_sub_dd_d(dd a, double b) {
    double s1, s2;
    s1 = two_diff(a.x, b, &s2);
    s2 += a.y;
    s1 = quick_two_sum(s1, s2, &s2);
    dd res = {s1,s2};
    return res;
}

/* double - double-double */
dd dd_sub_d_dd(double a, dd b) {
    double s1, s2;
    s1 = two_diff(a, b.x, &s2);
    s2 -= b.y;
    s1 = quick_two_sum(s1, s2, &s2);
    dd res = {s1,s2};
    return res;
}

/*********** Unary Minus ***********/
dd dd_neg(dd a){
    dd res = {-a.x, -a.y};
    return res;
}
/* -----------------END subtractions----------------- */


/* -----------------multiplications----------------- */

/* double-double * double-double */
dd dd_mul(dd a, dd b) {
  double p1, p2;

  p1 = two_prod(a.x, b.x, &p2);
  p2 += (a.x * b.y + a.y * b.x);
  p1 = quick_two_sum(p1, p2, &p2);
  dd res = {p1, p2};
  return res;
}

/* double-double * double */
dd dd_mul_dd_d(dd a, double b) {
    double p1, p2;

    p1 = two_prod(a.x, b, &p2);
    p2 += (a.y * b);
    p1 = quick_two_sum(p1, p2, &p2);
    dd res = {p1, p2};
    return res;
}

/* double * double-double */
dd dd_mul_d_dd(double a, dd b) { 
    return dd_mul_dd_d(b, a);
}
/* -----------------END multiplications----------------- */

/* -----------------divosions----------------- */

/* double-double / double-double */
dd dd_div(dd a, dd b) {
#ifdef QD_SLOPPY_DIV
  return sloppy_div(a, b);
#else
  return accurate_div(a, b);
#endif
}

/* double / double-double */
dd dd_div_d_dd(double a, dd b) { 
  dd dd_a = {a, 0};
  return dd_div(dd_a, b); 
}

/* double-double / double */
dd dd_div_dd_d(dd a, double b) { 
  dd dd_b = {b, 0};
  return dd_div(a, dd_b); 
}
