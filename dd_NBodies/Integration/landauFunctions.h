
#ifndef NBODIES_LANDAUFUNCTIONS_H
#define NBODIES_LANDAUFUNCTIONS_H

extern "C" {
  #include "dd.h"
};

int forceSub(dd * forceSub11, dd * body112, dd * body213) {
  dd dist14[3];
  dist14[0] = dd_sub(body112[1] , body213[1]);
  dist14[1] = dd_sub(body112[2] , body213[2]);
  dist14[2] = dd_sub(body112[3] , body213[3]);
  dd len15;
  len15 = dd_sqrt(dd_add(dd_add(dd_mul(dist14[0] , dist14[0]) , dd_mul(dist14[1] , dist14[1])) , dd_mul(dist14[2] , dist14[2])));
  dd temp16;
  temp16 = dd_neg(dd_div(dd_mul_d_dd(6.674299999999999379e-11 , body213[0]) , dd_mul(dd_mul(len15 , len15) , len15)));
  {
    for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
      forceSub11[(0 + slice_idx)] = dd_mul(dist14[(0 + slice_idx)] , temp16);
    }
  }
  return 0;
}

int newState(dd * newState17, dd * bodies118) {
  dd newBodies19[56];
  for (int i20 = 0; i20 < 8; i20++) {
    dd total_force21[3];
    {
      for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
        total_force21[(0 + slice_idx)] = d_dd(0);
      }
    }
    dd body122[7];
    {
      for (int slice_idx = 0; slice_idx < 7; slice_idx++) {
        body122[(0 + slice_idx)] = bodies118[((i20 * 7) + slice_idx)];
      }
    }
    for (int j23 = 0; j23 < i20; j23++) {
      dd body224[7];
      {
        for (int slice_idx = 0; slice_idx < 7; slice_idx++) {
          body224[(0 + slice_idx)] = bodies118[((j23 * 7) + slice_idx)];
        }
      }
      dd f25[3];
      {
        dd forceSub69927[3];
        dd _inl_forceSub_dist29[3];
        _inl_forceSub_dist29[0] = dd_sub(body122[1] , body224[1]);
        _inl_forceSub_dist29[1] = dd_sub(body122[2] , body224[2]);
        _inl_forceSub_dist29[2] = dd_sub(body122[3] , body224[3]);
        dd _inl_forceSub_len30;
        _inl_forceSub_len30 = dd_sqrt(dd_add(dd_add(dd_mul(_inl_forceSub_dist29[0] , _inl_forceSub_dist29[0]) , dd_mul(_inl_forceSub_dist29[1] , _inl_forceSub_dist29[1])) , dd_mul(_inl_forceSub_dist29[2] , _inl_forceSub_dist29[2])));
        dd _inl_forceSub_temp31;
        _inl_forceSub_temp31 = dd_neg(dd_div(dd_mul_d_dd(6.674299999999999379e-11 , body224[0]) , dd_mul(dd_mul(_inl_forceSub_len30 , _inl_forceSub_len30) , _inl_forceSub_len30)));
        {
          for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
            forceSub69927[(0 + slice_idx)] = dd_mul(_inl_forceSub_dist29[(0 + slice_idx)] , _inl_forceSub_temp31);
          }
        }
        for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
          f25[(0 + slice_idx)] = forceSub69927[(0 + slice_idx)];
        }
      }
      {
        for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
          total_force21[(0 + slice_idx)] = dd_add(total_force21[(0 + slice_idx)] , f25[(0 + slice_idx)]);
        }
      }
    }
    for (int j32 = (i20 + 1); j32 < 8; j32++) {
      dd body233[7];
      {
        for (int slice_idx = 0; slice_idx < 7; slice_idx++) {
          body233[(0 + slice_idx)] = bodies118[((j32 * 7) + slice_idx)];
        }
      }
      dd f34[3];
      {
        dd forceSub88636[3];
        dd _inl_forceSub_dist38[3];
        _inl_forceSub_dist38[0] = dd_sub(body122[1] , body233[1]);
        _inl_forceSub_dist38[1] = dd_sub(body122[2] , body233[2]);
        _inl_forceSub_dist38[2] = dd_sub(body122[3] , body233[3]);
        dd _inl_forceSub_len39;
        _inl_forceSub_len39 = dd_sqrt(dd_add(dd_add(dd_mul(_inl_forceSub_dist38[0] , _inl_forceSub_dist38[0]) , dd_mul(_inl_forceSub_dist38[1] , _inl_forceSub_dist38[1])) , dd_mul(_inl_forceSub_dist38[2] , _inl_forceSub_dist38[2])));
        dd _inl_forceSub_temp40;
        _inl_forceSub_temp40 = dd_neg(dd_div(dd_mul_d_dd(6.674299999999999379e-11 , body233[0]) , dd_mul(dd_mul(_inl_forceSub_len39 , _inl_forceSub_len39) , _inl_forceSub_len39)));
        {
          for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
            forceSub88636[(0 + slice_idx)] = dd_mul(_inl_forceSub_dist38[(0 + slice_idx)] , _inl_forceSub_temp40);
          }
        }
        for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
          f34[(0 + slice_idx)] = forceSub88636[(0 + slice_idx)];
        }
      }
      {
        for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
          total_force21[(0 + slice_idx)] = dd_add(total_force21[(0 + slice_idx)] , f34[(0 + slice_idx)]);
        }
      }
    }
    newBodies19[(i20 * 7)] = bodies118[(i20 * 7)];
    {
      for (int slice_idx = 0; slice_idx < (((i20 * 7) + 4) - ((i20 * 7) + 1)); slice_idx++) {
        newBodies19[(((i20 * 7) + 1) + slice_idx)] = bodies118[(((i20 * 7) + 4) + slice_idx)];
      }
    }
    {
      for (int slice_idx = 0; slice_idx < (((i20 * 7) + 7) - ((i20 * 7) + 4)); slice_idx++) {
        newBodies19[(((i20 * 7) + 4) + slice_idx)] = total_force21[(0 + slice_idx)];
      }
    }
  }
  {
    for (int slice_idx = 0; slice_idx < 56; slice_idx++) {
      newState17[(0 + slice_idx)] = newBodies19[(0 + slice_idx)];
    }
  }
  return 0;
}
#endif // NBODIES_LANDAUFUNCTIONS_H
