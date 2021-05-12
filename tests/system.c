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

;
int xdot(dd *restrict xdot11, dd *restrict x12, dd GM13) {
  const static int dstate_dot468dGM_mpg14[6] = { 0, 0, 0, 0, 0, 0 };
  const static int dstate_dot468dGM_inv_mpg15[6] = { 0, 0, 0, 0, 0, 0 };
  const static int dstate314dGM_mpg16[6] = { 0, 0, 0, 0, 0, 0 };
  const static int dstate314dGM_inv_mpg17[6] = { 0, 0, 0, 0, 0, 0 };
  const static int ddist2553dGM_mpg18[1] = { 0 };
  const static int ddist2553dGM_inv_mpg19[1] = { 0 };
  const static int ddist3inv566dGM_mpg20[1] = { 0 };
  const static int ddist3inv566dGM_inv_mpg21[1] = { 0 };
  const static int dstate_dot468dinitial_mpg22[36] = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5 };
  const static int dstate_dot468dinitial_inv_mpg23[36] = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5 };
  const static int dstate314dinitial_mpg24[36] = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5 };
  const static int dstate314dinitial_inv_mpg25[36] = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5 };
  const static int ddist2553dinitial_mpg26[6] = { 0, 1, 2, 3, 4, 5 };
  const static int ddist2553dinitial_inv_mpg27[6] = { 0, 1, 2, 3, 4, 5 };
  const static int ddist3inv566dinitial_mpg28[6] = { 0, 1, 2, 3, 4, 5 };
  const static int ddist3inv566dinitial_inv_mpg29[6] = { 0, 1, 2, 3, 4, 5 };
  dd chicken30;
  dd beer31[5];
  dd state_derivatives_initial32[36];
  {
    for (int slice_idx = 0; slice_idx < 36; slice_idx++) {
      state_derivatives_initial32[(0 + slice_idx)] = x12[(6 + slice_idx)];
    }
  }
  dd state_derivatives_gm33[6];
  {
    for (int slice_idx = 0; slice_idx < 6; slice_idx++) {
      state_derivatives_gm33[(0 + slice_idx)] = x12[(42 + slice_idx)];
    }
  }
  dd state36[6];
  double dstate314dGM_der34[6];
  double dstate314dinitial_der35[36];
  for (int slice_idx = 0; slice_idx < 6; slice_idx++) {
    for (int mapped_idx = 0;; mapped_idx++) {
      int mappings_full_idx_symbol;
      mappings_full_idx_symbol = (mapped_idx + (1 * (0 + slice_idx)));
      if ((mappings_full_idx_symbol >= 6)) {
        break;
      } else {
        int al_index_name_symbol;
        al_index_name_symbol = dstate314dGM_mpg16[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 1))) {
          break;
        } else {
          dstate314dGM_der34[((1 * (0 + slice_idx)) + mapped_idx)] = 0.0;
        }
      }
    }
  }
  for (int slice_idx = 0; slice_idx < 6; slice_idx++) {
    for (int mapped_idx = 0;; mapped_idx++) {
      int mappings_full_idx_symbol;
      mappings_full_idx_symbol = (mapped_idx + (6 * (0 + slice_idx)));
      if ((mappings_full_idx_symbol >= 36)) {
        break;
      } else {
        int al_index_name_symbol;
        al_index_name_symbol = dstate314dinitial_mpg24[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 6))) {
          break;
        } else {
          dstate314dinitial_der35[((6 * (0 + slice_idx)) + mapped_idx)] = 0.0;
        }
      }
    }
  }
  for (int slice_idx = 0; slice_idx < 6; slice_idx++) {
    state36[(0 + slice_idx)] = x12[(0 + slice_idx)];
  }
  for (int df_slice_idx = 0; df_slice_idx < 6; df_slice_idx++) {
    for (int dx_slice_idx = 0; dx_slice_idx < 6; dx_slice_idx++) {
      int mapped_idx;
      mapped_idx = (((0 + dx_slice_idx) < 6) ? dstate314dinitial_inv_mpg25[(((0 + df_slice_idx) * 6) + (0 + dx_slice_idx))] : -1);
      if ((mapped_idx >= 0)) {
        int slice_idx;
        slice_idx = ((df_slice_idx * 6) + dx_slice_idx);
        dstate314dinitial_der35[(((0 + df_slice_idx) * 6) + mapped_idx)] = state_derivatives_initial32[(0 + slice_idx)];
      }
    }
  }
  for (int slice_idx = 0; slice_idx < 6; slice_idx++) {
    int mapped_idx;
    mapped_idx = ((0 < 1) ? dstate314dGM_inv_mpg17[(((0 + slice_idx) * 1) + 0)] : -1);
    if ((mapped_idx >= 0)) {
      dstate314dGM_der34[(((0 + slice_idx) * 1) + mapped_idx)] = state_derivatives_gm33[(0 + slice_idx)];
    }
  }
  dd state_dot39[6];
  double dstate_dot468dGM_der37[6];
  double dstate_dot468dinitial_der38[36];
  for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
    for (int mapped_idx = 0;; mapped_idx++) {
      int mappings_full_idx_symbol;
      mappings_full_idx_symbol = (mapped_idx + (1 * (0 + slice_idx)));
      if ((mappings_full_idx_symbol >= 6)) {
        break;
      } else {
        int al_index_name_symbol;
        al_index_name_symbol = dstate_dot468dGM_mpg14[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 1))) {
          break;
        } else {
          dstate_dot468dGM_der37[((1 * (0 + slice_idx)) + mapped_idx)] = dstate314dGM_der34[(((3 + slice_idx) * 1) + al_index_name_symbol)];
        }
      }
    }
  }
  for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
    for (int mapped_idx = 0;; mapped_idx++) {
      int mappings_full_idx_symbol;
      mappings_full_idx_symbol = (mapped_idx + (6 * (0 + slice_idx)));
      if ((mappings_full_idx_symbol >= 36)) {
        break;
      } else {
        int al_index_name_symbol;
        al_index_name_symbol = dstate_dot468dinitial_mpg22[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 6))) {
          break;
        } else {
          dstate_dot468dinitial_der38[((6 * (0 + slice_idx)) + mapped_idx)] = dstate314dinitial_der35[(((3 + slice_idx) * 6) + al_index_name_symbol)];
        }
      }
    }
  }
  for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
    state_dot39[(0 + slice_idx)] = state36[(3 + slice_idx)];
  }
  {
    for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
      xdot11[(0 + slice_idx)] = state_dot39[(0 + slice_idx)];
    }
  }
  dd dist242;
  double ddist2553dGM_der40[1];
  double ddist2553dinitial_der41[6];
  dd dist3inv45;
  double ddist3inv566dGM_der43[1];
  double ddist3inv566dinitial_der44[6];
  for (int loop_var46 = 0; loop_var46 < 1; loop_var46++) {
    int mapped_idx;
    mapped_idx = loop_var46;
    int al_index_name_symbol;
    al_index_name_symbol = ddist2553dGM_mpg18[loop_var46];
    ddist2553dGM_der40[mapped_idx] = dd_add(dd_add(dd_mul(dd_mul(2.0 , state36[0]) , dstate314dGM_der34[((0 * 1) + al_index_name_symbol)]) , dd_mul(dd_mul(2.0 , state36[1]) , dstate314dGM_der34[((1 * 1) + al_index_name_symbol)])) , dd_mul(dd_mul(2.0 , state36[2]) , dstate314dGM_der34[((2 * 1) + al_index_name_symbol)]));
  }
  for (int loop_var47 = 0; loop_var47 < 6; loop_var47++) {
    int mapped_idx;
    mapped_idx = loop_var47;
    int al_index_name_symbol;
    al_index_name_symbol = ddist2553dinitial_mpg26[loop_var47];
    ddist2553dinitial_der41[mapped_idx] = dd_add(dd_add(dd_mul(dd_mul(2.0 , state36[0]) , dstate314dinitial_der35[((0 * 6) + al_index_name_symbol)]) , dd_mul(dd_mul(2.0 , state36[1]) , dstate314dinitial_der35[((1 * 6) + al_index_name_symbol)])) , dd_mul(dd_mul(2.0 , state36[2]) , dstate314dinitial_der35[((2 * 6) + al_index_name_symbol)]));
  }
  dist242 = dd_add(dd_add((state36[0] * state36[0]) , (state36[1] * state36[1])) , (state36[2] * state36[2]));
  for (int loop_var48 = 0; loop_var48 < 1; loop_var48++) {
    int mapped_idx;
    mapped_idx = loop_var48;
    int al_index_name_symbol;
    al_index_name_symbol = ddist3inv566dGM_mpg20[loop_var48];
    ddist3inv566dGM_der43[mapped_idx] = dd_mul(dd_neg(1.0) , dd_mul(1.0e0 , dd_mul(dd_div(1.0 , (dd_mul(dist242 , dd_sqrt(dist242)) * dd_mul(dist242 , dd_sqrt(dist242)))) , dd_add(dd_mul(dist242 , dd_mul(0.5 , dd_mul(dd_sqrt(dd_div(1.0 , dist242)) , ddist2553dGM_der40[al_index_name_symbol]))) , dd_mul(dd_sqrt(dist242) , ddist2553dGM_der40[al_index_name_symbol])))));
  }
  for (int loop_var49 = 0; loop_var49 < 6; loop_var49++) {
    int mapped_idx;
    mapped_idx = loop_var49;
    int al_index_name_symbol;
    al_index_name_symbol = ddist3inv566dinitial_mpg28[loop_var49];
    ddist3inv566dinitial_der44[mapped_idx] = dd_mul(dd_neg(1.0) , dd_mul(1.0e0 , dd_mul(dd_div(1.0 , (dd_mul(dist242 , dd_sqrt(dist242)) * dd_mul(dist242 , dd_sqrt(dist242)))) , dd_add(dd_mul(dist242 , dd_mul(0.5 , dd_mul(dd_sqrt(dd_div(1.0 , dist242)) , ddist2553dinitial_der41[al_index_name_symbol]))) , dd_mul(dd_sqrt(dist242) , ddist2553dinitial_der41[al_index_name_symbol])))));
  }
  dist3inv45 = dd_div_d_dd(1.0e0 , dd_mul(dist242 , dd_sqrt(dist242)));
  for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
    for (int mapped_idx = 0;; mapped_idx++) {
      int mappings_full_idx_symbol;
      mappings_full_idx_symbol = (mapped_idx + (1 * (3 + slice_idx)));
      if ((mappings_full_idx_symbol >= 6)) {
        break;
      } else {
        int al_index_name_symbol;
        al_index_name_symbol = dstate_dot468dGM_mpg14[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 1))) {
          break;
        } else {
          dstate_dot468dGM_der37[((1 * (3 + slice_idx)) + mapped_idx)] = dd_add(dd_mul(dd_mul(GM13 , dd_neg(state36[(0 + slice_idx)])) , ddist3inv566dGM_der43[al_index_name_symbol]) , dd_mul(dist3inv45 , dd_add(dd_mul(GM13 , dd_neg(dstate314dGM_der34[(((0 + slice_idx) * 1) + al_index_name_symbol)])) , dd_mul(dd_neg(state36[(0 + slice_idx)]) , 1.0))));
        }
      }
    }
  }
  for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
    for (int mapped_idx = 0;; mapped_idx++) {
      int mappings_full_idx_symbol;
      mappings_full_idx_symbol = (mapped_idx + (6 * (3 + slice_idx)));
      if ((mappings_full_idx_symbol >= 36)) {
        break;
      } else {
        int al_index_name_symbol;
        al_index_name_symbol = dstate_dot468dinitial_mpg22[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 6))) {
          break;
        } else {
          dstate_dot468dinitial_der38[((6 * (3 + slice_idx)) + mapped_idx)] = dd_add(dd_mul(dd_mul(GM13 , dd_neg(state36[(0 + slice_idx)])) , ddist3inv566dinitial_der44[al_index_name_symbol]) , dd_mul(dist3inv45 , dd_add(dd_mul(GM13 , dd_neg(dstate314dinitial_der35[(((0 + slice_idx) * 6) + al_index_name_symbol)])) , dd_mul(dd_neg(state36[(0 + slice_idx)]) , 0.0))));
        }
      }
    }
  }
  for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
    state_dot39[(3 + slice_idx)] = dd_mul(dd_mul(GM13 , dd_neg(state36[(0 + slice_idx)])) , dist3inv45);
  }
  {
    for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
      xdot11[(3 + slice_idx)] = state_dot39[(3 + slice_idx)];
    }
  }
  for (int slice_idx = 0; slice_idx < (36 / 6); slice_idx++) {
    for (int dx_slice_idx = 0; dx_slice_idx < 6; dx_slice_idx++) {
      int al_index_name_symbol;
      al_index_name_symbol = (dx_slice_idx + 0);
      int func_slice_idx;
      func_slice_idx = ((6 * slice_idx) + dx_slice_idx + 6);
      xdot11[func_slice_idx] = dstate_dot468dinitial_der38[(((0 + slice_idx) * 6) + al_index_name_symbol)];
    }
  }
  for (int slice_idx = 0; slice_idx < 6; slice_idx++) {
    int al_index_name_symbol;
    al_index_name_symbol = 0;
    int func_slice_idx;
    func_slice_idx = (slice_idx + 42);
    xdot11[func_slice_idx] = dstate_dot468dGM_der37[(((0 + slice_idx) * 1) + al_index_name_symbol)];
  }
  return 0;
}

