#include "Matrix.h"
#include <stdlib.h>

#define R_STS BadRowNumber
#define C_STS BadColNumber
#define S_STS Success

struct matrix_st {
	size_t rows, cols;
	float **data;
};

bool mat_row_OOB( Matrix mat, size_t row ) {
	return row <= 0 || row > mat->rows;
}

bool mat_col_OOB( Matrix mat, size_t col ) {
	return col <= 0 || col > mat->cols;
}

Matrix mat_create( size_t rows, size_t cols ) {
	if (!rows || !cols) return NULL;
	Matrix mtx = (Matrix) malloc(sizeof(Matrix));
	if (!mtx) return NULL;
	mtx->rows = rows;
	mtx->cols = cols;
	mtx->data = (float**) malloc(sizeof(float*) * rows);
	if (!mtx->data) {
		free(mtx);
		return NULL;
	}
	for (int row_i = 0; row_i < rows; ++row_i) {
		mtx->data[row_i] = (float*) malloc(sizeof(float) * cols);
		if (!mtx->data[row_i]) {
			for (int prev_row = 0; prev_row < row_i; ++prev_row)
				free(mtx->data[prev_row]);
			free(mtx->data);
			free(mtx);
			return NULL;
		}
		if (rows == cols)
			mtx->data[row_i][row_i] = 1;
	}
	return mtx;
}

void mat_destroy( Matrix mat ) {
	if (!mat) return;
	for (int row_i = 0; row_i < mat->rows; ++row_i)
		free(mat->data[row_i]);
	free(mat->data);
	free(mat);
}

void mat_init( Matrix mat, const float data[] ) {
	if (!mat) return;
	size_t num_rows = mat->rows,
	       num_cols = mat->cols,
	       index = 0, row_i = 0, col_i;
	for (; row_i < num_rows; ++row_i) {
		for (col_i = 0; col_i < num_cols; ++col_i) {
			index = (row_i * num_cols) + col_i;
			mat->data[row_i][col_i] = data[index];
		}
	}
}

Matrix mat_duplicate( const Matrix mat ) {
	if (!mat) return NULL;
	size_t num_rows = mat->rows,
	       num_cols = mat->cols,
	       row_i = 0 , col_i;
	Matrix dupe = mat_create(num_rows, num_cols);
	if (!dupe) return NULL;
	for (; row_i < num_rows; ++row_i) {
		for (col_i = 0; col_i < num_cols; ++col_i) {
			dupe->data[row_i][col_i] = mat->data[row_i][col_i];
		}
	}
	return dupe;
}

bool mat_equals( const Matrix m1, const Matrix m2 ) {
	bool null1 = !m1,
	     null2 = !m2,
	     rows_eq, cols_eq;
	if (null1 ^ null2) return false;
	if (null1 && null2) return true;
	rows_eq = m1->rows == m2->rows;
	cols_eq = m1->cols == m2->cols;
	if (!(rows_eq && cols_eq)) return false;
	size_t num_rows = m1->rows,
	       num_cols = m1->cols;
	float value1, value2;
	for (int row_i = 0; row_i < num_rows; ++row_i) {
		for (int col_i = 0; col_i < num_cols; ++col_i) {
			value1 = m1->data[row_i][col_i];
			value2 = m2->data[row_i][col_i];
			if (value1 != value2) return false;
		}
	}
	return true;
}

Status mat_get_cell( const Matrix mat, float *data, size_t row, size_t col ) {
	if (mat_row_OOB(mat, row) || !mat) return R_STS;
	if (mat_col_OOB(mat, col)) return C_STS;
	size_t row_index = row - 1, col_index = col - 1;
	*data = mat->data[row_index][col_index];
	return S_STS;
}

Status mat_set_cell( Matrix mat, float data, size_t row, size_t col ) {
	if (mat_row_OOB(mat, row) || !mat) return R_STS;
	if (mat_col_OOB(mat, col)) return C_STS;
	size_t row_index = row - 1, col_index = col - 1;
	mat->data[row_index][col_index] = data;
	return S_STS;
}

