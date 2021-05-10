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
  const static int ddist2522dGM_mpg14[1] = { 0 };
  const static int ddist2522dGM_inv_mpg15[1] = { 0 };
  const static int dstate284dGM_mpg16[6] = { 0, 0, 0, 0, 0, 0 };
  const static int dstate284dGM_inv_mpg17[6] = { 0, 0, 0, 0, 0, 0 };
  const static int dstate_dot438dGM_mpg18[6] = { 0, 0, 0, 0, 0, 0 };
  const static int dstate_dot438dGM_inv_mpg19[6] = { 0, 0, 0, 0, 0, 0 };
  const static int ddist3inv535dGM_mpg20[1] = { 0 };
  const static int ddist3inv535dGM_inv_mpg21[1] = { 0 };
  const static int ddist2522dinitial_mpg22[6] = { 0, 1, 2, 3, 4, 5 };
  const static int ddist2522dinitial_inv_mpg23[6] = { 0, 1, 2, 3, 4, 5 };
  const static int dstate284dinitial_mpg24[36] = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5 };
  const static int dstate284dinitial_inv_mpg25[36] = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5 };
  const static int dstate_dot438dinitial_mpg26[36] = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5 };
  const static int dstate_dot438dinitial_inv_mpg27[36] = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5 };
  const static int ddist3inv535dinitial_mpg28[6] = { 0, 1, 2, 3, 4, 5 };
  const static int ddist3inv535dinitial_inv_mpg29[6] = { 0, 1, 2, 3, 4, 5 };
  dd state_derivatives_initial30[36];
  {
    for (int slice_idx = 0; slice_idx < 36; slice_idx++) {
      state_derivatives_initial30[(0 + slice_idx)] = x12[(6 + slice_idx)];
    }
  }
  dd state_derivatives_gm31[6];
  {
    for (int slice_idx = 0; slice_idx < 6; slice_idx++) {
      state_derivatives_gm31[(0 + slice_idx)] = x12[(42 + slice_idx)];
    }
  }
  dd state34[6];
  dd dstate284dGM_der32[6];
  dd dstate284dinitial_der33[36];
  for (int slice_idx = 0; slice_idx < 6; slice_idx++) {
    for (int mapped_idx = 0;; mapped_idx++) {
      int mappings_full_idx_symbol;
      mappings_full_idx_symbol = (mapped_idx + (1 * (0 + slice_idx)));
      if ((mappings_full_idx_symbol >= 6)) {
        break;
      } else {
        int al_index_name_symbol;
        al_index_name_symbol = dstate284dGM_mpg16[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 1))) {
          break;
        } else {
          dstate284dGM_der32[((1 * (0 + slice_idx)) + mapped_idx)] = 0.0;
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
        al_index_name_symbol = dstate284dinitial_mpg24[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 6))) {
          break;
        } else {
          dstate284dinitial_der33[((6 * (0 + slice_idx)) + mapped_idx)] = 0.0;
        }
      }
    }
  }
  for (int slice_idx = 0; slice_idx < 6; slice_idx++) {
    state34[(0 + slice_idx)] = x12[(0 + slice_idx)];
  }
  for (int df_slice_idx = 0; df_slice_idx < 6; df_slice_idx++) {
    for (int dx_slice_idx = 0; dx_slice_idx < 6; dx_slice_idx++) {
      int mapped_idx;
      mapped_idx = (((0 + dx_slice_idx) < 6) ? dstate284dinitial_inv_mpg25[(((0 + df_slice_idx) * 6) + (0 + dx_slice_idx))] : -1);
      if ((mapped_idx >= 0)) {
        int slice_idx;
        slice_idx = ((df_slice_idx * 6) + dx_slice_idx);
        dstate284dinitial_der33[(((0 + df_slice_idx) * 6) + mapped_idx)] = state_derivatives_initial30[(0 + slice_idx)];
      }
    }
  }
  for (int slice_idx = 0; slice_idx < 6; slice_idx++) {
    int mapped_idx;
    mapped_idx = ((0 < 1) ? dstate284dGM_inv_mpg17[(((0 + slice_idx) * 1) + 0)] : -1);
    if ((mapped_idx >= 0)) {
      dstate284dGM_der32[(((0 + slice_idx) * 1) + mapped_idx)] = state_derivatives_gm31[(0 + slice_idx)];
    }
  }
  dd state_dot37[6];
  dd dstate_dot438dGM_der35[6];
  dd dstate_dot438dinitial_der36[36];
  for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
    for (int mapped_idx = 0;; mapped_idx++) {
      int mappings_full_idx_symbol;
      mappings_full_idx_symbol = (mapped_idx + (1 * (0 + slice_idx)));
      if ((mappings_full_idx_symbol >= 6)) {
        break;
      } else {
        int al_index_name_symbol;
        al_index_name_symbol = dstate_dot438dGM_mpg18[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 1))) {
          break;
        } else {
          dstate_dot438dGM_der35[((1 * (0 + slice_idx)) + mapped_idx)] = dstate284dGM_der32[(((3 + slice_idx) * 1) + al_index_name_symbol)];
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
        al_index_name_symbol = dstate_dot438dinitial_mpg26[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 6))) {
          break;
        } else {
          dstate_dot438dinitial_der36[((6 * (0 + slice_idx)) + mapped_idx)] = dstate284dinitial_der33[(((3 + slice_idx) * 6) + al_index_name_symbol)];
        }
      }
    }
  }
  for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
    state_dot37[(0 + slice_idx)] = state34[(3 + slice_idx)];
  }
  {
    for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
      xdot11[(0 + slice_idx)] = state_dot37[(0 + slice_idx)];
    }
  }
  dd dist240;
  dd ddist2522dGM_der38[1];
  dd ddist2522dinitial_der39[6];
  dd dist3inv43;
  dd ddist3inv535dGM_der41[1];
  dd ddist3inv535dinitial_der42[6];
  for (int loop_var44 = 0; loop_var44 < 1; loop_var44++) {
    int mapped_idx;
    mapped_idx = loop_var44;
    int al_index_name_symbol;
    al_index_name_symbol = ddist2522dGM_mpg14[loop_var44];
    ddist2522dGM_der38[mapped_idx] = dd_add(dd_add(dd_mul(dd_mul(2.0 , state34[0]) , dstate284dGM_der32[((0 * 1) + al_index_name_symbol)]) , dd_mul(dd_mul(2.0 , state34[1]) , dstate284dGM_der32[((1 * 1) + al_index_name_symbol)])) , dd_mul(dd_mul(2.0 , state34[2]) , dstate284dGM_der32[((2 * 1) + al_index_name_symbol)]));
  }
  for (int loop_var45 = 0; loop_var45 < 6; loop_var45++) {
    int mapped_idx;
    mapped_idx = loop_var45;
    int al_index_name_symbol;
    al_index_name_symbol = ddist2522dinitial_mpg22[loop_var45];
    ddist2522dinitial_der39[mapped_idx] = dd_add(dd_add(dd_mul(dd_mul(2.0 , state34[0]) , dstate284dinitial_der33[((0 * 6) + al_index_name_symbol)]) , dd_mul(dd_mul(2.0 , state34[1]) , dstate284dinitial_der33[((1 * 6) + al_index_name_symbol)])) , dd_mul(dd_mul(2.0 , state34[2]) , dstate284dinitial_der33[((2 * 6) + al_index_name_symbol)]));
  }
  dist240 = dd_add(dd_add((state34[0] * state34[0]) , (state34[1] * state34[1])) , (state34[2] * state34[2]));
  for (int loop_var46 = 0; loop_var46 < 1; loop_var46++) {
    int mapped_idx;
    mapped_idx = loop_var46;
    int al_index_name_symbol;
    al_index_name_symbol = ddist3inv535dGM_mpg20[loop_var46];
    ddist3inv535dGM_der41[mapped_idx] = dd_mul(dd_neg(1.0) , dd_mul(1.0e0 , dd_mul(dd_div(1.0 , (dd_mul(dist240 , dd_sqrt(dist240)) * dd_mul(dist240 , dd_sqrt(dist240)))) , dd_add(dd_mul(dist240 , dd_mul(0.5 , dd_mul(dd_sqrt(dd_div(1.0 , dist240)) , ddist2522dGM_der38[al_index_name_symbol]))) , dd_mul(dd_sqrt(dist240) , ddist2522dGM_der38[al_index_name_symbol])))));
  }
  for (int loop_var47 = 0; loop_var47 < 6; loop_var47++) {
    int mapped_idx;
    mapped_idx = loop_var47;
    int al_index_name_symbol;
    al_index_name_symbol = ddist3inv535dinitial_mpg28[loop_var47];
    ddist3inv535dinitial_der42[mapped_idx] = dd_mul(dd_neg(1.0) , dd_mul(1.0e0 , dd_mul(dd_div(1.0 , (dd_mul(dist240 , dd_sqrt(dist240)) * dd_mul(dist240 , dd_sqrt(dist240)))) , dd_add(dd_mul(dist240 , dd_mul(0.5 , dd_mul(dd_sqrt(dd_div(1.0 , dist240)) , ddist2522dinitial_der39[al_index_name_symbol]))) , dd_mul(dd_sqrt(dist240) , ddist2522dinitial_der39[al_index_name_symbol])))));
  }
  dist3inv43 = dd_div_d_dd(1.0e0 , dd_mul(dist240 , dd_sqrt(dist240)));
  for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
    for (int mapped_idx = 0;; mapped_idx++) {
      int mappings_full_idx_symbol;
      mappings_full_idx_symbol = (mapped_idx + (1 * (3 + slice_idx)));
      if ((mappings_full_idx_symbol >= 6)) {
        break;
      } else {
        int al_index_name_symbol;
        al_index_name_symbol = dstate_dot438dGM_mpg18[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 1))) {
          break;
        } else {
          dstate_dot438dGM_der35[((1 * (3 + slice_idx)) + mapped_idx)] = dd_add(dd_mul(dd_mul(GM13 , dd_neg(state34[(0 + slice_idx)])) , ddist3inv535dGM_der41[al_index_name_symbol]) , dd_mul(dist3inv43 , dd_add(dd_mul(GM13 , dd_neg(dstate284dGM_der32[(((0 + slice_idx) * 1) + al_index_name_symbol)])) , dd_mul(dd_neg(state34[(0 + slice_idx)]) , 1.0))));
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
        al_index_name_symbol = dstate_dot438dinitial_mpg26[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 6))) {
          break;
        } else {
          dstate_dot438dinitial_der36[((6 * (3 + slice_idx)) + mapped_idx)] = dd_add(dd_mul(dd_mul(GM13 , dd_neg(state34[(0 + slice_idx)])) , ddist3inv535dinitial_der42[al_index_name_symbol]) , dd_mul(dist3inv43 , dd_add(dd_mul(GM13 , dd_neg(dstate284dinitial_der33[(((0 + slice_idx) * 6) + al_index_name_symbol)])) , dd_mul(dd_neg(state34[(0 + slice_idx)]) , 0.0))));
        }
      }
    }
  }
  for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
    state_dot37[(3 + slice_idx)] = dd_mul(dd_mul(GM13 , dd_neg(state34[(0 + slice_idx)])) , dist3inv43);
  }
  {
    for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
      xdot11[(3 + slice_idx)] = state_dot37[(3 + slice_idx)];
    }
  }
  for (int slice_idx = 0; slice_idx < (36 / 6); slice_idx++) {
    for (int dx_slice_idx = 0; dx_slice_idx < 6; dx_slice_idx++) {
      int al_index_name_symbol;
      al_index_name_symbol = (dx_slice_idx + 0);
      int func_slice_idx;
      func_slice_idx = ((6 * slice_idx) + dx_slice_idx + 6);
      xdot11[func_slice_idx] = dstate_dot438dinitial_der36[(((0 + slice_idx) * 6) + al_index_name_symbol)];
    }
  }
  for (int slice_idx = 0; slice_idx < 6; slice_idx++) {
    int al_index_name_symbol;
    al_index_name_symbol = 0;
    int func_slice_idx;
    func_slice_idx = (slice_idx + 42);
    xdot11[func_slice_idx] = dstate_dot438dGM_der35[(((0 + slice_idx) * 1) + al_index_name_symbol)];
  }
  return 0;
}

