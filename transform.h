#include "extract.h"
#ifndef TRANSFORM_HEADER
#define TRANSFORM_HEADER
struct int_matrix {
    int xCount;
    int yCount;
    int **values;
};

struct int_matrix * transform(struct csv_page * page, int qtt_columns, int skip);
void delete_int_matrix(struct int_matrix * matrix);

#endif