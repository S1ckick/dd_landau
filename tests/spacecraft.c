#include <math.h>
#include "../qd/include/qd/c_dd.h"

static inline double* get_dfdx_cell(int full_idx0, int dx_mapped_size1, int al_idx2, int inv_mapping_period3, int *restrict dx_idx_mappings4, dd *restrict der_vec5) {
  return ((al_idx2 < inv_mapping_period3) ? ((dx_idx_mappings4[((inv_mapping_period3 * full_idx0) + al_idx2)] >= 0) ? der_vec5[((dx_mapped_size1 * full_idx0) + dx_idx_mappings4[((inv_mapping_period3 * full_idx0) + al_idx2)])] : 0.0) : 0.0);
}

static inline double* get_dfdx_cell_dx(int full_idx0, int dx_mapped_size1, int al_idx2, int inv_mapping_period3, int *restrict dx_idx_mappings4, dd *restrict der_vec5) {
  return ((al_idx2 < inv_mapping_period3) ? ((al_idx2 == full_idx0) ? 1.0 : ((dx_idx_mappings4[((inv_mapping_period3 * full_idx0) + al_idx2)] >= 0) ? der_vec5[((dx_mapped_size1 * full_idx0) + dx_idx_mappings4[((inv_mapping_period3 * full_idx0) + al_idx2)])] : 0.0)) : 0.0);
}

static inline double* get_dfdx_var(int al_idx7, int inv_mapping_period8, int *restrict dx_idx_mappings9, dd *restrict der_vec10) {
  return ((((al_idx7 < inv_mapping_period8) ? dx_idx_mappings9[al_idx7] : -1) >= 0) ? der_vec10[((al_idx7 < inv_mapping_period8) ? dx_idx_mappings9[al_idx7] : -1)] : 0.0);
}

