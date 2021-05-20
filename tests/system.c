#include <math.h>
#include "dd.h"

static inline double get_dfdx_cell(int full_idx0, int dx_mapped_size1, int al_idx2, int inv_mapping_period3, int *restrict dx_idx_mappings4, dd *restrict der_vec5) {
  return ((al_idx2 < inv_mapping_period3) ? ((dx_idx_mappings4[((inv_mapping_period3 * full_idx0) + al_idx2)] >= 0) ? der_vec5[((dx_mapped_size1 * full_idx0) + dx_idx_mappings4[((inv_mapping_period3 * full_idx0) + al_idx2)])] : 0.0) : 0.0);
}

static inline double get_dfdx_cell_dx(int full_idx0, int dx_mapped_size1, int al_idx2, int inv_mapping_period3, int *restrict dx_idx_mappings4, dd *restrict der_vec5) {
  return ((al_idx2 < inv_mapping_period3) ? ((al_idx2 == full_idx0) ? 1.0 : ((dx_idx_mappings4[((inv_mapping_period3 * full_idx0) + al_idx2)] >= 0) ? der_vec5[((dx_mapped_size1 * full_idx0) + dx_idx_mappings4[((inv_mapping_period3 * full_idx0) + al_idx2)])] : 0.0)) : 0.0);
}

static inline double get_dfdx_var(int al_idx7, int inv_mapping_period8, int *restrict dx_idx_mappings9, dd *restrict der_vec10) {
  return ((((al_idx7 < inv_mapping_period8) ? dx_idx_mappings9[al_idx7] : -1) >= 0) ? der_vec10[((al_idx7 < inv_mapping_period8) ? dx_idx_mappings9[al_idx7] : -1)] : 0.0);
}

int forceSub(dd *restrict forceSub11, dd *restrict body112, dd *restrict body213) {
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

int newState(dd *restrict newState17, dd *restrict bodies118) {
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
    for (int j23 = 0; j23 < 8; j23++) {
      dd body224[7];
      {
        for (int slice_idx = 0; slice_idx < 7; slice_idx++) {
          body224[(0 + slice_idx)] = bodies118[((j23 * 7) + slice_idx)];
        }
      }
      dd f25[3];
      {
        dd forceSub70527[3];
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
            forceSub70527[(0 + slice_idx)] = dd_mul(_inl_forceSub_dist29[(0 + slice_idx)] , _inl_forceSub_temp31);
          }
        }
        for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
          f25[(0 + slice_idx)] = forceSub70527[(0 + slice_idx)];
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
        dd forceSub89236[3];
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
            forceSub89236[(0 + slice_idx)] = dd_mul(_inl_forceSub_dist38[(0 + slice_idx)] , _inl_forceSub_temp40);
          }
        }
        for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
          f34[(0 + slice_idx)] = forceSub89236[(0 + slice_idx)];
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

