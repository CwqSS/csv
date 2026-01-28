#ifndef EXTRACT_HEADER
#define EXTRACT_HEADER
#include <stdlib.h>
#include <stdio.h>

typedef struct csv_instance {
    char ** values;
} csv_instance;

typedef struct csv_schema {
    char ** columns;
    int quantity_of_columns;
} csv_schema;

typedef struct csv_page {
    csv_instance ** instances;
    size_t size;
} csv_page;

struct csv;

struct csv * create_csv(const char * filename, size_t page_size);
struct csv* extract_from_file(const char * filename, size_t page_size, size_t quantity_of_pages_to_load);
void delete_csv(struct csv * csv);
struct csv_page * get_csv_data(struct csv * csv);
struct csv_schema * get_csv_schema(struct csv * csv);


#endif