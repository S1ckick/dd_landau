#include "dd.h"
static inline double get_dfdx_cell(int full_idx0, int dx_mapped_size1, int al_idx2, int inv_mapping_period3, int *restrict dx_idx_mappings4, int *restrict der_vec5);

static inline double get_dfdx_cell_dx(int full_idx0, int dx_mapped_size1, int al_idx2, int inv_mapping_period3, int *restrict dx_idx_mappings4, int *restrict der_vec5);
static inline double get_dfdx_var(int al_idx7, int inv_mapping_period8, int *restrict dx_idx_mappings9, int *restrict der_vec10);

int test_function_1(dd *restrict test_function_111, dd *restrict v112, dd v213);
