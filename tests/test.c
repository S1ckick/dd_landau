#include <math.h>

static inline long double get_dfdx_cell(int full_idx0, int dx_mapped_size1, int al_idx2, int inv_mapping_period3, int *restrict dx_idx_mappings4, long double *restrict der_vec5) {
  return ((al_idx2 < inv_mapping_period3) ? ((dx_idx_mappings4[((inv_mapping_period3 * full_idx0) + al_idx2)] >= 0) ? der_vec5[((dx_mapped_size1 * full_idx0) + dx_idx_mappings4[((inv_mapping_period3 * full_idx0) + al_idx2)])] : 0.0) : 0.0);
}

static inline long double get_dfdx_cell_dx(int full_idx0, int dx_mapped_size1, int al_idx2, int inv_mapping_period3, int *restrict dx_idx_mappings4, long double *restrict der_vec5) {
  return ((al_idx2 < inv_mapping_period3) ? ((al_idx2 == full_idx0) ? 1.0 : ((dx_idx_mappings4[((inv_mapping_period3 * full_idx0) + al_idx2)] >= 0) ? der_vec5[((dx_mapped_size1 * full_idx0) + dx_idx_mappings4[((inv_mapping_period3 * full_idx0) + al_idx2)])] : 0.0)) : 0.0);
}

static inline long double get_dfdx_var(int al_idx7, int inv_mapping_period8, int *restrict dx_idx_mappings9, long double *restrict der_vec10) {
  return ((((al_idx7 < inv_mapping_period8) ? dx_idx_mappings9[al_idx7] : -1) >= 0) ? der_vec10[((al_idx7 < inv_mapping_period8) ? dx_idx_mappings9[al_idx7] : -1)] : 0.0);
}

int test_function_1(long double *restrict test_function_111, long double *restrict a12, long double b13) {
  long double c14;
  c14 = ((b13 + 0.5e0) - a12[0]);
  {
    for (int slice_idx = 0; slice_idx < 3; slice_idx++) {
      test_function_111[(0 + slice_idx)] = (a12[(0 + slice_idx)] + b13);
    }
  }
  return 0;
}

