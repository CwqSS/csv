#include <stdlib.h>
#include <string.h>
#include "extract.h"
#include "transform.h"

struct int_matrix * transform(struct csv_page * page, int qtt_columns, int skip) {
    int ** values  = malloc(sizeof(int) * page->size);

    for(int i = 0; i < page->size; i++) {
        values[i] = malloc(sizeof(int) * (qtt_columns - skip));
        struct csv_instance * instance = page->instances[i];
        for(int j = 0; j < qtt_columns - skip; j++) {
            values[i][j] = strcmp(instance->values[j + skip], "sim") == 0 ? 1 : 0;
        }
    }

    struct int_matrix * matrix = malloc(sizeof(struct int_matrix));
    matrix->values = values;
    matrix->yCount = (qtt_columns - skip);
    matrix->xCount = page->size;

    return matrix;
}

void delete_int_matrix(struct int_matrix * matrix) {
    for(int i = 0; i < matrix->xCount; i++) {
        free(matrix->values[i]);
    }
    free(matrix->values);
    free(matrix);
}