;
int xdot(dd *restrict xdot11, dd *restrict x12, dd GM13) {
  const static int dstate283dGM_mpg14[6] = { 0, 0, 0, 0, 0, 0 };
  const static int dstate283dGM_inv_mpg15[6] = { 0, 0, 0, 0, 0, 0 };
  const static int ddist2521dGM_mpg16[1] = { 0 };
  const static int ddist2521dGM_inv_mpg17[1] = { 0 };
  const static int ddist3inv534dGM_mpg18[1] = { 0 };
  const static int ddist3inv534dGM_inv_mpg19[1] = { 0 };
  const static int dstate_dot437dGM_mpg20[6] = { 0, 0, 0, 0, 0, 0 };
  const static int dstate_dot437dGM_inv_mpg21[6] = { 0, 0, 0, 0, 0, 0 };
  const static int dstate283dinitial_mpg22[36] = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5 };
  const static int dstate283dinitial_inv_mpg23[36] = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5 };
  const static int ddist2521dinitial_mpg24[6] = { 0, 1, 2, 3, 4, 5 };
  const static int ddist2521dinitial_inv_mpg25[6] = { 0, 1, 2, 3, 4, 5 };
  const static int ddist3inv534dinitial_mpg26[6] = { 0, 1, 2, 3, 4, 5 };
  const static int ddist3inv534dinitial_inv_mpg27[6] = { 0, 1, 2, 3, 4, 5 };
  const static int dstate_dot437dinitial_mpg28[36] = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5 };
  const static int dstate_dot437dinitial_inv_mpg29[36] = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5 };
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
  dd dstate283dGM_der32[6];
  dd dstate283dinitial_der33[36];
  for (int slice_idx = 0; slice_idx < 6; slice_idx++) {
    for (int mapped_idx = 0;; mapped_idx++) {
      int mappings_full_idx_symbol;
      mappings_full_idx_symbol = (mapped_idx + (1 * (0 + slice_idx)));
      if ((mappings_full_idx_symbol >= 6)) {
        break;
      } else {
        int al_index_name_symbol;
        al_index_name_symbol = dstate283dGM_mpg14[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 1))) {
          break;
        } else {
          dstate283dGM_der32[((1 * (0 + slice_idx)) + mapped_idx)] = 0.0;
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
        al_index_name_symbol = dstate283dinitial_mpg22[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 6))) {
          break;
        } else {
          dstate283dinitial_der33[((6 * (0 + slice_idx)) + mapped_idx)] = 0.0;
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
      mapped_idx = (((0 + dx_slice_idx) < 6) ? dstate283dinitial_inv_mpg23[(((0 + df_slice_idx) * 6) + (0 + dx_slice_idx))] : -1);
      if ((mapped_idx >= 0)) {
        int slice_idx;
        slice_idx = ((df_slice_idx * 6) + dx_slice_idx);
        dstate283dinitial_der33[(((0 + df_slice_idx) * 6) + mapped_idx)] = state_derivatives_initial30[(0 + slice_idx)];
      }
    }
  }
  for (int slice_idx = 0; slice_idx < 6; slice_idx++) {
    int mapped_idx;
    mapped_idx = ((0 < 1) ? dstate283dGM_inv_mpg15[(((0 + slice_idx) * 1) + 0)] : -1);
    if ((mapped_idx >= 0)) {
      dstate283dGM_der32[(((0 + slice_idx) * 1) + mapped_idx)] = state_derivatives_gm31[(0 + slice_idx)];
    }
  }
  dd state_dot37[6];
  dd dstate_dot437dGM_der35[6];
  dd dstate_dot437dinitial_der36[36];
  for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
    for (int mapped_idx = 0;; mapped_idx++) {
      int mappings_full_idx_symbol;
      mappings_full_idx_symbol = (mapped_idx + (1 * (0 + slice_idx)));
      if ((mappings_full_idx_symbol >= 6)) {
        break;
      } else {
        int al_index_name_symbol;
        al_index_name_symbol = dstate_dot437dGM_mpg20[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 1))) {
          break;
        } else {
          dstate_dot437dGM_der35[((1 * (0 + slice_idx)) + mapped_idx)] = dstate283dGM_der32[(((3 + slice_idx) * 1) + al_index_name_symbol)];
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
        al_index_name_symbol = dstate_dot437dinitial_mpg28[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 6))) {
          break;
        } else {
          dstate_dot437dinitial_der36[((6 * (0 + slice_idx)) + mapped_idx)] = dstate283dinitial_der33[(((3 + slice_idx) * 6) + al_index_name_symbol)];
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
  dd ddist2521dGM_der38[1];
  dd ddist2521dinitial_der39[6];
  dd dist3inv43;
  dd ddist3inv534dGM_der41[1];
  dd ddist3inv534dinitial_der42[6];
  for (int loop_var44 = 0; loop_var44 < 1; loop_var44++) {
    int mapped_idx;
    mapped_idx = loop_var44;
    int al_index_name_symbol;
    al_index_name_symbol = ddist2521dGM_mpg16[loop_var44];
    ddist2521dGM_der38[mapped_idx] = c_dd_add(c_dd_add(c_dd_mul(c_dd_mul(2.0 , state34[0]) , dstate283dGM_der32[((0 * 1) + al_index_name_symbol)]) , c_dd_mul(c_dd_mul(2.0 , state34[1]) , dstate283dGM_der32[((1 * 1) + al_index_name_symbol)])) , c_dd_mul(c_dd_mul(2.0 , state34[2]) , dstate283dGM_der32[((2 * 1) + al_index_name_symbol)]));
  }
  for (int loop_var45 = 0; loop_var45 < 6; loop_var45++) {
    int mapped_idx;
    mapped_idx = loop_var45;
    int al_index_name_symbol;
    al_index_name_symbol = ddist2521dinitial_mpg24[loop_var45];
    ddist2521dinitial_der39[mapped_idx] = c_dd_add(c_dd_add(c_dd_mul(c_dd_mul(2.0 , state34[0]) , dstate283dinitial_der33[((0 * 6) + al_index_name_symbol)]) , c_dd_mul(c_dd_mul(2.0 , state34[1]) , dstate283dinitial_der33[((1 * 6) + al_index_name_symbol)])) , c_dd_mul(c_dd_mul(2.0 , state34[2]) , dstate283dinitial_der33[((2 * 6) + al_index_name_symbol)]));
  }
  dist240 = c_dd_add(c_dd_add((state34[0] * state34[0]) , (state34[1] * state34[1])) , (state34[2] * state34[2]));
  for (int loop_var46 = 0; loop_var46 < 1; loop_var46++) {
    int mapped_idx;
    mapped_idx = loop_var46;
    int al_index_name_symbol;
    al_index_name_symbol = ddist3inv534dGM_mpg18[loop_var46];
    ddist3inv534dGM_der41[mapped_idx] = c_dd_mul(-1.0 , c_dd_mul(1.0e0 , c_dd_mul(c_dd_div(1.0 / (c_dd_mul(dist240 , sqrt(dist240)) * c_dd_mul(dist240 , sqrt(dist240)))) , c_dd_add(c_dd_mul(dist240 , c_dd_mul(0.5 , c_dd_mul(sqrt(c_dd_div(1.0 / dist240)) , ddist2521dGM_der38[al_index_name_symbol]))) , c_dd_mul(sqrt(dist240) , ddist2521dGM_der38[al_index_name_symbol])))));
  }
  for (int loop_var47 = 0; loop_var47 < 6; loop_var47++) {
    int mapped_idx;
    mapped_idx = loop_var47;
    int al_index_name_symbol;
    al_index_name_symbol = ddist3inv534dinitial_mpg26[loop_var47];
    ddist3inv534dinitial_der42[mapped_idx] = c_dd_mul(-1.0 , c_dd_mul(1.0e0 , c_dd_mul(c_dd_div(1.0 / (c_dd_mul(dist240 , sqrt(dist240)) * c_dd_mul(dist240 , sqrt(dist240)))) , c_dd_add(c_dd_mul(dist240 , c_dd_mul(0.5 , c_dd_mul(sqrt(c_dd_div(1.0 / dist240)) , ddist2521dinitial_der39[al_index_name_symbol]))) , c_dd_mul(sqrt(dist240) , ddist2521dinitial_der39[al_index_name_symbol])))));
  }
  dist3inv43 = c_dd_div_d_dd(1.0e0 / c_dd_mul(dist240 , sqrt(dist240)));
  for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
    for (int mapped_idx = 0;; mapped_idx++) {
      int mappings_full_idx_symbol;
      mappings_full_idx_symbol = (mapped_idx + (1 * (3 + slice_idx)));
      if ((mappings_full_idx_symbol >= 6)) {
        break;
      } else {
        int al_index_name_symbol;
        al_index_name_symbol = dstate_dot437dGM_mpg20[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 1))) {
          break;
        } else {
          dstate_dot437dGM_der35[((1 * (3 + slice_idx)) + mapped_idx)] = c_dd_add(c_dd_mul(c_dd_mul(GM13 , -state34[(0 + slice_idx)]) , ddist3inv534dGM_der41[al_index_name_symbol]) , c_dd_mul(dist3inv43 , c_dd_add(c_dd_mul(GM13 , -dstate283dGM_der32[(((0 + slice_idx) * 1) + al_index_name_symbol)]) , c_dd_mul(-state34[(0 + slice_idx)] , 1.0))));
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
        al_index_name_symbol = dstate_dot437dinitial_mpg28[mappings_full_idx_symbol];
        if (((al_index_name_symbol < 0) || (mapped_idx >= 6))) {
          break;
        } else {
          dstate_dot437dinitial_der36[((6 * (3 + slice_idx)) + mapped_idx)] = c_dd_add(c_dd_mul(c_dd_mul(GM13 , -state34[(0 + slice_idx)]) , ddist3inv534dinitial_der42[al_index_name_symbol]) , c_dd_mul(dist3inv43 , c_dd_add(c_dd_mul(GM13 , -dstate283dinitial_der33[(((0 + slice_idx) * 6) + al_index_name_symbol)]) , c_dd_mul(-state34[(0 + slice_idx)] , 0.0))));
        }
      }
    }
  }
  for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
    state_dot37[(3 + slice_idx)] = c_dd_mul(c_dd_mul(GM13 , -state34[(0 + slice_idx)]) , dist3inv43);
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
      xdot11[func_slice_idx] = dstate_dot437dinitial_der36[(((0 + slice_idx) * 6) + al_index_name_symbol)];
    }
  }
  for (int slice_idx = 0; slice_idx < 6; slice_idx++) {
    int al_index_name_symbol;
    al_index_name_symbol = 0;
    int func_slice_idx;
    func_slice_idx = (slice_idx + 42);
    xdot11[func_slice_idx] = dstate_dot437dGM_der35[(((0 + slice_idx) * 1) + al_index_name_symbol)];
  }
  return 0;
}

