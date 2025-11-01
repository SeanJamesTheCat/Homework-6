#include "Matrix.h"
#include <stdlib.h>

struct matrix_st {
	size_t rows, cols;
	float **data;
};

Matrix mat_create( size_t rows, size_t cols ) {
	if (rows == 0 || cols == 0) return NULL;
	Matrix mtx = (Matrix) malloc(sizeof(Matrix));
	if (mtx == NULL) return NULL;
	mtx->rows = rows;
	mtx->cols = cols;
	mtx->data = (float**) malloc(sizeof(float*) * rows);
	if (mtx->data == NULL) {
		free(mtx);
		return NULL;
	}
	for (int row_i = 0; row_i < rows; ++row_i) {
		mtx->data[row_i] = (float*) malloc(sizeof(float) * cols);
		if (mtx->data[row_i] == NULL) {
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
	if (mat == NULL) return;
	for (int row_i = 0; row_i < mat->rows; ++row_i)
		free(mat->data[row_i]);
	free(mat->data);
	free(mat);
}

void mat_init( Matrix mat, const float data[] ) {
	return;
}

Matrix mat_duplicate( const Matrix mat ) {
	return NULL;
}

bool mat_equals( const Matrix m1, const Matrix m2 ) {
	return true;
}

void mat_scalar_mult( Matrix mat, float data ) {
	return;
}

Status mat_get_cell( const Matrix mat, float *data, size_t row, size_t col ) {
	return Success;
}

Status mat_set_cell( Matrix mat, float data, size_t row, size_t col ) {
	return Success;
}

Status mat_get_row( const Matrix mat, float data[], size_t row ) {
	return Success;
}

Status mat_set_row( Matrix mat, const float data[], size_t row ) {
	return Success;
}

Matrix mat_mult( const Matrix m1, const Matrix m2 ) {
	return NULL;
}

Matrix mat_transpose( const Matrix mat ) {
	return NULL;
}

void mat_print( const Matrix mat, FILE *stream ) {
	return;
}

int main( void ) {
	return EXIT_SUCCESS;
}