Status mat_get_row( const Matrix mat, float data[], size_t row ) {
	if (mat_row_OOB(mat, row) || !mat) return R_STS;
	float value, *value_ptr = &value;
	size_t num_cols = mat->cols, col_i = 1;
	Status status;
	for (; col_i <= num_cols; ++col_i) {
		status = mat_get_cell(mat, value_ptr, row, col_i);
		if (status != S_STS) return C_STS;
		data[col_i - 1] = value;
	}
	return S_STS;
}

Status mat_set_row( Matrix mat, const float data[], size_t row ) {
	if (mat_row_OOB(mat, row) || !mat) return R_STS;
	size_t col_i = 1, num_cols = mat->cols;
	Status status;
	for (; col_i <= num_cols; ++col_i) {
		status = mat_set_cell(mat, data[col_i - 1], row, col_i);
		if (status != S_STS) return C_STS;
	}
	return S_STS;
}

void mat_scalar_mult( Matrix mat, float data ) {
	if (data == 1 || !mat) return;
	size_t num_rows = mat->rows,
	       num_cols = mat->cols,
	       row_i = 1, col_i;
	float value, new_value, *value_ptr = &value;
	for (; row_i <= num_rows; ++row_i) {
		for (col_i = 1; col_i <= num_cols; ++col_i) {
			mat_get_cell(mat, value_ptr, row_i, col_i);
			new_value = value * data;
			mat_set_cell(mat, new_value, row_i, col_i);
		}
	}
}

Matrix mat_transpose( const Matrix mat ) {
	if (!mat) return NULL;
	size_t num_t_rows = mat->cols,
	       num_t_cols = mat->rows,
	       t_row = 1, t_col;
	Matrix t_mtx = mat_create(num_t_rows, num_t_cols);
	if (!t_mtx) return NULL;
	float value, *value_ptr = &value;
	Status status;
	for (; t_row <= num_t_rows; ++t_row) {
		for (t_col = 1; t_col <= num_t_cols; ++t_col) {
			status = mat_get_cell(mat, value_ptr, t_col, t_row);
			if (status != S_STS) {
				mat_destroy(t_mtx);
				return NULL;
			}
			status = mat_set_cell(t_mtx, value, t_row, t_col);
			if (status != S_STS) {
				mat_destroy(t_mtx);
				return NULL;
			}
		}
	}
	return t_mtx;
}

Matrix mat_mult( const Matrix m1, const Matrix m2 ) {
	if (!m1 || !m2) return NULL;
	if (m1->cols != m2->rows) return NULL;
	size_t num_rows = m1->rows,
	       num_cols = m2->cols,
	       prod_row = 1, prod_col,
	       num_shared = m1->cols, shared;
	Matrix prod_mtx = mat_create(num_rows, num_cols);
	if (!prod_mtx) return NULL;
	float sum, value1, value2, 
	      *value_ptr1 = &value1, *value_ptr2 = &value2;
	Status status1, status2, status_prod;
	for (; prod_row <= num_rows; ++prod_row) {
		for (prod_col = 1; prod_col <= num_cols; ++prod_col) {
			for (shared = 1, sum = 0; shared <= num_shared; ++shared) {
				status1 = mat_get_cell(m1, value_ptr1, prod_row, shared);
				status2 = mat_get_cell(m2, value_ptr2, shared, prod_col);
				if (status1 != S_STS || status2 != S_STS) {
					mat_destroy(prod_mtx);
					return NULL;
				}
				sum += value1 * value2;
			}
			status_prod = mat_set_cell(prod_mtx, sum, prod_row, prod_col);
			if (status_prod != S_STS) {
				mat_destroy(prod_mtx);
				return NULL;
			}

		}
	}
	return prod_mtx;
}

void mat_print( const Matrix mat, FILE *stream ) {
	if (!mat) return;
	size_t num_rows = mat->rows,
	       num_cols = mat->cols,
	       row_i = 1, col_i;
	float value, *value_ptr = &value;
	fprintf(stream, "%d rows, %d columns:\n", num_rows, num_cols);
	for (; row_i <= num_rows; ++row_i) {
		for (col_i = 1; col_i <= num_cols; ++col_i) {
			mat_get_cell(mat, value_ptr, row_i, col_i);
			fprintf(stream, "%8.3f", value);
			if (col_i == num_cols)
				fprintf(stream, "\n");
		}
	}
}

int main( void ) {
	return EXIT_SUCCESS;
}
