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
}

Matrix mat_duplicate( const Matrix mat ) {
	if (!mat) return NULL;
	return NULL;
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

void mat_scalar_mult( Matrix mat, float data ) {
	if (data == 1 || !mat) return;
	return;
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
	return S_STS;
}

Status mat_get_row( const Matrix mat, float data[], size_t row ) {
	if (mat_row_OOB(mat, row) || !mat) return R_STS;
	return S_STS;
}

Status mat_set_row( Matrix mat, const float data[], size_t row ) {
	if (mat_row_OOB(mat, row) || !mat) return R_STS;
	return S_STS;
}

Matrix mat_mult( const Matrix m1, const Matrix m2 ) {
	if (!m1 || !m2) return NULL;
	return NULL;
}

Matrix mat_transpose( const Matrix mat ) {
	if (!mat) return NULL;
	return NULL;
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
	return;
}

int main( void ) {
	float value, *value_ptr = &value;
	Matrix mtx = mat_create(4, 4);
	mat_print(mtx, stdout);
	mat_get_cell(mtx, value_ptr, 1, 1);
	printf("\n%8.3f\n", value);
	mat_destroy(mtx);
	return EXIT_SUCCESS;
}
