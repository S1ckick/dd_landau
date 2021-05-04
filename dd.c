typedef struct{
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

/*---------------------------HELPERS------------------------------*/


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


/* mul */
void c_dd_mul(const double *a, const double *b, double *c) {
  dd_real cc;
  cc = dd_real(a) * dd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_dd_mul_dd_d(const double *a, double b, double *c) {
  dd_real cc;
  cc = dd_real(a) * b;
  TO_DOUBLE_PTR(cc, c);
}
void c_dd_mul_d_dd(double a, const double *b, double *c) {
  dd_real cc;
  cc = a * dd_real(b);
  TO_DOUBLE_PTR(cc, c);
}


/* div */
void c_dd_div(const double *a, const double *b, double *c) {
  dd_real cc;
  cc = dd_real(a) / dd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
void c_dd_div_dd_d(const double *a, double b, double *c) {
  dd_real cc;
  cc = dd_real(a) / b;
  TO_DOUBLE_PTR(cc, c);
}
void c_dd_div_d_dd(double a, const double *b, double *c) {
  dd_real cc;
  cc = a / dd_real(b);
  TO_DOUBLE_PTR(cc, c);
}
